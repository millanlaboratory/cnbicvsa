#ifndef CNBICVSA_INFINITEBAR_HPP
#define CNBICVSA_INFINITEBAR_HPP

#include <cmath>
#include <cnbidraw/Shape.hpp>

namespace cnbi {
	namespace cvsa {

class InfiniteBar : public draw::Shape {

	public:
		InfiniteBar(float width, float height, unsigned int nobj, float wobj);
		virtual ~InfiniteBar(void);

		void Create(void);
		void Update(float dx);

	private:

		unsigned int	nobj_;
		float			hobj_;
		float			wobj_;
		float			iobj_;

		unsigned int	nbars_;
		float			bheight_;
		dtk_hshape*		bars_;
		float*			xbars_;
		dtk_hshape		border_;
		dtk_hshape		bg_;
		dtk_hshape		cbars_;
		dtk_hshape		lmask_;
		dtk_hshape		rmask_;


};


	}
}

#endif

