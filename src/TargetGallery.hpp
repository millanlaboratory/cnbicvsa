#ifndef CNBICVSA_TARGETGALLERY_HPP
#define CNBICVSA_TARGETGALLERY_HPP

#include <string>
#include <memory>
#include <vector>
#include <algorithm>
#include <dirent.h>
#include <cnbidraw/Image.hpp>
#include "Flags.hpp"

namespace cnbi {
    namespace cvsa {

class TargetGallery : public draw::Image {

	public: 
		TargetGallery(float x = CNBICVSA_TARGETGALLERY_X, 
					  float y = CNBICVSA_TARGETGALLERY_Y, 
					  float width = CNBICVSA_TARGETGALLERY_WIDTH, 
					  float height = CNBICVSA_TARGETGALLERY_HEIGHT);
		virtual ~TargetGallery(void);

		bool SetFolder(const std::string& folder, const std::vector<std::string> exts);
		bool Pick(unsigned int id = -1);

		bool HasExtension(const std::string& name, const std::string& ext);

	public:
		std::vector<std::string>	list;

	private:
		unsigned int	current_id_;
		std::string		folder_;

};

	}
}

#endif

