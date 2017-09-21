#include <cnbidraw/Engine.hpp>
#include <cnbidraw/Events.hpp>
#include <cnbidraw/EventKeyboard.hpp>
#include "TargetGallery.hpp"

#define FOLDER_IMAGES "extra/icons/"
using namespace cnbi;

bool quit = false;
void callback(draw::EventKeyboard* evt) {
	if(evt->IsPressed(DTKK_ESCAPE))
		quit = !quit;
}


int main(int argc, char** argv) {

	draw::Engine*			 engine;
	draw::Events*			 events;
	cvsa::TargetGallery*	 gallery;
	std::string				 path(FOLDER_IMAGES);
	std::vector<std::string> allowed_ext = {".jpg", ".png"};

	gallery = new cvsa::TargetGallery(0.0f, 0.0f, 0.1f, 0.1f);
	engine  = new draw::Engine("Test", 1680, 1050);
	events  = new draw::Events(engine);


	if(gallery->SetFolder(path, allowed_ext) == false) {
		printf("[test_gallery] + Cannot access to folder '%s'\n", path.c_str());
		exit(1);
	}

	

	printf("[test_gallery] + Found %lu images on folder '%s':\n", gallery->list.size(), path.c_str());
	for(auto it = gallery->list.begin(); it!=gallery->list.end(); ++it) 
		printf("               |- %s\n", (*it).c_str());

	

	//engine->Open();
	//
	//events->Start();
	//events->onKeyboard = callback;


	while(quit == false) {

		CcTime::Sleep(100.0f);
	}

	return 0;
}


