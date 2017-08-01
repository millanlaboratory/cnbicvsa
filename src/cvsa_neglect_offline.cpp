#include <random>
#include <cnbicore/CcCore.hpp>
#include <cnbicore/CcThreadSafe.hpp>
#include <cnbicore/CcTime.hpp>
#include <cnbidraw/Engine.hpp>
#include <cnbidraw/Events.hpp>
#include <cnbidraw/EventKeyboard.hpp>
#include <cnbidraw/Cross.hpp>
#include <cnbidraw/Arrow.hpp>
#include "InfiniteBar.hpp"


#define CNBICVSA_FIX_SIZE	0.2f
#define CNBICVSA_FIX_THICK	0.03f
#define CNBICVSA_FIX_COLOR	dtk_white
#define CNBICVSA_CUE_WIDTH	0.3f
#define CNBICVSA_CUE_HEIGHT	0.2f
#define CNBICVSA_CUE_COLOR	dtk_green
#define CNBICVSA_BAR_WIDTH	0.6f
#define CNBICVSA_BAR_HEIGHT	0.1f
#define CNBICVSA_BAR_NOBJS	7
#define CNBICVSA_BAR_WOBJS	0.05f

#define CNBICVSA_FIX_TIME	    1000.0f
#define CNBICVSA_CUE_TIME	    500.0f
#define CNBICVSA_CFDBK_TIME_MIN	    3000.0f
#define CNBICVSA_CFDBK_TIME_MAX	    5000.0f
#define CNBICVSA_BAR_VELOCITY	    0.01f

using namespace cnbi;

int main(int argc, char** argv) {

    
    // Draw elements definitions
    draw::Engine*	engine;
    draw::Events* 	events;
    draw::Cross*	fix;
    draw::Arrow*	cue;
    cvsa::InfiniteBar*	bar;

    // Cnbiloop definitions

    // Protocol definitions
    unsigned int idtrial = 1;
    unsigned int ntrials = 30;


    // Draw elements initialization
    fix    = new draw::Cross(CNBICVSA_FIX_SIZE, CNBICVSA_FIX_THICK, CNBICVSA_FIX_COLOR);
    cue    = new draw::Arrow(CNBICVSA_CUE_WIDTH, CNBICVSA_CUE_HEIGHT, CNBICVSA_CUE_COLOR);
    bar	   = new cvsa::InfiniteBar(CNBICVSA_BAR_WIDTH, CNBICVSA_BAR_HEIGHT, 
				   CNBICVSA_BAR_NOBJS, CNBICVSA_BAR_WOBJS);
    engine = new draw::Engine;
    events = new draw::Events(engine);

    // Draw elements setup
    engine->Open();
    events->Open();
    fix->Create();
    cue->Create();
    bar->Create();
    bar->Move(0.0f, -0.2f);
    cue->Rotate(40.0f);
    engine->Add("fix", fix);
    engine->Add("cue", cue);
    engine->Add("bar", bar);

    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<> rndtime(CNBICVSA_CFDBK_TIME_MIN, 
					     CNBICVSA_CFDBK_TIME_MAX);
   
    while( (idtrial <= ntrials) & engine->IsRunning() ) {
	printf("[cnbicvsa] - Trial %d/%d\n", idtrial++, ntrials);

	// Fixation scene
	fix->Show();
	cue->Hide();
	CcTime::Sleep(CNBICVSA_FIX_TIME);

	// Cue scene
	fix->Hide();
	cue->Show();
	CcTime::Sleep(CNBICVSA_CUE_TIME);

	// Continuous feedback scene
	fix->Show();
	cue->Hide();

	CcTimeValue tic;
    	CcTime::Tic(&tic);
	auto ctime = rndtime(gen);
	while(true) {
	    bar->Update(-CNBICVSA_BAR_VELOCITY);

	    if(CcTime::Toc(&tic) > ctime) {
		printf("Elapsed: %f\n", CcTime::Toc(&tic));
		break;
	    }

	    CcTime::Sleep(50.0f);
	}

	
	// Discrete feedback
	fix->Show();
	cue->Hide();
	
	// ITI
	fix->Show();
	cue->Hide();

    }
    
    engine->Stop();
    events->Stop();

    delete engine;
    delete events;
    delete fix;
    delete cue;
    delete bar;

    return 0;
}

