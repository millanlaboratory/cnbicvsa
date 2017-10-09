#ifndef CNBICVSA_COLORFEEDBACK_CPP
#define CNBICVSA_COLORFEEDBACK_CPP

#include "ColorFeedback.hpp"

namespace cnbi {
	namespace cvsa {

ColorFeedback::ColorFeedback(float radius, float thick) {
	
	this->ring_radius_ = radius;
	this->ring_thick_  = thick;
	this->ring_strk_ = nullptr;
	this->ring_fill_ = nullptr;
	this->ring_fdbk_ = nullptr;
	
	/*** Initialize parameters with default values ***/
	this->SetColorRing(default_color_strk, default_color_fill, default_color_fdbk);
	this->SetColorBoom(default_color_hit, default_color_miss);
	
	this->value_		= 0.0f;
	this->threshold_	= 1.0f;

	this->auto_started_ = false;


	/*** Create inner shapes ***/
	this->ring_strk_ = new draw::Ring(this->ring_radius_, this->ring_thick_, this->ring_color_strk_.data(), 500);
	this->ring_fill_ = new draw::Circle(this->ring_radius_, this->ring_color_fill_.data());
	this->ring_fdbk_  = new draw::Circle(this->ring_radius_, this->ring_color_fdbk_.data());

	// Create fill shape
	this->CreateFill();

	// Create shape
	this->Create();
}

ColorFeedback::~ColorFeedback(void) {
}

void ColorFeedback::CreateFill(void) {


	this->WaitShape();


	dtk_hshape shps[] = { this->ring_fill_->shp_ptr_, 
					      this->ring_fdbk_->shp_ptr_, 
						  this->ring_strk_->shp_ptr_};	

	this->fill_ptr_ = dtk_create_composite_shape(this->fill_ptr_, 3, shps, 1);


	this->PostShape();

}

void ColorFeedback::CreateStroke(void) {
}

void ColorFeedback::SetColorRing(float* stroke, float* fill, float* feedback) {

	this->WaitShape();
	std::copy(stroke,   stroke+4,   std::begin(this->ring_color_strk_));
	std::copy(fill,     fill+4,     std::begin(this->ring_color_fill_));
	std::copy(feedback, feedback+4, std::begin(this->ring_color_fdbk_));
	this->PostShape();
	if(this->ring_strk_ != nullptr)
		this->ring_strk_->SetColor(this->ring_color_strk_.data());
	if(this->ring_fill_ != nullptr)
		this->ring_fill_->SetColor(this->ring_color_fill_.data());
	if(this->ring_fdbk_ != nullptr)
		this->ring_fdbk_->SetColor(this->ring_color_fdbk_.data());
}

void ColorFeedback::SetColorBoom(float* hit, float* miss) {

	this->WaitShape();
	std::copy(hit,  hit+4,  std::begin(this->ring_color_hit_));
	std::copy(miss, miss+4, std::begin(this->ring_color_miss_));
	this->PostShape();
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
	this->ring_fdbk_->SetAlpha(this->value_);

	return this->value_ == 1.0f ? true : false;
}

bool ColorFeedback::AutoUpdate(float mintime, float maxtime) {

	bool result = false;
	float dt, da;

	// First iteration: set autostarted to true, initialize the timer and
	// initialize the time
	if(this->auto_started_ == false) {
		this->auto_started_ = true;
		CcTime::Tic(&(this->auto_timer_));
		
		std::random_device	rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<> dis(mintime, maxtime);
		this->auto_trial_duration_ = dis(gen);
		this->auto_trial_time_ = 0.0f;
		printf("Trial duration: %f [ms]\n", this->auto_trial_duration_);
		return false;
	}

	// Following iterations, compute the elapsed time (dt) from the previous
	// call and update the time
	dt = CcTime::Toc(&(this->auto_timer_));
	CcTime::Tic(&(this->auto_timer_));
	this->auto_trial_time_ = this->auto_trial_time_ + dt; // for debugging

	da = ((dt/1000.0f))/(this->auto_trial_duration_/1000.0f);

	this->value_ = this->value_ + da;
	this->value_ = this->value_ > 1.0f ? 1.0f : this->value_;
	this->ring_fdbk_->SetAlpha(this->value_);

	if(this->value_ >= 1.0f) {
		this->auto_started_ = false;
		this->value_ = 0.0f;
		printf("Elapsed time: %f [ms]\n", this->auto_trial_time_);
		result = true;
	}

	return result;
}

void ColorFeedback::Reset(void) {
	this->Update(0.0f);
	this->ring_strk_->SetColor(this->ring_color_strk_.data());
}

void ColorFeedback::SetDiscrete(unsigned int mode) {
	
	this->WaitShape();

	switch(mode) {
		case ColorFeedback::AsHit:
			this->ring_strk_->SetColor(this->ring_color_hit_.data());
			break;
		case ColorFeedback::AsMiss:
			this->ring_strk_->SetColor(this->ring_color_miss_.data());
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
