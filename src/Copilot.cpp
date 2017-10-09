#ifndef CNBICVSA_COPILOT_CPP
#define CNBICVSA_COPILOT_CPP

#include "Copilot.hpp"

namespace cnbi {
    namespace cvsa {

Copilot::Copilot(void){
	this->nrepetitions_ = 0;
	this->ncatch_ = 0;
}

Copilot::~Copilot(void){}

bool Copilot::Add(unsigned int id, unsigned int nrepetitions) {
	if(this->map_.insert(std::make_pair(id, nrepetitions)).second == false)
		return false;

	this->nrepetitions_ += nrepetitions;
	return true;
}

void Copilot::Generate(float ncatch) {

	// Store percentage of catch trials
	this->ncatch_ = ncatch;

	/*** Generate sequence with all given class ids ***/
	for(auto itc=this->map_.begin(); itc!=this->map_.end(); ++itc) {
		for(auto i=0; i<itc->second; i++) {
			this->sequence_trial_.push_back(itc->first);
		}
	}

	/*** Shuffle the order of the sequence ***/
	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(this->sequence_trial_.begin(), this->sequence_trial_.end(), g);

	this->sequence_it_ = this->Begin();
}

unsigned int Copilot::GetId(void) {
	return (*this->sequence_it_);
}

TrialsIt Copilot::GetIt(void) {
	return this->sequence_it_;
}

TrialsIt Copilot::Begin(void) {
	return this->sequence_trial_.begin();
}

TrialsIt Copilot::End(void) {
	return this->sequence_trial_.end();
}

bool Copilot::Next(void) {
	bool result = false;
	if(this->sequence_it_ != this->End()) {
		this->sequence_it_++;
		result = true;
	}
	return result;
}

bool Copilot::Previous(void) {
	bool result = false;
	if(this->sequence_it_ != this->Begin()) {
		this->sequence_it_--;
		result = true;
	}
	return result;
}

void Copilot::Dump(void) {
	printf("[cvsa::Copilot] - Number of trials: %u\n", this->nrepetitions_);
	printf("[cvsa::Copilot] - Number of classes: %lu\n", map_.size());

	auto i = 0;
	for(auto it=this->sequence_trial_.begin(); it!=this->sequence_trial_.end(); ++it) {
		printf("                     |- Trial %u - ClassId %u\n", i, (*it));
		i++;
	}
}

unsigned int Copilot::GetPosition(void) {
	return std::distance(this->sequence_trial_.begin(), this->sequence_it_);
}

unsigned int Copilot::GetSize(void) {
	return this->nrepetitions_;
}


	}
}
#endif
