#include <cnbidraw/Engine.hpp>
#include <cnbidraw/Events.hpp>
#include <cnbidraw/EventKeyboard.hpp>
#include <cnbidraw/Cross.hpp>
#include <cnbidraw/Arrow.hpp>
#include <cnbidraw/Gallery.hpp>
#include "ColorFeedback.hpp"
#include "TargetControl.hpp"


// Temporary definitions
#define CVSA_WINDOW_TITLE		"CVSA calibration"
#define CVSA_WINDOW_WIDTH		1680
#define CVSA_WINDOW_HEIGHT		1050

#define CVSA_FIXATION_SIZE		CNBICVSA_COLORFEEDBACK_RING_RADIUS*0.4f*2.0f
#define CVSA_FIXATION_THICK		CVSA_FIXATION_SIZE/4.0f

#define CVSA_CUE_WIDTH			CNBICVSA_COLORFEEDBACK_RING_RADIUS*0.5f*2.0f
#define CVSA_CUE_HEIGHT			CVSA_CUE_WIDTH/1.0f

#define CVSA_TARGET_WIDTH		0.3f
#define CVSA_TARGET_HEIGHT		0.3f
#define CVSA_TARGET_DISTANCE	1.0f
#define CVSA_TARGET_FOLDER		"./extra/icons/"

#define CVSA_TRIAL_NUMBER		3

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

const std::array<float, 2>	tangles = {200.0f, 340.0f};

int main(int argc, char** argv) {

	/*** Protocol definitions ***/
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


	/*** Graphic initialization ***/
	engine   = new draw::Engine(CVSA_WINDOW_TITLE, 
								CVSA_WINDOW_WIDTH, 
								CVSA_WINDOW_HEIGHT); 
	events   = new draw::Events(engine);
	fixation = new draw::Cross(CVSA_FIXATION_SIZE, 
							   CVSA_FIXATION_THICK, 
							   white.data());
	cue		 = new draw::Arrow(CVSA_CUE_WIDTH, 
							   CVSA_CUE_HEIGHT,
							   white.data());
	feedback = new cvsa::ColorFeedback;
	tcontrol = new cvsa::TargetControl(CVSA_TARGET_FOLDER);

	// Initialize targets
	targetL = tcontrol->Add(CVSA_TARGET_WIDTH, CVSA_TARGET_HEIGHT, tangles.at(0), CVSA_TARGET_DISTANCE);
	targetR = tcontrol->Add(CVSA_TARGET_WIDTH, CVSA_TARGET_HEIGHT, tangles.at(1), CVSA_TARGET_DISTANCE);

	if(targetL == nullptr || targetR == nullptr)
		exit(1);

	tcontrol->SetTime(CVSA_TIMING_TARGETMOVE);
	tcontrol->Generate(CVSA_TRIAL_NUMBER);

	/*** Graphic setup ***/
	engine->Open();
	engine->Add("feedback", feedback);
	engine->Add("fixation", fixation);
	engine->Add("cue", cue);
	engine->Add("targetL", targetL);
	engine->Add("targetR", targetR);


	events->onKeyboard = callback;
	events->Start();

	for(auto cTrial=0; cTrial<CVSA_TRIAL_NUMBER; cTrial++) {
	
		// Inter-Trial Interval
		tcontrol->Hide();
		fixation->Hide();
		cue->Hide();
		CcTime::Sleep(CVSA_TIMING_ITI);

		// Fixation 
		fixation->Show();
		tcontrol->Show();
		CcTime::Sleep(CVSA_TIMING_FIXATION);

		// Cue
		cTarget = 0;
		if(cTrial % 2 == 0) {
			cTarget = 1;
		}
		cue->Rotate(tangles.at(cTarget)+180.0f);
		cue->Show();
		fixation->Hide();
		CcTime::Sleep(CVSA_TIMING_CUE);
		
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
		CcTime::Sleep(CVSA_TIMING_BOOM);

		// Random time before target hit
		CcTime::Sleep(tcontrol->WaitRandom(CVSA_TIMING_TARGETMAX, CVSA_TIMING_TARGETMIN));

		// Target Hit
		tcontrol->Hit(cTarget, lgreen.data());
		CcTime::Sleep(CVSA_TIMING_TARGETHIT);

		// Target Move
		while(quit == false) {
			if(tcontrol->ToCenter(cTarget) == true)
				break;
		}

		// Target Stop
		CcTime::Sleep(CVSA_TIMING_TARGETSTOP);

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

