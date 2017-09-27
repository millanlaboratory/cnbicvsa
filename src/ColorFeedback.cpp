#ifndef CNBICVSA_COLORFEEDBACK_CPP
#define CNBICVSA_COLORFEEDBACK_CPP

#include "ColorFeedback.hpp"

namespace cnbi {
	namespace cvsa {

ColorFeedback::ColorFeedback(void) {
	
	/*** Initialize parameters with default values ***/
	this->ring_radius_ = CNBICVSA_COLORFEEDBACK_RING_RADIUS;
	this->ring_thick_  = CNBICVSA_COLORFEEDBACK_RING_THICK;
	this->ring_color_  = {169.0f/256.0f, 169.0f/256.0f, 169.0f/256.0f, 1.0f};
	
	this->bg_radius_ = CNBICVSA_COLORFEEDBACK_FILL_RADIUS;
	this->bg_color_  = {57.0f/256.0f, 57.0f/256.0f, 57.0f/256.0f, 1.0f};
	
	this->feedback_radius_ = CNBICVSA_COLORFEEDBACK_FEEDBACK_RADIUS;
	this->feedback_color_  = {18.0f/256.0f, 86.0f/256.0f, 0.0f, 0.0f};

	this->value_		= 0.0f;
	this->threshold_	= 1.0f;
	this->boom_color_	= {31.0f/256.0f, 140.0f/256.0f, 0.0f, 0.0f};
	this->timeout_color_= {140.0f/256.0f, 0.0f, 10.0f/256.0f, 0.0f};

	/*** Create inner shapes ***/
	this->ring_ = new draw::Ring(this->ring_radius_, this->ring_thick_, this->ring_color_.data(), 500);
	this->bg_ = new draw::Circle(this->bg_radius_, this->bg_color_.data());
	this->feedback_ = new draw::Circle(this->feedback_radius_, this->feedback_color_.data());

	// Create fill shape
	this->CreateFill();

	// Create shape
	this->Create();
}

ColorFeedback::~ColorFeedback(void) {
}

void ColorFeedback::CreateFill(void) {


	this->WaitShape();


	dtk_hshape shps[] = { this->bg_->shp_ptr_, 
					      this->feedback_->shp_ptr_, 
						  this->ring_->shp_ptr_};	

	this->fill_ptr_ = dtk_create_composite_shape(this->fill_ptr_, 3, shps, 1);


	this->PostShape();

}

void ColorFeedback::CreateStroke(void) {
}

float ColorFeedback::GetValue(void) {
	float value;
	this->WaitShape();
	value = this->value_;
	this->PostShape();
	return value;
}

bool ColorFeedback::Update(float value) {

	this->WaitShape();
	value = value/this->threshold_;
	value = value < 1.0f ? value : 1.0f;
	this->value_ = value;
	this->PostShape();
	this->feedback_->SetAlpha(this->value_);

	return this->value_ == 1.0f ? true : false;
}

void ColorFeedback::Reset(void) {
	this->Update(0.0f);
	this->ring_->SetColor(this->ring_color_.data());
}

void ColorFeedback::SetDiscrete(unsigned int mode) {
	
	this->WaitShape();

	switch(mode) {
		case ColorFeedback::AsBoom:
			this->ring_->SetColor(this->boom_color_.data());
			break;
		case ColorFeedback::AsTimeout:
			this->ring_->SetColor(this->timeout_color_.data());
			break;
		default:
			break;
	}

	this->PostShape();
}

void ColorFeedback::SetThreshold(float th) {
	this->WaitShape();
	th = th < 1.0f ? th : 1.0f;
	this->threshold_ = th;
	this->PostShape();
}



	}
}

#endif
