#include <cnbidraw/Engine.hpp>
#include <cnbidraw/Events.hpp>
#include <cnbidraw/EventKeyboard.hpp>
#include "Target.hpp"
#include "TargetControl.hpp"

#define FOLDER_IMAGES "./extra/icons/"

using namespace cnbi;

bool quit = false;
void callback(draw::EventKeyboard* evt) {
	if(evt->IsPressed(DTKK_ESCAPE))
		quit = true;
}

int main(int argc, char** argv) {

	std::string		rpath = "./extra/icons/";
	draw::Engine*	engine;
	draw::Events*	events;
	cvsa::Target*	targetL;
	cvsa::Target*	targetR;
	std::vector<std::string> allowed_ext = {".jpg", ".png"};
	CcTimeValue		tic;
	float tcolorL[4] = {31.0f/256.0f, 140.0f/256.0f, 0.0f, 1.0f};
	float tcolorR[4] = {31.0f/256.0f, 140.0f/256.0f, 0.0f, 1.0f};
	unsigned int ntargets = 2;
	unsigned int nobjects = 20;
	unsigned int nrepetitions = 30;
	unsigned int cTrial;


	engine  = new draw::Engine("Test", 800, 600);
	events  = new draw::Events(engine);
	targetL	= new cvsa::Target(0.3f, 0.3f, 0.05f, tcolorL);
	targetR	= new cvsa::Target(0.3f, 0.3f, 0.05f, tcolorR);
	cvsa::TargetControl tcontrol(ntargets, nobjects, nrepetitions);
	cvsa::Target* ctarget;


	if(targetL->SetFolder(rpath, allowed_ext) == false) {
		printf("[test_target] + Cannot access to folder '%s'\n", rpath.c_str());
		exit(1);
	}
	
	if(targetR->SetFolder(rpath, allowed_ext) == false) {
		printf("[test_target] + Cannot access to folder '%s'\n", rpath.c_str());
		exit(1);
	}

	engine->Open();
	engine->Add("targetL", targetL);
	engine->Add("targetR", targetR);
	
	events->Start();
	events->onKeyboard = callback;

	targetL->SetPosition(200.0f, 0.8f);
	targetL->SetTime(5000.0f);
	targetR->SetPosition(340.0f, 0.8f);
	targetR->SetTime(5000.0f);

	cTrial = 0;
	while(quit == false) {

		printf("Hide targets\n");
		targetL->Hide();
		targetR->Hide();
		CcTime::Sleep(1000.0f);

		printf("Present targets\n");
		targetL->Present(tcontrol.At(0));
		targetR->Present(tcontrol.At(1));
		CcTime::Sleep(1000.0f);

		printf("Hit target\n");
		ctarget = targetR;
		if(cTrial % 2 == 0)
			ctarget = targetL;

		ctarget->Hit();
		CcTime::Sleep(1000.0f);

		printf("Move target\n");
		CcTime::Tic(&tic);
		while(quit == false) {

			if(ctarget->ToCenter() == true) {
				printf("elapsed: %f\n", CcTime::Toc(&tic));
				break;
			}

			CcTime::Sleep(100.0f);
		}
		printf("Center target\n");
		CcTime::Sleep(1000.0f);

		printf("Reset target\n");
		ctarget->Reset();
		tcontrol.Next();
		cTrial++;
	}


	delete engine;
	delete events;
	

	return 0;
}
