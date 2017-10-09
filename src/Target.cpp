#ifndef CNBICVSA_TARGET_CPP
#define CNBICVSA_TARGET_CPP

#include "Target.hpp"


namespace cnbi {
    namespace cvsa {

Target::Target(const std::string& folder, const std::vector<std::string> ext) {
	this->folder_	  = folder;
	this->extensions_ = ext;
	this->time_ = 1000.0f;	
	this->ismoving_ = false;
	this->ntargets_ = 0;
}

Target::~Target(void) {}


draw::Gallery* Target::Add(float width, float height, float angle, float radius) {

	Position position;
	std::shared_ptr<draw::Gallery>  itarget(new draw::Gallery(width, height));

	if(itarget->SetFolder(this->folder_, this->extensions_) == false) {
		printf("[Target] - Cannot access to folder: '%s'\n", this->folder_.c_str());
		return nullptr;
	}

	this->thick_ = width/10.0f;
	this->nobjects_ = itarget->Size();
	this->ntargets_++;
	
	position = this->tocartesian(angle, radius);	
	this->positions_.push_back(position);
	
	itarget->Move(position.at(0), position.at(1));
	this->list_.push_back(std::move(itarget));
	
	return this->list_.back().get();
}

void Target::Generate(unsigned int nrepetitions) {
	this->nrepetitions_ = nrepetitions;

	/*** Generate all possible combinations given ntargets, nobjects ***/
	Sequence seq;
	seq = this->nchoosek(this->nobjects_, this->ntargets_);

	/*** Replicate the sequence to cover all the repetitions ***/
	seq = this->replicate(seq, std::ceil((float)this->nrepetitions_/(float)seq.size()));

	/*** Shuffle the order of the sequence ***/
	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(seq.begin(), seq.end(), g);

	/*** Truncate the sequence to match the number of repetitions required ***/
	seq.erase(seq.begin() + this->nrepetitions_, seq.end());

	/*** Store the sequence in the internal object sequence ***/
	this->sequence_ = seq;

	this->sequence_it_ = this->Begin();

	printf("nrepetions: %u\n", nrepetitions);
	printf("nobjects: %u\n", this->nobjects_);
	printf("ntargets: %u\n", this->ntargets_);
	auto i=0;
	for(auto its=this->sequence_.begin(); its!=this->sequence_.end(); ++its) {
		printf("[%d]", i);
		for(auto ite=(*its).begin(); ite!=(*its).end(); ++ite) {
			printf(" %u", (*ite));	
		}
		printf("\n");
		i++;
	}

}

void Target::SetTime(float time) {
	this->time_ = time;
}

float Target::WaitRandom(float maxtime, float mintime) {
	
	float rndn;
	std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(mintime, maxtime);
	rndn = dis(gen);
	return rndn;
}

void Target::Show(void) {

	unsigned int id = 0;
	for(auto it=this->list_.begin(); it!=this->list_.end(); ++it) {
		(*it)->Select((*this->sequence_it_).at(id));
		(*it)->Show(draw::Shape::Fill);
		(*it)->Hide(draw::Shape::Stroke);
		id++;
	}
}

void Target::Hide(void) {
	for(auto it=this->list_.begin(); it!=this->list_.end(); ++it) {
		(*it)->Hide();
	}
}

void Target::Hit(unsigned int id, const float* color) {
	this->list_.at(id)->SetStroke(this->thick_, color);
	this->list_.at(id)->Show(draw::Shape::Stroke);
}

void Target::Reset(void) {

	Position position;
	unsigned int id = 0;
	for(auto it=this->list_.begin(); it!=this->list_.end(); ++it) {
		position = this->positions_.at(id);
		(*it)->Move(position.at(0), position.at(1));
		(*it)->Show(draw::Shape::Fill);
		(*it)->Hide(draw::Shape::Stroke);
		id++;
	}
}

bool Target::ToCenter(unsigned int id) {

	bool result = false;
	float cx, cy, cangle, cradius, oradius;
	Position position, oposition;
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
	this->list_.at(id)->GetPosition(&cx, &cy);
	cangle  = atan2(cy, cx);
	cradius = std::sqrt((std::pow(cx, 2))+(std::pow(cy, 2)));

	// Compute the current movement ds (in polar coordinates) and convert it in
	// cartesian coordiantes
	oposition = this->positions_.at(id);
	oradius = std::sqrt((std::pow(oposition.at(0), 2))+(std::pow(oposition.at(1), 2)));
	ds = ((dt/1000.0f)*oradius)/(this->time_/1000.0f);

	// Move the shape to the new coordinates
	position.at(0) = (cradius-ds)*cos(cangle);
	position.at(1) = (cradius-ds)*sin(cangle);
	this->list_.at(id)->Move(position.at(0), position.at(1));

	// Compute the current radius (with respect to the center)
	cradius = std::sqrt((std::pow(position.at(0), 2))+(std::pow(position.at(1), 2)));

	// Check if the current radius is close to the center
	if(std::fabs(cradius) <= 0.01f) {
		this->ismoving_ = false;
		result = true;
	}

	return result;

}

SequenceIt Target::Begin(void) {
	return this->sequence_.begin();
}

SequenceIt Target::End(void) {
	return this->sequence_.end();
}

bool Target::Next(void) {
	bool result = false;
	if(this->sequence_it_ != this->End()) {
		this->sequence_it_++;
		result = true;
	}
	return result;
}

bool Target::Previous(void) {
	bool result = false;
	if(this->sequence_it_ != this->Begin()) {
		this->sequence_it_--;
		result = true;
	}
	return result;
}

//unsigned int Target::At(unsigned int pos) {
//	return (*this->sequence_it_).at(pos);
//}

Sequence Target::replicate(Sequence src, unsigned int n) {
	
	Sequence dst;
	for (auto i = 0; i<n; i++)
		dst.insert(dst.end(), src.begin(), src.end());

	return dst;
}

Sequence Target::nchoosek(unsigned int n, unsigned int k) {

	std::string bitmask(k, 1);
	bitmask.resize(n, 0);
	Entry entry;
	Sequence  seq;

	do {
		for(auto i=0; i<n; i++) {
			if(bitmask[i]) {
				entry.push_back(i);
			}
		}
		seq.push_back(entry);
		entry.clear();
	} while(std::prev_permutation(bitmask.begin(), bitmask.end()));

	return seq;
}

Position Target::tocartesian(float angle, float radius) {

	std::array<float, 2> position;
	position.at(0) = radius*cos(angle*M_PI/180.0f);
	position.at(1) = radius*sin(angle*M_PI/180.0f);

	return position;
}

	}
}

#endif
