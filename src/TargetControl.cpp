#ifndef CNBICVSA_TARGETCONTROL_CPP
#define CNBICVSA_TARGETCONTROL_CPP

#include "TargetControl.hpp"


namespace cnbi {
    namespace cvsa {

TargetControl::TargetControl(unsigned int ntargets, unsigned int nobjects, unsigned int nrepetitions) {

	this->ntargets_ = ntargets;
	this->nobjects_ = nobjects;
	this->nrepetitions_ = nrepetitions;


	/*** Generate all possible combinations given ntargets, nobjects ***/
	List list;
	list = this->nchoosek(this->nobjects_, this->ntargets_);

	/*** Replicate the list to cover all the repetitions ***/
	list = this->replicate(list, std::ceil((float)this->nrepetitions_/(float)list.size()));

	/*** Shuffle the order of the list ***/
	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(list.begin(), list.end(), g);

	/*** Truncate the list to match the number of repetitions required ***/
	list.erase(list.begin() + this->nrepetitions_, list.end());

	/*** Store the list in the internal object list ***/
	this->list_ = list;

	this->iter_ = this->Begin();

}

TargetControl::~TargetControl(void) {}

unsigned int TargetControl::Size(void) {
	return this->list_.size();
}

ListIt TargetControl::Begin(void) {
	return this->list_.begin();
}

ListIt TargetControl::End(void) {
	return this->list_.end();
}

bool TargetControl::Next(void) {
	bool result = false;
	if(this->iter_ != this->End()) {
		this->iter_++;
		result = true;
	}
	return result;
}

bool TargetControl::Previous(void) {
	bool result = false;
	if(this->iter_ != this->Begin()) {
		this->iter_--;
		result = true;
	}
	return result;
}

unsigned int TargetControl::At(unsigned int pos) {
	return (*this->iter_).at(pos);
}

List TargetControl::replicate(List src, unsigned int n) {
	
	List dst;
	for (auto i = 0; i<n; i++)
		dst.insert(dst.end(), src.begin(), src.end());

	return dst;
}

List TargetControl::nchoosek(unsigned int n, unsigned int k) {

	std::string bitmask(k, 1);
	bitmask.resize(n, 0);
	Entry entry;
	List  list;

	do {
		for(auto i=0; i<n; i++) {
			if(bitmask[i]) {
				entry.push_back(i);
			}
		}
		list.push_back(entry);
		entry.clear();
	} while(std::prev_permutation(bitmask.begin(), bitmask.end()));

	return list;
}

	}
}

#endif
