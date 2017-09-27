#include <cmath>
#include <cnbidraw/Engine.hpp>
#include <cnbidraw/Events.hpp>
#include <cnbidraw/EventKeyboard.hpp>
#include "ColorFeedback.hpp"

using namespace cnbi;

bool quit = false;
void callback(draw::EventKeyboard* evt) {
	if(evt->IsPressed(DTKK_ESCAPE))
		quit = true;
}

float sinfunc(float x, float F = 1.0f, float delay = 0.0f) {
	return sin(2.0f*M_PI*x*F - delay);
}

int main(int argc, char** argv) {

	draw::Engine*		 engine;
	draw::Events*		 events;
	cvsa::ColorFeedback* cfeedback;

	CcTimeValue tic;
	CcTimeValue	tic_to;
	float SleepPeriod = 50.0f;
	float SleepDiscrete = 1000.0f;
	float TimeoutPeriod = 2000.0f;
	float dt = 0.0f;
	float cx = 0.0f;
	enum State {FeedbackState, BoomState, TimeoutState};
	State stt = FeedbackState;

	cfeedback = new cvsa::ColorFeedback;
	engine    = new draw::Engine("Test", 1680, 1050);
	events    = new draw::Events(engine);

	engine->Open();
	cfeedback->SetThreshold(0.9f);
	engine->Add("cfeedback", cfeedback);
	
	events->Start();
	events->onKeyboard = callback;

	dt = 0.0f;
	
	CcTime::Tic(&tic);
	while(quit == false) {	

		CcTime::Tic(&tic_to);


		// Continuous feedback loop
		stt = FeedbackState;
		while(true) {
				
			cx = (sinfunc(dt, 0.1f) + 1.0f)/2.0f;
			dt = CcTime::Toc(&tic);
			
			// Check for timeout
			if(CcTime::Toc(&tic_to) >= TimeoutPeriod) {
				stt = TimeoutState;
				break;
			}
		
			// Update feedback and check if threshold is reached
			if(cfeedback->Update(cx) == true) {
				stt = BoomState;	
				break;
			}

			CcTime::Sleep(SleepPeriod);
		}

		switch(stt) {
			case BoomState: 
				cfeedback->SetDiscrete(cvsa::ColorFeedback::AsBoom);
				break;
			case TimeoutState: 
				cfeedback->SetDiscrete(cvsa::ColorFeedback::AsTimeout);
				break;
			default:
				break;
		}
		CcTime::Sleep(SleepDiscrete);	
		cfeedback->Reset();
		dt = 0.0f;
	}

	delete engine;
	delete events;
	delete cfeedback;

	return 0;
}
