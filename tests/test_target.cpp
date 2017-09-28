#include <cnbidraw/Engine.hpp>
#include <cnbidraw/Events.hpp>
#include <cnbidraw/EventKeyboard.hpp>
#include "TargetControl.hpp"

#define FOLDER_IMAGES "./extra/icons/"

using namespace cnbi;

bool quit = false;
void callback(draw::EventKeyboard* evt) {
	if(evt->IsPressed(DTKK_ESCAPE))
		quit = true;
}

int main(int argc, char** argv) {

	draw::Engine*	engine;
	draw::Events*	events;
	draw::Gallery*	targetL;
	draw::Gallery*	targetR;
	cvsa::TargetControl* tcontrol;


	CcTimeValue		tic;
	float tcolorL[4] = {31.0f/256.0f, 140.0f/256.0f, 0.0f, 1.0f};
	float tcolorR[4] = {31.0f/256.0f, 140.0f/256.0f, 0.0f, 1.0f};
	unsigned int nrepetitions = 30;
	unsigned int cTrial;
	unsigned int cTarget;


	engine  = new draw::Engine("Test", 800, 600);
	events  = new draw::Events(engine);
	tcontrol = new cvsa::TargetControl(FOLDER_IMAGES);
	targetL = tcontrol->Add(0.3f, 0.3f, 200.0f, 0.8f);
	targetR = tcontrol->Add(0.3f, 0.3f, 340.0f, 0.8f);

	if(targetL == nullptr || targetR == nullptr)
		goto shutdown;
	
	tcontrol->SetTime(5000.0f);
	tcontrol->Generate(nrepetitions);

	engine->Open();
	engine->Add("targetL", targetL);
	engine->Add("targetR", targetR);
	
	events->Start();
	events->onKeyboard = callback;


	cTrial = 0;
	while(quit == false) {

		// Hide targets
		tcontrol->Hide();
		CcTime::Sleep(1000.0f);

		// Show targets
		tcontrol->Show();
		CcTime::Sleep(1000.0f);

		// Hit target
		cTarget = 0;
		if(cTrial % 2 == 0)
			cTarget = 1;

		tcontrol->Hit(cTarget, dtk_green);
		CcTime::Sleep(1000.0f);

		// Move target
		while(quit == false) {
			if(tcontrol->ToCenter(cTarget) == true)
				break;

			CcTime::Sleep(100.0f);
		}
		
		CcTime::Sleep(1000.0f);
		tcontrol->Reset();
		tcontrol->Next();
		cTrial++;
	}


shutdown:
	printf("Shutdown\n");
	delete engine;
	delete events;
	exit(0);
	

	return 0;
}
