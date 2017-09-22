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

	printf("list size: %lu\n", list.size());
	/*** Replicate the list to cover all the repetitions ***/
	printf("%f\n", std::ceil((float)this->nrepetitions_/(float)list.size()));
	list = this->replicate(list, std::ceil((float)this->nrepetitions_/(float)list.size()));
	printf("list size: %lu\n", list.size());

	/*** Shuffle the order of the list ***/
	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(list.begin(), list.end(), g);
	printf("list size: %lu\n", list.size());

	/*** Truncate the list to match the number of repetitions required ***/
	list.erase(list.begin() + this->nrepetitions_, list.end());
	printf("list size: %lu\n", list.size());

	/*** Store the list in the internal object list ***/
	this->list_ = list;


	// TO BE DELETED - ONLY TEST
	unsigned int count = 0;
	for(auto it1=this->list_.begin(); it1!=this->list_.end(); ++it1) {
		count++;
		printf("[%u]", count);
		for(auto it2 = (*it1).begin(); it2 != (*it1).end(); ++it2) {
			printf(" %u", (*it2));
		}
		printf("\n");
	}
	printf("%lu combinations\n", this->list_.size());
	
}

TargetControl::~TargetControl(void) {}

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
