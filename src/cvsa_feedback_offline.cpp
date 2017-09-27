#include <cnbidraw/Engine.hpp>
#include <cnbidraw/Events.hpp>
#include <cnbidraw/EventKeyboard.hpp>
#include <cnbidraw/Cross.hpp>
#include "ColorFeedback.hpp"
#include "Target.hpp"
#include "TargetControl.hpp"

#define FOLDER_IMAGES "./extra/icons/"


// Temporary definitions
#define CVSA_WINDOW_TITLE		"CVSA calibration"
#define CVSA_WINDOW_WIDTH		1680
#define CVSA_WINDOW_HEIGHT		1050

#define CVSA_FIXATION_SIZE		CNBICVSA_COLORFEEDBACK_RING_RADIUS*0.4f*2.0f
#define CVSA_FIXATION_THICK		CVSA_FIXATION_SIZE/4.0f

#define CVSA_TARGET_WIDTH		0.3f
#define CVSA_TARGET_HEIGHT		0.3f
#define CVSA_TARGET_BORDER		0.03f
#define CVSA_TARGET_DISTANCE	1.0f
#define CVSA_TARGET_FOLDER		"./extra/icons/"

#define CVSA_TRIAL_NUMBER		30

#define CVSA_TIMING_ITI			2000.0f
#define CVSA_TIMING_FIXATION	2000.0f
#define CVSA_TIMING_CUE			500.0f
#define CVSA_TIMING_FEEDBACKMIN	4000.0f
#define CVSA_TIMING_FEEDBACKMAX	5000.0f
#define CVSA_TIMING_BOOM		1000.0f
#define CVSA_TIMING_TARGETMIN	500.0f
#define CVSA_TIMING_TARGETMAX	1500.0f
#define CVSA_TIMING_TARGETHIT	1000.0f
#define CVSA_TIMING_TARGETMOVE	2000.0f
#define CVSA_TIMING_TARGETSTOP	500.0f

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

int main(int argc, char** argv) {


	/*** Graphic definitions ***/
	draw::Engine*					engine;
	draw::Events*					events;
	draw::Cross*					fixation;
	cvsa::ColorFeedback*			feedback;
	std::array<cvsa::Target*, 2>	targets;
	cvsa::Target*					ctarget;
	cvsa::TargetControl*			tcontrol;


	/*** Graphic initialization ***/
	engine   = new draw::Engine(CVSA_WINDOW_TITLE, 
								CVSA_WINDOW_WIDTH, 
								CVSA_WINDOW_HEIGHT); 
	events   = new draw::Events(engine);
	fixation = new draw::Cross(CVSA_FIXATION_SIZE, 
							   CVSA_FIXATION_THICK, 
							   white.data());
	feedback = new cvsa::ColorFeedback;

	// Initialize targets
	targets.at(0) = new cvsa::Target(CVSA_TARGET_HEIGHT, 
										   CVSA_TARGET_WIDTH, 
										   CVSA_TARGET_BORDER, 
										   lgreen.data());
	targets.at(1) = new cvsa::Target(CVSA_TARGET_HEIGHT, 
										   CVSA_TARGET_WIDTH, 
										   CVSA_TARGET_BORDER, 
										   lgreen.data());

	for(auto it=targets.begin(); it!=targets.end(); ++it) {
		if((*it)->SetFolder(CVSA_TARGET_FOLDER, {".png"}) == false) {
			printf("[cvsa_feedback_offline] + Cannot access to folder '%s'\n", CVSA_TARGET_FOLDER);
			exit(1);
		}
	}

	// Initialize target control
	tcontrol = new cvsa::TargetControl(2, 
									   (*targets.begin())->Size(), 
									   CVSA_TRIAL_NUMBER); 

	/*** Graphic setup ***/
	engine->Add("feedback", feedback);
	engine->Add("fixation", fixation);
	engine->Add("targetL",  targets.at(0));
	engine->Add("targetR",  targets.at(1));

	targets.at(0)->SetPosition(200.0f, CVSA_TARGET_DISTANCE);
	targets.at(1)->SetPosition(340.0f, CVSA_TARGET_DISTANCE);
	targets.at(0)->SetTime(CVSA_TIMING_TARGETMOVE);
	targets.at(1)->SetTime(CVSA_TIMING_TARGETMOVE);


	engine->Open();
	events->onKeyboard = callback;
	events->Start();

	auto cTrial = 0;
	float cValue;

	while(quit == false) {
	
		// Inter-Trial Interval
		targets.at(0)->Hide();
		targets.at(1)->Hide();
		fixation->Hide();
		CcTime::Sleep(CVSA_TIMING_ITI);

		// Fixation 
		fixation->Show();
		targets.at(0)->Present(tcontrol->At(0));
		targets.at(1)->Present(tcontrol->At(1));
		CcTime::Sleep(CVSA_TIMING_FIXATION);

		// Cue
		ctarget = targets.at(0);
		if(cTrial % 2 == 0)
			ctarget = targets.at(1);
		
		// Feedback (To be random)
		cValue = 0.0f;
		while(quit == false) {
			cValue = cValue + 0.01f;
			if(feedback->Update(cValue))
				break;
			CcTime::Sleep(50.0f);
		}

		// Boom
		feedback->SetDiscrete(cvsa::ColorFeedback::AsBoom);
		CcTime::Sleep(CVSA_TIMING_BOOM);

		// Random time CVSA_TIMING_TARGETMIN-MAX

		// Target Hit
		ctarget->Hit();
		CcTime::Sleep(CVSA_TIMING_TARGETHIT);

		// Target Move
		while(quit == false) {
			if(ctarget->ToCenter() == true)
				break;
		}

		// Target Stop
		CcTime::Sleep(CVSA_TIMING_TARGETSTOP);

		// Resetting
		feedback->Reset();
		ctarget->Reset();
		tcontrol->Next();
		cTrial++;

	}

	

	return 0;
}

