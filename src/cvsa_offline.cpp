#include <cnbidraw/Engine.hpp>
#include <cnbidraw/Events.hpp>
#include <cnbidraw/EventKeyboard.hpp>
#include <cnbidraw/Cross.hpp>
#include <cnbidraw/Arrow.hpp>
#include <cnbidraw/Gallery.hpp>
#include "ColorFeedback.hpp"
#include "TargetControl.hpp"
#include "cvsa_utilities.hpp"


// Temporary definitions

#define CVSA_TRIAL_NUMBER		3

using namespace cnbi;

bool quit = false;
void callback(draw::EventKeyboard* evt) {
	if(evt->IsPressed(DTKK_ESCAPE))
		quit = true;
}


int main(int argc, char** argv) {

	/*** CNBI Loop initialization ***/
	CcCore::OpenLogger("cvsa_offline");
	CcCore::CatchSIGINT();
	CcCore::CatchSIGTERM();


	/*** Protocol definitions ***/
	std::string		xmlfile = "./extra/xml/cvsa_template.xml";
	std::string		mname = "offline";
	std::string		bname = "cvsa";
	std::string		tname = "cvsa_btrl";
	CCfgConfig		config;
	CCfgTaskset*	taskset = nullptr;
	cvsa::timing_t	cfgtime;
	cvsa::event_t	cfgevent;
	cvsa::graphic_t	cfggraph;

	unsigned int cTarget;
	float cValue;

	/*** Graphic definitions ***/
	draw::Engine*					engine;
	draw::Events*					events;
	draw::Cross*					fixation;
	draw::Arrow*					cue;
	cvsa::ColorFeedback*			feedback;
	draw::Gallery*					targetL;
	draw::Gallery*					targetR;
	cvsa::TargetControl*			tcontrol;


	/*** XML setup ***/
	// Importing the xml file
	CcLogInfoS("Importing XML file: " << xmlfile);
	try {
		config.ImportFileEx(xmlfile);
	} catch (XMLException e) {
		CcLogException(e.Info());
		exit(1);
	}

	// Configuration
	if( (cvsa::xml_configure_taskset(&config, &taskset, mname, bname, tname) == false)||
		(cvsa::xml_configure_graphics(&config, taskset, &cfggraph) == false) ||
		(cvsa::xml_configure_events(&config, &cfgevent) == false)		||
		(cvsa::xml_configure_timings(&config, &cfgtime) == false)) {
		CcLogFatal("Configuration failed");
		exit(1);
	}

	/*** Graphic setup ***/
	if(cvsa::setup_graphic_engine(engine, &cfggraph) == false)
		exit(1);
	if(cvsa::setup_graphic_events(events, engine) == false)
		exit(1);
	if(cvsa::setup_graphic_feedback(feedback, &cfggraph, engine) == false)
		exit(1);
	if(cvsa::setup_graphic_cue(cue, &cfggraph, engine) == false)
		exit(1);
	if(cvsa::setup_graphic_fixation(fixation, &cfggraph, engine) == false)
		exit(1);
	if(cvsa::setup_graphic_target(tcontrol, &cfggraph, taskset, engine) == false)
		exit(1);

	tcontrol->SetTime(cfgtime.targetmove);
	tcontrol->Generate(CVSA_TRIAL_NUMBER);

	engine->Open();
	events->onKeyboard = callback;
	events->Start();

	for(auto cTrial=0; cTrial<CVSA_TRIAL_NUMBER; cTrial++) {
	
		// Inter-Trial Interval
		tcontrol->Hide();
		fixation->Hide();
		cue->Hide();
		CcTime::Sleep(cfgtime.iti);

		// Fixation 
		fixation->Show();
		tcontrol->Show();
		CcTime::Sleep(cfgtime.fixation);

		// Cue
		cTarget = 0;
		if(cTrial % 2 == 0) {
			cTarget = 1;
		}
		cue->Rotate(cfggraph.target.angles[cTarget]+180.0f);
		cue->Show();
		fixation->Hide();
		CcTime::Sleep(cfgtime.cue);
		
		// Feedback (To be random)
		fixation->Show();
		cue->Hide();
		cValue = 0.0f;
		while(quit == false) {
			cValue = cValue + 0.01f;
			if(feedback->Update(cValue))
				break;
			CcTime::Sleep(50.0f);
		}

		if(quit == true)
			goto shutdown;

		// Boom
		feedback->SetDiscrete(cvsa::ColorFeedback::AsHit);
		CcTime::Sleep(cfgtime.boom);

		// Random time before target hit
		CcTime::Sleep(tcontrol->WaitRandom(cfgtime.targetmax, cfgtime.targetmin));

		// Target Hit
		tcontrol->Hit(cTarget, cfggraph.target.color);
		CcTime::Sleep(cfgtime.targethit);

		// Target Move
		while(quit == false) {
			if(tcontrol->ToCenter(cTarget) == true)
				break;
		}

		// Target Stop
		CcTime::Sleep(cfgtime.targetstop);

		// Resetting
		feedback->Reset();
		tcontrol->Reset();
		tcontrol->Next();

	}

shutdown:

	delete engine;
	delete events;
	

	return 0;
}

