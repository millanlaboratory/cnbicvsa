#ifndef CNBICVSA_TARGET_HPP
#define CNBICVSA_TARGET_HPP

#include <cnbidraw/Gallery.hpp>
#include <cnbidraw/Rectangle.hpp>
#include "Flags.hpp"

namespace cnbi {
	namespace cvsa {

class Target : public draw::Gallery {

	public:
		Target(float width, float height, float thick, const float* color);
		virtual ~Target(void);

		void SetPosition(float angle, float radius);
		void SetTime(float time);
		bool ToCenter(void);

		void Present(unsigned int id);
		void Hit(void);
		void Reset(void);


	private:
		bool		ismoving_;
		CcTimeValue timer_;
		float		radius_;
		float		angle_;
		float		time_;
		float		setx_;
		float		sety_;

};

	}
}

#endif

