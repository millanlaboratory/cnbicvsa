#ifndef CNBICVSA_TARGET_CPP
#define CNBICVSA_TARGET_CPP

#include "Target.hpp"

namespace cnbi {
	namespace cvsa {

Target::Target(float width, float height, float thick, const float* color) : Gallery(width, height) {
	this->ismoving_ = false;
	this->time_ = 1000.0f;

	this->SetStroke(thick, color);

}

Target::~Target(void) {}


void Target::SetPosition(float angle, float radius) {

	float x, y;
	x = radius*cos(angle*M_PI/180.0f);
	y = radius*sin(angle*M_PI/180.0f);
	this->radius_ = radius;
	this->angle_  = angle;
	this->Move(x, y);
	this->setx_ = x;
	this->sety_ = y;
}

void Target::SetTime(float time) {
	this->time_ = time;
}


bool Target::ToCenter(void) {

	bool result = false;
	float cx, cy, cangle, cradius;
	float x, y;
	float ds, dt;

	// First iteration, set ismoving to true and initialize the timer
	if(this->ismoving_ == false) {
		this->ismoving_ = true;
		CcTime::Tic(&(this->timer_));
		return false;
	} 

	// Following iterations, compute the elapsed time (dt) from the previous
	// call and update the timer
	dt = CcTime::Toc(&(this->timer_));
	CcTime::Tic(&(this->timer_));

	// Get the current position and compute polar coordinates
	this->GetPosition(&cx, &cy);
	cangle  = atan2(cy, cx);
	cradius = std::sqrt((std::pow(cx, 2))+(std::pow(cy, 2)));

	// Compute the current movement ds (in polar coordinates) and convert it in
	// cartesian coordiantes
	ds = ((dt/1000.0f)*this->radius_)/(this->time_/1000.0f);
	x = (cradius-ds)*cos(cangle);
	y = (cradius-ds)*sin(cangle);

	// Move the shape to the new coordinates
	this->Move(x, y);

	// Compute the current radius (with respect to the center)
	cradius = std::sqrt((std::pow(x, 2))+(std::pow(y, 2)));

	// Check if the current radius is close to the center
	if(std::fabs(cradius) <= 0.01f) {
		this->ismoving_ = false;
		result = true;
	}

	return result;

}

void Target::Present(unsigned int id) {
	this->Select(id);
	this->Show(draw::Shape::Fill);
	this->Hide(draw::Shape::Stroke);
}

void Target::Hit(void) {
	this->Show(draw::Shape::Stroke);
}

void Target::Reset(void) {
	this->Hide(draw::Shape::Stroke);
	this->Show(draw::Shape::Fill);
	this->Move(this->setx_, this->sety_);
}

	}
}

#endif

