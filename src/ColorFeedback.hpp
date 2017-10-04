#ifndef CNBICVSA_COLORFEEDBACK_HPP
#define CNBICVSA_COLORFEEDBACK_HPP

#include <array>
#include <cnbidraw/Shape.hpp>
#include <cnbidraw/Ring.hpp>
#include <cnbidraw/Circle.hpp>
#include "Flags.hpp"

namespace cnbi {
    namespace cvsa {

float default_color_strk[4] = {169.0f/256.0f, 169.0f/256.0f, 169.0f/256.0f, 1.0f};
float default_color_fill[4] = {57.0f/256.0f, 57.0f/256.0f, 57.0f/256.0f, 1.0f};
float default_color_fdbk[4] = {18.0f/256.0f, 86.0f/256.0f, 0.0f, 0.0f};
float default_color_hit[4]  = {31.0f/256.0f, 140.0f/256.0f, 0.0f, 0.0f};
float default_color_miss[4] = {140.0f/256.0f, 0.0f, 10.0f/256.0f, 0.0f};

class ColorFeedback : public draw::Shape {

	public:
		ColorFeedback(float radius = 0.18f, float thick = 0.03f);
		virtual ~ColorFeedback(void);

		void  SetColorRing(float* stroke, float* fill, float* feeback);
		void  SetColorBoom(float* hit, float* miss);

		bool  Update(float value);
		void  Reset(void);
		void  SetDiscrete(unsigned int mode);
		float GetValue(void);
		void  SetThreshold(float th);

	protected:
		void  CreateFill(void);
		void  CreateStroke(void);

	public:
		const static unsigned int AsHit    = 1;
		const static unsigned int AsMiss = 2;

	private:
		draw::Ring*		ring_strk_;
		draw::Circle*	ring_fill_;
		draw::Circle*	ring_fdbk_;

		float					ring_radius_;
		float					ring_thick_;
		std::array<float, 4>	ring_color_hit_;
		std::array<float, 4>	ring_color_miss_;
		std::array<float, 4>	ring_color_strk_;
		std::array<float, 4>	ring_color_fill_;
		std::array<float, 4>	ring_color_fdbk_;

		float	value_;
		float	threshold_;
};

	}
}

#endif


