#ifndef CNBICVSA_COPILOT_CPP
#define CNBICVSA_COPILOT_CPP

#include "Copilot.hpp"

namespace cnbi {
    namespace cvsa {

Copilot::Copilot(void){
	this->ntrials_ = 0;
}

Copilot::~Copilot(void){}

bool Copilot::Add(unsigned int id, unsigned int ntrials, float ncatch) {
	if(this->map_trial_.insert(std::make_pair(id, ntrials)).second == false)
		return false;

	if(this->map_catch_.insert(std::make_pair(id, ncatch)).second == false)
		return false;
		
	this->ntrials_ += ntrials;

	return true;
}

void Copilot::Generate(void) {

	std::vector<unsigned int> indexes(this->ntrials_);
	std::vector<unsigned int> trials;
	std::vector<unsigned int> catches;
	unsigned int cncatch;

	/*** Generate sequence with all given class ids ***/
	for(auto ittrial=this->map_trial_.begin(); ittrial!=this->map_trial_.end(); ++ittrial) {
		auto itcatch = this->map_catch_.find(ittrial->first);

		cncatch = std::ceil(((itcatch->second)*(ittrial->second))/100.0f);
		
		for(auto i=0; i<ittrial->second; i++) {
			trials.push_back(ittrial->first);
			catches.push_back((i < cncatch) ? 1 : 0);
		}
	}

	/*** Create an index vector ***/
	std::iota(indexes.begin(), indexes.end(), 0);

	/*** Shuffle the order of the index vector ***/
	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(indexes.begin(), indexes.end(), g);

	for(auto it=indexes.begin(); it!=indexes.end(); ++it) {
		this->sequence_trial_.push_back(trials.at((*it)));
		this->sequence_catch_.push_back(catches.at((*it)));
	}

	this->sequence_trial_it_ = this->Begin();
}

unsigned int Copilot::GetId(void) {
	return (*this->sequence_trial_it_);
}

TrialsIt Copilot::GetIt(void) {
	return this->sequence_trial_it_;
}

TrialsIt Copilot::Begin(void) {
	return this->sequence_trial_.begin();
}

TrialsIt Copilot::End(void) {
	return this->sequence_trial_.end();
}

bool Copilot::Next(void) {
	bool result = false;
	if(this->sequence_trial_it_ != this->End()) {
		this->sequence_trial_it_++;
		result = true;
	}
	return result;
}

bool Copilot::Previous(void) {
	bool result = false;
	if(this->sequence_trial_it_ != this->Begin()) {
		this->sequence_trial_it_--;
		result = true;
	}
	return result;
}

void Copilot::Dump(void) {
	printf("[cvsa::Copilot] - Number of trials: %u\n", this->ntrials_);
	printf("[cvsa::Copilot] - Number of classes: %lu\n", map_trial_.size());

	auto i = 0;
	for(auto it=this->sequence_trial_.begin(); it!=this->sequence_trial_.end(); ++it) {
		printf("                     |- Trial %u - ClassId %u - Catch %u\n", i+1, (*it), this->sequence_catch_.at(i));
		i++;
	}
}

unsigned int Copilot::GetPosition(void) {
	return std::distance(this->sequence_trial_.begin(), this->sequence_trial_it_);
}

unsigned int Copilot::GetSize(void) {
	return this->ntrials_;
}

bool Copilot::IsCatch(void) {
	return (this->sequence_catch_.at(this->GetPosition()) == 0) ? false : true;
}

unsigned int Copilot::GetCatchId(void) {
	
	unsigned int result;

	/*** Create random distribution ***/
	std::random_device rd;
	std::mt19937 g(rd());
	std::uniform_int_distribution<> dis(0, this->map_trial_.size()-1);

	do {
		result = dis(g);
	} while (result == this->GetId());

	return result;

}

	}
}
#endif
