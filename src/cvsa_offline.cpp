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
#define CVSA_TARGET_DISTANCE	1.0f

#define CVSA_TRIAL_NUMBER		3

using namespace cnbi;

bool quit = false;
void callback(draw::EventKeyboard* evt) {
	if(evt->IsPressed(DTKK_ESCAPE))
		quit = true;
}

const std::array<float, 4>	white	= {169.0f/256.0f, 169.0f/256.0f, 169.0f/256.0f, 1.0f};
const std::array<float, 4>	dgreen	= {18.0f/256.0f, 86.0f/256.0f, 0.0f, 1.0f};
const std::array<float, 4>	lgreen	= {31.0f/256.0f, 140.0f/256.0f, 0.0f, 1.0f};
const std::array<float, 4>	red		= {140.0f/256.0f, 0.0f, 10.0f/256.0f, 1.0f};

const std::array<float, 2>	tangles = {200.0f, 340.0f};

int main(int argc, char** argv) {

	/*** CNBI Loop initialization ***/
	CcCore::OpenLogger("cvsa_offline");
	CcCore::CatchSIGINT();
	CcCore::CatchSIGTERM();


	/*** Protocol definitions ***/
	std::string		xmlfile = "./extra/xml/cvsa_template.xml";
	std::string		mname = "offline";
	std::string		bname = "cvsa";
	std::string		tname = "cvsa_brbl";
	CCfgConfig		config;
	cvsa::timing_t	cfgtime;
	cvsa::event_t	cfgevent;
	cvsa::graphic_t	cfggraph;
	CCfgTaskset*	taskset = nullptr;

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

	for(auto it=cfggraph.target.angles.begin(); it!=cfggraph.target.angles.end(); ++it) {
		printf("%u - %f\n", it->first, it->second);
	}
	
	for(auto it=cfggraph.target.radius.begin(); it!=cfggraph.target.radius.end(); ++it) {
		printf("%u - %f\n", it->first, it->second);
	}

	/*** Graphic initialization ***/
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
	/*
	engine   = new draw::Engine(cfggraph.window.title, 
								cfggraph.window.width, 
								cfggraph.window.height); 
	events   = new draw::Events(engine);
	fixation = new draw::Cross(cfggraph.fixation.size, 
							   cfggraph.fixation.thick, 
							   white.data());
	cue		 = new draw::Arrow(cfggraph.cue.width, 
							   cfggraph.cue.height,
							   white.data());
	feedback = new cvsa::ColorFeedback;
	tcontrol = new cvsa::TargetControl(cfggraph.target.folder);
	*/

	// Initialize targets
	//targetL = tcontrol->Add(cfggraph.target.width, cfggraph.target.height, 
	//						cfggraph.target.angles[0], cfggraph.target.radius[0]);
	//targetR = tcontrol->Add(cfggraph.target.width, cfggraph.target.height, 
	//						cfggraph.target.angles[1], cfggraph.target.radius[1]);

	//if(targetL == nullptr || targetR == nullptr)
	//	exit(1);

	tcontrol->SetTime(cfgtime.targetmove);
	tcontrol->Generate(CVSA_TRIAL_NUMBER);

	/*** Graphic setup ***/
	//engine->Add("feedback", feedback);
	//engine->Add("fixation", fixation);
	//engine->Add("cue", cue);
	//engine->Add("targetL", targetL);
	//engine->Add("targetR", targetR);

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
		feedback->SetDiscrete(cvsa::ColorFeedback::AsBoom);
		CcTime::Sleep(cfgtime.boom);

		// Random time before target hit
		CcTime::Sleep(tcontrol->WaitRandom(cfgtime.targetmax, cfgtime.targetmin));

		// Target Hit
		tcontrol->Hit(cTarget, lgreen.data());
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

