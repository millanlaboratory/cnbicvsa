#ifndef CNBICVSA_TARGETCONTROL_CPP
#define CNBICVSA_TARGETCONTROL_CPP

#include "TrialControl.hpp"

namespace cnbi {
    namespace cvsa {

TrialControl::TrialControl(void){
	this->nrepetitions_ = 0;
};

TrialControl::~TrialControl(void){};

bool TrialControl::Add(unsigned int id, unsigned int nrepetitions) {
	if(this->map_.insert(std::make_pair(id, nrepetitions)).second == false)
		return false;

	this->nrepetitions_ += nrepetitions;
	return true;
}

void TrialControl::Generate(void) {


	/*** Generate sequence with all given class ids ***/
	for(auto itc=this->map_.begin(); itc!=this->map_.end(); ++itc) {
		for(auto i=0; i<itc->second; i++) {
			this->sequence_.push_back(itc->first);
		}
	}

	/*** Shuffle the order of the sequence ***/
	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(this->sequence_.begin(), this->sequence_.end(), g);

	this->sequence_it_ = this->Begin();
}

unsigned int TrialControl::GetId(void) {
	return std::distance(this->sequence_.begin(), this->sequence_it_);
}

TrialsIt TrialControl::GetIt(void) {
	return this->sequence_it_;
}

TrialsIt TrialControl::Begin(void) {
	return this->sequence_.begin();
}

TrialsIt TrialControl::End(void) {
	return this->sequence_.end();
}

bool TrialControl::Next(void) {
	bool result = false;
	if(this->sequence_it_ != this->End()) {
		this->sequence_it_++;
		result = true;
	}
	return result;
}

bool TrialControl::Previous(void) {
	bool result = false;
	if(this->sequence_it_ != this->Begin()) {
		this->sequence_it_--;
		result = true;
	}
	return result;
}


	}
}
#endif
