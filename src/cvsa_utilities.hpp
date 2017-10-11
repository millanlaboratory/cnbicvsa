#ifndef CNBICVSA_UTILITIES_HPP
#define CNBICVSA_UTILITIES_HPP

#include <regex>
#include <stdlib.h>
#include <unordered_map>

#include <cnbicore/CcBasic.hpp>
#include <cnbiconfig/CCfgConfig.hpp>

#include <cnbidraw/Engine.hpp>
#include <cnbidraw/Events.hpp>
#include <cnbidraw/EventKeyboard.hpp>
#include <cnbidraw/Cross.hpp>
#include <cnbidraw/Arrow.hpp>
#include <cnbidraw/String.hpp>
#include <cnbidraw/Font.hpp>
#include <cnbidraw/Gallery.hpp>
#include "ColorFeedback.hpp"
#include "Copilot.hpp"

namespace cnbi {
	namespace cvsa {

typedef struct timing_struct {
	float iti;
	float fixation;
	float cue;
	float feedbackmin;
	float feedbackmax;
	float boom;
	float targetmin;
	float targetmax;
	float targethit;
	float targetmove;
	float targetstop;
	float timeout;
} timing_t;

typedef struct event_struct {
	unsigned int fixation;
	unsigned int feedback;
	unsigned int off;
	unsigned int hit;
	unsigned int miss;
	unsigned int targethit;
	unsigned int targetmove;
	unsigned int targetstop;
} event_t;

typedef struct window_struct {
	std::string title;
	unsigned int width;
	unsigned int height;
} window_t;

typedef struct fixation_struct {
	float size;
	float thick;
	float color[4];
} fixation_t;

typedef struct cue_struct {
	float width;
	float height;
	float color[4];
} cue_t;

typedef struct feedback_struct {
	float radius;
	float thick;
	float color_stroke[4];
	float color_fill[4];
	float color_feedback[4];
	float color_hit[4];
	float color_miss[4];
} feedback_t;

typedef struct trialtext_struct {
	float				size;
	float				color[4];
	cnbi::draw::Font*	font;
} trialtext_t;

typedef struct target_struct {
	float				width;
	float				height;
	std::string 		folder;
	std::unordered_map<unsigned int, float>	angles;
	std::unordered_map<unsigned int, float>	radius;
	float color[4];
	std::string			logdir;
} target_t;

typedef struct graphic_struct {
	window_t	window;
	fixation_t	fixation;
	cue_t		cue;
	feedback_t	feedback;
	trialtext_t trialtext;
	target_t	target;
} graphic_t;


bool cvsa_configure_taskset(CCfgConfig* config, CCfgTaskset** taskset, 
							const std::string& mname,
							const std::string& bname, 
							const std::string& tname);
bool xml_configure_timings(CCfgConfig* config, timing_t* timings);
bool xml_configure_events(CCfgConfig* config, event_t* events);
bool xml_configure_graphics(CCfgConfig* config, CCfgTaskset* taskset, graphic_t* graphics);

void hex2rgba(const std::string& hex, float* color);
void ExpandPath(std::string& text);


bool xml_configure_timings(CCfgConfig* config, timing_t* timings) {
	try {

		config->RootEx()->QuickEx("timings/")->SetBranch();
		timings->iti		  = config->BranchEx()->QuickFloatEx("iti");
		timings->fixation	  = config->BranchEx()->QuickFloatEx("fixation");
		timings->cue		  = config->BranchEx()->QuickFloatEx("cue");
		timings->feedbackmin  = config->BranchEx()->QuickFloatEx("feedbackmin");
		timings->feedbackmax  = config->BranchEx()->QuickFloatEx("feedbackmax");
		timings->boom		  = config->BranchEx()->QuickFloatEx("boom");
		timings->targetmin	  = config->BranchEx()->QuickFloatEx("targetmin");
		timings->targetmax	  = config->BranchEx()->QuickFloatEx("targetmax");
		timings->targethit	  = config->BranchEx()->QuickFloatEx("targethit");
		timings->targetmove	  = config->BranchEx()->QuickFloatEx("targetmove");
		timings->targetstop	  = config->BranchEx()->QuickFloatEx("targetstop");
		timings->timeout	  = config->BranchEx()->QuickFloatEx("timeout");
		CcLogConfig("Timings configuration succeed");

	} catch (XMLException e) {
		CcLogException(e.Info());
		CcLogFatal("Timing configuration failed");
		return false;
	}

	return true;
}


bool xml_configure_events(CCfgConfig* config, event_t* events) {
	try {

		config->RootEx()->QuickEx("events/gdfevents/")->SetBranch();
		events->fixation   = config->BranchEx()->QuickGDFIntEx("fixation");
		events->feedback   = config->BranchEx()->QuickGDFIntEx("cfeedback");
		events->off		   = config->BranchEx()->QuickGDFIntEx("off");
		events->hit		   = config->BranchEx()->QuickGDFIntEx("hit");
		events->miss	   = config->BranchEx()->QuickGDFIntEx("miss");
		events->targethit  = config->BranchEx()->QuickGDFIntEx("targethit");
		events->targetmove = config->BranchEx()->QuickGDFIntEx("targetmove");
		events->targetstop = config->BranchEx()->QuickGDFIntEx("targetstop");
		CcLogConfig("Events configuration succeed");

	} catch (XMLException e) {
		CcLogException(e.Info());
		CcLogFatal("Events configuration failed");
		return false;
	}

	return true;
}

bool xml_configure_graphics(CCfgConfig* config, CCfgTaskset* taskset, graphic_t* graphics) {
	try {

		config->RootEx()->QuickEx("configuration/cnbidraw/")->SetBranch();
		graphics->window.title	= config->BranchEx()->QuickStringEx("window/caption");
		graphics->window.width	= config->BranchEx()->QuickIntEx("window/width");
		graphics->window.height	= config->BranchEx()->QuickIntEx("window/height");
		
		graphics->fixation.size	 = config->BranchEx()->QuickFloatEx("fixation/size");
		graphics->fixation.thick = config->BranchEx()->QuickFloatEx("fixation/thick");
		hex2rgba(config->BranchEx()->QuickStringEx("fixation/color"), graphics->fixation.color);
		
		graphics->cue.width  = config->BranchEx()->QuickFloatEx("cue/width");
		graphics->cue.height = config->BranchEx()->QuickFloatEx("cue/height");
		hex2rgba(config->BranchEx()->QuickStringEx("cue/color"), graphics->cue.color);
		
		graphics->feedback.radius = config->BranchEx()->QuickFloatEx("feedback/radius");
		graphics->feedback.thick  = config->BranchEx()->QuickFloatEx("feedback/thick");
		hex2rgba(config->BranchEx()->QuickStringEx("feedback/color/stroke"), graphics->feedback.color_stroke);
		hex2rgba(config->BranchEx()->QuickStringEx("feedback/color/fill"), graphics->feedback.color_fill);
		hex2rgba(config->BranchEx()->QuickStringEx("feedback/color/feedback"), graphics->feedback.color_feedback);
		hex2rgba(config->BranchEx()->QuickStringEx("feedback/color/hit"), graphics->feedback.color_hit);
		hex2rgba(config->BranchEx()->QuickStringEx("feedback/color/miss"), graphics->feedback.color_miss);

		graphics->trialtext.size = config->BranchEx()->QuickFloatEx("trialtext/size");;
		hex2rgba(config->BranchEx()->QuickStringEx("trialtext/color"), graphics->trialtext.color);
		graphics->trialtext.font = new cnbi::draw::Font(config->BranchEx()->QuickStringEx("trialtext/font"));

		graphics->target.width	  = config->BranchEx()->QuickFloatEx("target/width");
		graphics->target.height   = config->BranchEx()->QuickFloatEx("target/height");
		graphics->target.folder	  = config->BranchEx()->QuickStringEx("target/folder");
		ExpandPath(graphics->target.folder);
		hex2rgba(config->BranchEx()->QuickStringEx("target/color"), graphics->target.color);
		graphics->target.logdir	  = config->BranchEx()->QuickStringEx("target/logdir");
		ExpandPath(graphics->target.logdir);
		

		// Target angle and radius
		for(auto it=taskset->Begin(); it!=taskset->End(); ++it) {
			// Target angle field
			if(it->second->HasConfig("angle") == false) {
				CcLogErrorS("Task "<<it->second->name<<" does not define \"angle\"");
				return false;
			}
			graphics->target.angles.emplace(std::make_pair(it->second->id, it->second->config["angle"].Float()));
			
			// Target radius field
			if(it->second->HasConfig("radius") == false) {
				CcLogErrorS("Task "<<it->second->name<<" does not define \"radius\"");
				return false;
			}
			graphics->target.radius.emplace(std::make_pair(it->second->id, it->second->config["radius"].Float()));
		}

	} catch (XMLException e) {
		CcLogException(e.Info());
		printf("%s\n", e.Info().c_str());
		CcLogFatal("Graphics configuration failed");
		return false;
	}

	CcLogConfig("Graphics configuration succeed");
	return true;
}

bool xml_configure_taskset(CCfgConfig* config, CCfgTaskset** taskset, 
							const std::string& mname,
							const std::string& bname, 
							const std::string& tname) {
	try { 
		if(mname.compare("offline") == 0)
			*taskset = config->OfflineEx(bname, tname);
		else if(mname.compare("online") == 0)
			*taskset = config->OnlineEx(bname, tname);
	} catch(XMLException e) {
		CcLogException(e.Info());
		return false;
	}
	return true;
}

bool setup_graphic_engine(cnbi::draw::Engine*& engine, graphic_t* graphic) {

	engine = new cnbi::draw::Engine(graphic->window.title,
									graphic->window.width,
									graphic->window.height);

	return true;	
}

bool setup_graphic_events(cnbi::draw::Events*& events, cnbi::draw::Engine* engine) {
	
	bool retcode = true;

	if(engine == nullptr)
		retcode = false;

	events = new cnbi::draw::Events(engine);

	if(retcode == false)	
		CcLogFatal("Events setup failed");

	return retcode;
}

bool setup_graphic_fixation(cnbi::draw::Cross*& fixation, 
							graphic_t* graphic, 
							cnbi::draw::Engine* engine) {

	bool retcode = true;

	if(engine == nullptr)
		retcode = false;

	fixation = new cnbi::draw::Cross(graphic->fixation.size,
									 graphic->fixation.thick,
									 graphic->fixation.color); 

	if(engine->Add("fixation", fixation) == false)
		retcode = false;
	else
		fixation->Hide();
	
	if(retcode == false)	
		CcLogFatal("Fixation setup failed");

	return retcode;
}

bool setup_graphic_cue(cnbi::draw::Arrow*& cue, 
					   graphic_t* graphic, 
					   cnbi::draw::Engine* engine) {
	
	bool retcode = true;
	
	if(engine == nullptr)
		retcode = false;

	cue	= new cnbi::draw::Arrow(graphic->cue.width, 
								graphic->cue.height,
								graphic->cue.color);
	
	if(engine->Add("cue", cue) == false) 
		retcode = false;
	else
		cue->Hide();

	if(retcode == false)	
		CcLogFatal("Cue setup failed");

	return retcode;
}

bool setup_graphic_feedback(cnbi::cvsa::ColorFeedback*& feedback, 
							graphic_t* graphic, 
							cnbi::draw::Engine* engine) {

	bool retcode = true;

	if(engine == nullptr)
		retcode = false;

	feedback = new cnbi::cvsa::ColorFeedback(graphic->feedback.radius,
											 graphic->feedback.thick);
	feedback->SetColorRing(graphic->feedback.color_stroke,
					  	   graphic->feedback.color_fill,
					  	   graphic->feedback.color_feedback);
	feedback->SetColorBoom(graphic->feedback.color_hit, 
						   graphic->feedback.color_miss);
	
	if(engine->Add("feedback", feedback) == false)
		retcode = false;
	else
		feedback->Hide();
	
	if(retcode == false)
		CcLogFatal("Feedback setup failed");

	return retcode;
}

bool setup_graphic_trialtext(cnbi::draw::String*& text, 
							 graphic_t* graphic, 
							 cnbi::draw::Engine* engine) {

	bool retcode = true;

	if(engine == nullptr)
		retcode = false;

	text = new cnbi::draw::String(graphic->trialtext.size, graphic->trialtext.color);
	text->SetFont(graphic->trialtext.font);
	
	if(engine->Add("trialtext", text) == false)
		retcode = false;
	
	if(retcode == false)
		CcLogFatal("TrialText setup failed");

	return retcode;
}

bool setup_graphic_target(cnbi::cvsa::Target*& tcontrol, 
						  graphic_t* graphic,
						  CCfgTaskset* taskset,
						  cnbi::draw::Engine* engine) {

	bool retcode = true;
	cnbi::draw::Gallery* ctarget;

	if(engine == nullptr)
		retcode = false;

	tcontrol = new cnbi::cvsa::Target(graphic->target.folder);

	
	for(auto it=taskset->Begin(); it!=taskset->End(); ++it) {
		ctarget = tcontrol->Add(graphic->target.width, graphic->target.height,
								graphic->target.angles[it->second->id],
					  			graphic->target.radius[it->second->id]);
		
		if(ctarget == nullptr) {
			retcode = false;
			break;
		} 
		
		if(engine->Add("target"+std::to_string(it->second->id), ctarget) == false) {
			retcode = false;
			break;
		} else {
			ctarget->Hide();
		}

	}
	
	if(retcode == false)
		CcLogFatal("Target setup failed");


	return retcode;

}

bool setup_copilot(cnbi::cvsa::Copilot*& copilot,
						 CCfgTaskset* taskset) {

	bool retcode = true;
	int ctrials;
	float ccatch;

	copilot = new cnbi::cvsa::Copilot;

	try {	
		// Trial number
		for(auto it=taskset->Begin(); it!=taskset->End(); ++it) {
			// Trial field
			if(it->second->HasConfig("trials") == false) {
				CcLogErrorS("Task "<<it->second->name<<" does not define \"trials\"");
				retcode = false;
				break;
			} else {
				ctrials = it->second->config["trials"].Int();
			}
			CcLogInfoS("Number of trials for " << it->second->name<<": "<<ctrials);

			// Catch field
			if(it->second->HasConfig("catch") == false) {
				CcLogWarningS("Task "<<it->second->name<<" does not define \"catch\"");
				ccatch = 0.0f;
			} else {
				ccatch = it->second->config["catch"].Float();
			}
			CcLogInfoS("Percentage of catch trials for " << it->second->name<<": "<<ccatch<<"%");

			retcode = retcode & copilot->Add(it->second->id, ctrials, ccatch);
		}
	} catch(XMLException e) {
		CcLogException(e.Info());
	}

	if(retcode == false)
		printf("Trial setup failed\n");

	return retcode;
}

void hex2rgba(const std::string& hex, float* color) {

	unsigned int r, g, b, a;
	sscanf(hex.c_str(), "#%02x%02x%02x%02x", &r, &g, &b, &a);
	color[0] = r/255.0f;
	color[1] = g/255.0f;
	color[2] = b/255.0f;
	color[3] = a/255.0f;
}

void ExpandPath(std::string& text) {
	static std::regex env("/?\\$\\{?([^}/]+)\\}?/?");
	std::smatch match;
	while(std::regex_search(text, match, env)) {
		const char* s = getenv(match[1].str().c_str());
		const std::string var(s==NULL ? "" : s);
		text.replace(match[0].first, match[0].second, var+"/");
	}
	
}


	}
}

#endif
