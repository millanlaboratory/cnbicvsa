#ifndef CNBICVSA_TARGETGALLERY_CPP
#define CNBICVSA_TARGETGALLERY_CPP

#include "TargetGallery.hpp"

namespace cnbi {
    namespace cvsa {

TargetGallery::TargetGallery(float x, float y, float width, float height) : draw::Image(width, height) {

}


TargetGallery::~TargetGallery(void) {}


bool TargetGallery::SetFolder(const std::string& folder, const std::vector<std::string> exts) {


	this->WaitShape();

	DIR* dir = opendir(folder.c_str());

	if(!dir) {
		return false;
	}

	dirent* entry;
	while((entry = readdir(dir)) != nullptr) {
		for(auto it=exts.begin(); it!=exts.end(); ++it) {
			if(this->HasExtension(entry->d_name, *it)) {
				this->list.push_back(entry->d_name);
			}
		}
	}

	closedir(dir);
	
	std::sort(this->list.begin(), this->list.end());
	this->PostShape();

	return true;
}

bool TargetGallery::HasExtension(const std::string& name, const std::string& ext) {

	bool result = false;
	result = (name.size() >= ext.size()) && std::equal(ext.rbegin(), ext.rend(), name.rbegin());
	return result;

}


bool TargetGallery::Pick(unsigned int id) {
}
	}
}

#endif
