#include <unistd.h>
#include <cnbiloop/ClLoop.hpp>
#include <cnbiloop/ClTobiId.hpp>

#include <cnbidraw/Engine.hpp>
#include <cnbidraw/Events.hpp>
#include <cnbidraw/EventKeyboard.hpp>
#include <cnbidraw/Cross.hpp>
#include <cnbidraw/Arrow.hpp>
#include <cnbidraw/String.hpp>
#include <cnbidraw/Gallery.hpp>
#include "ColorFeedback.hpp"
#include "Target.hpp"
#include "Copilot.hpp"
#include "cvsa_utilities.hpp"

#define CVSA_EXECUTABLE_NAME	"cvsa_offline"

using namespace cnbi;

bool quit  = false;
bool start = false;
void callback(draw::EventKeyboard* evt) {
	if(evt->IsPressed(DTKK_ESCAPE)) {
		quit = true;
	} else if(evt->IsPressed(DTKK_SPACE)) {
		start = true;
	}
}

void usage(void) { 
	printf("[%s] Usage: %s [OPTION]...\n\n", CVSA_EXECUTABLE_NAME, CVSA_EXECUTABLE_NAME);
	printf("       -x       xml file: path to the xml file\n");
	printf("       -m       modality: modality name (e.g., offline, online)\n");
	printf("       -b       block: block name (e.g., cvsa) \n");
	printf("       -t       taskset: taskset name (e.g., cvsa_brbl)\n");
	printf("       -l       logname: target log filename\n");
	printf("       -h       display this help and exit\n");
}


int main(int argc, char** argv) {
	
	int opt;
	
	std::string	xmlfile;
	std::string mname;
	std::string bname;
	std::string tname;
	std::string lname;  

	while((opt = getopt(argc, argv, "x:m:b:t:l:")) != -1) {
		if(opt == 'x')
			xmlfile.assign(optarg);
		else if(opt == 'm')
			mname.assign(optarg);
		else if(opt == 'b')
			bname.assign(optarg);
		else if(opt == 't')
			tname.assign(optarg);
		else if(opt == 'l')
			lname.assign(optarg);
		else {
			usage();
			CcCore::Exit(opt == 'h' ? EXIT_SUCCESS : EXIT_FAILURE);
		}
	}

	/*** CNBI Loop initialization ***/
	CcCore::OpenLogger(CVSA_EXECUTABLE_NAME);
	CcCore::CatchSIGINT();
	CcCore::CatchSIGTERM();
	ClLoop::Configure();
	
	// Tools for TOBI iD
	ClTobiId id(ClTobiId::SetOnly);
	IDMessage idm;
	IDSerializerRapid ids(&idm);
	idm.SetDescription(CVSA_EXECUTABLE_NAME);
	idm.SetFamilyType(IDMessage::FamilyBiosig);
	idm.SetEvent(0);

	/*** Protocol definitions ***/
	CCfgConfig		config;
	CCfgTaskset*	taskset = nullptr;
	cvsa::timing_t	cfgtime;
	cvsa::event_t	cfgevent;
	cvsa::graphic_t	cfggraph;
	cvsa::Copilot*	copilot = nullptr;
	
	std::string  cTaskName, cTrialText;
	unsigned int cTaskId, cTaskEvt, cTargetId, cTargetEvt;

	/*** Graphic definitions ***/
	draw::Engine*			engine = nullptr;
	draw::Events*			events = nullptr;
	draw::Cross*			fixation;
	draw::Arrow*			cue;
	draw::String*			trialtext;
	cvsa::ColorFeedback*	feedback;
	cvsa::Target*			targets;

	/*** CNBI Loop connection ***/
	CcLogInfo("Connecting to the CNBI loop...");
	if(ClLoop::Connect() == false) {
		CcLogFatal("Cannot connect to the CNBI loop");
		CcCore::Exit(0);
	}

	// Retrieve configuration from nameserver (if not already provided)
	if(xmlfile.empty() == true)
		xmlfile = ClLoop::nms.RetrieveConfig("cvsa", "xml");
	if(mname.empty() == true)
		mname = ClLoop::nms.RetrieveConfig("cvsa", "modality");
	if(bname.empty() == true)
		bname = ClLoop::nms.RetrieveConfig("cvsa", "block");
	if(tname.empty() == true)
		tname = ClLoop::nms.RetrieveConfig("cvsa", "taskset");
	if(lname.empty() == true)
		lname = ClLoop::nms.RetrieveConfig("cvsa", "logname");
	CcLogConfigS("Modality="  << mname <<
				 ", Block="   << bname <<
				 ", Taskset=" << tname <<
				 ", Xml="	  << xmlfile <<
				 ", Logname=" << lname);

	if(xmlfile.empty() || mname.empty() || bname.empty() || tname.empty()) {
		CcLogFatal("Configuration not provided neither via arguments nor via nameserver");
		CcCore::Exit(0);
	}


	/*** CNBI Loop configuration ***/
	if(id.Attach("/bus") == false) {
		CcLogFatal("Cannot attach to TobiId");
		CcCore::Exit(0);
	}

	/*** XML setup ***/
	// Importing the xml file
	CcLogInfoS("Importing XML file: " << xmlfile);
	try {
		config.ImportFileEx(xmlfile);
	} catch (XMLException e) {
		CcLogException(e.Info());
		CcCore::Exit(0);
	}

	// Configuration
	if( (cvsa::xml_configure_taskset(&config, &taskset, mname, bname, tname) == false)||
		(cvsa::xml_configure_graphics(&config, taskset, &cfggraph) == false) ||
		(cvsa::xml_configure_events(&config, &cfgevent) == false)		||
		(cvsa::xml_configure_timings(&config, &cfgtime) == false)) {
		CcLogFatal("Configuration failed");
		CcCore::Exit(0);
	}

	/*** Graphic setup ***/
	if(cvsa::setup_graphic_engine(engine, &cfggraph) == false)
		CcCore::Exit(0);
	if(cvsa::setup_graphic_events(events, engine) == false)
		CcCore::Exit(0);
	if(cvsa::setup_graphic_feedback(feedback, &cfggraph, engine) == false)
		CcCore::Exit(0);
	if(cvsa::setup_graphic_trialtext(trialtext, &cfggraph, engine) == false)
		CcCore::Exit(0);
	if(cvsa::setup_graphic_cue(cue, &cfggraph, engine) == false)
		CcCore::Exit(0);
	if(cvsa::setup_graphic_fixation(fixation, &cfggraph, engine) == false)
		CcCore::Exit(0);
	if(cvsa::setup_graphic_target(targets, &cfggraph, taskset, engine) == false)
		CcCore::Exit(0);

	/*** Protocol setup ***/
	if(cvsa::setup_copilot(copilot, taskset) == false)
		CcCore::Exit(0);

	copilot->Generate();
	targets->SetTime(cfgtime.targetmove);
	targets->Generate(copilot->GetSize());

	if(targets->Export(lname)) {
		CcLogConfigS("Target log file stored at: "<<lname);
	} else {
		CcLogWarningS("Cannot open target log at: "<<lname);
	}

	engine->Open();
	events->onKeyboard = callback;
	events->Start();

	trialtext->Show();
	trialtext->SetText("Press space to start...");

	while(start == false) {
		if(quit == true)
			goto shutdown;

		CcTime::Sleep(50.0f);
	}

	for(auto it=copilot->Begin(); it!=copilot->End(); ++it) {
	
		cTaskId    = taskset->GetTaskEx(copilot->GetId())->id;
		cTaskEvt   = taskset->GetTaskEx(copilot->GetId())->gdf;
		cTaskName  = taskset->GetTaskEx(copilot->GetId())->name;
		cTrialText = std::to_string(copilot->GetPosition()+1) + 
					 "/" + std::to_string(copilot->GetSize());

		CcLogInfoS("Trial "<<copilot->GetPosition()+1 << "/" << copilot->GetSize() 
					<< " [" << cTaskName << "|" << cTaskId << "|" << cTaskEvt << "]");

		// Inter-Trial Interval
		feedback->Show();
		feedback->Reset();
		trialtext->Show();
		trialtext->SetText(cTrialText);
		targets->Hide();
		fixation->Hide();
		cue->Hide();
		CcTime::Sleep(cfgtime.iti);

		// Fixation 
		idm.SetEvent(cfgevent.fixation);
		id.SetMessage(&ids);
		trialtext->Hide();
		fixation->Show();
		targets->Show();
		CcTime::Sleep(cfgtime.fixation);
		idm.SetEvent(cfgevent.fixation + cfgevent.off);
		id.SetMessage(&ids);

		// Cue
		idm.SetEvent(cTaskEvt);
		id.SetMessage(&ids);
		cue->Rotate(cfggraph.target.angles[cTaskId]+180.0f);
		cue->Show();
		fixation->Hide();
		CcTime::Sleep(cfgtime.cue);
		idm.SetEvent(cTaskEvt + cfgevent.off);
		id.SetMessage(&ids);
		
		// Feedback (To be random)
		fixation->Show();
		cue->Hide();
		idm.SetEvent(cfgevent.feedback);
		id.SetMessage(&ids);
		while(quit == false) { 
			if(feedback->AutoUpdate(cfgtime.feedbackmin, cfgtime.feedbackmax) == true)
				break;
			CcTime::Sleep(50.0f);
		}
		idm.SetEvent(cfgevent.feedback + cfgevent.off);
		id.SetMessage(&ids);

		if(quit == true)
			goto shutdown;

		// Boom
		feedback->SetDiscrete(cvsa::ColorFeedback::AsHit);
		idm.SetEvent(cfgevent.hit);
		id.SetMessage(&ids);
		CcTime::Sleep(cfgtime.boom);
		idm.SetEvent(cfgevent.hit + cfgevent.off);
		id.SetMessage(&ids);

		// Random time before target hit
		CcTime::Sleep(targets->WaitRandom(cfgtime.targetmax, cfgtime.targetmin));

		// Target Hit
		cTargetId  = cTaskId;
		cTargetEvt = cTaskEvt; 
		if(copilot->IsCatch()) {
			cTargetId  = taskset->GetTaskEx(copilot->GetCatchId())->id;
			cTargetEvt = taskset->GetTaskEx(copilot->GetCatchId())->gdf;
		}
		CcLogInfoS("Target expected|selected: " << cTaskId <<"|"<<cTargetId);

		targets->Hit(cTargetId, cfggraph.target.color);
		idm.SetEvent(cfgevent.targethit + cTargetEvt);
		id.SetMessage(&ids);
		CcTime::Sleep(cfgtime.targethit);
		idm.SetEvent(cfgevent.targethit + cTargetEvt + cfgevent.off);
		id.SetMessage(&ids);

		targets->Reset();
		CcTime::Sleep(cfgtime.targetstop);

		// Resetting
		feedback->Reset();
		targets->Next();
		copilot->Next();
	}

shutdown:
	if(engine != nullptr)
		delete engine;
	if(events != nullptr)
		delete events;
	if(copilot != nullptr)
		delete copilot;

	CcCore::Exit(0);
}

