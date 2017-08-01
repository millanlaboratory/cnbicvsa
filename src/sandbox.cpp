#include <cnbicore/CcCore.hpp>
#include <cnbicore/CcThreadSafe.hpp>
#include <cnbicore/CcTime.hpp>
#include <cnbidraw/Engine.hpp>
#include <cnbidraw/Events.hpp>
#include <cnbidraw/EventKeyboard.hpp>
#include "InfiniteBar.hpp"

using namespace cnbi;

bool space_pressed = true;
void callback(draw::EventKeyboard* evt) {
	if(evt->IsPressed(DTKK_SPACE))
		space_pressed = !space_pressed;
}

int main(int argc, char** argv) {

	float x, y;
	draw::Engine engine;
	draw::Events events(&engine);

	cvsa::InfiniteBar bar(0.6, 0.1, 7, 0.05);

	bar.Create();

	events.onKeyboard = callback;
	engine.Add("bar", &bar);

	engine.Open();
	events.Start();

	CcCore::CatchSIGINT();
	CcCore::CatchSIGTERM();

	while(true) {
		if(space_pressed == false)
			break;
		
		CcTime::Sleep(50.0f);
		
		if(CcCore::receivedSIGAny.Get()) {
			break;
		}
	}
	while(engine.IsRunning()) {

		if(space_pressed)
			bar.Update(0.01f);

		CcTime::Sleep(50.0f);

		if(CcCore::receivedSIGAny.Get()) {
			break;
		}
		
	}

	return 0;
}
