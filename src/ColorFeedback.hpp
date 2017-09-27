#ifndef CNBICVSA_COLORFEEDBACK_HPP
#define CNBICVSA_COLORFEEDBACK_HPP

#include <array>
#include <cnbidraw/Shape.hpp>
#include <cnbidraw/Ring.hpp>
#include <cnbidraw/Circle.hpp>
#include "Flags.hpp"

namespace cnbi {
    namespace cvsa {

class ColorFeedback : public draw::Shape {

	public:
		ColorFeedback(void);
		virtual ~ColorFeedback(void);

		bool  Update(float value);
		void  Reset(void);
		void  SetDiscrete(unsigned int mode);
		float GetValue(void);
		void  SetThreshold(float th);

	protected:
		void  CreateFill(void);
		void  CreateStroke(void);

	public:
		const static unsigned int AsBoom    = 1;
		const static unsigned int AsTimeout = 2;

	private:
		draw::Ring*		ring_;
		draw::Circle*	bg_;
		draw::Circle*	feedback_;

		float	value_;
		float	threshold_;
		std::array<float, 4>	boom_color_;
		std::array<float, 4>	timeout_color_;

		float					ring_radius_;
		float					ring_thick_;
		std::array<float, 4>	ring_color_;
		float					bg_radius_;
		std::array<float, 4>	bg_color_;
		float					feedback_radius_;
		std::array<float, 4>	feedback_color_;
};

	}
}

#endif


