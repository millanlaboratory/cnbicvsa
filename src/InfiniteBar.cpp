#ifndef CNBICVSA_INFINITEBAR_CPP
#define CNBICVSA_INFINITEBAR_CPP

#include "InfiniteBar.hpp"

namespace cnbi {
	namespace cvsa {

InfiniteBar::InfiniteBar(float wbar, float hbar, unsigned int nobj, float wobj) {

    this->height_ = hbar-0.01;
    this->width_  = wbar;
    this->filled_ = 1;
    
    this->color_[0] = dtk_black[0];
    this->color_[1] = dtk_black[1];
    this->color_[2] = dtk_black[2];
    this->color_[3] = dtk_black[3];
    
    
    this->wobj_ = wobj;
    this->iobj_ = (float)this->width_/nobj - this->wobj_;
    this->hobj_  = this->height_ - 0.01; 
    this->nobj_	= nobj+1;
    
    this->bars_     = new dtk_hshape[this->nobj_];
    this->xbars_    = new float[this->nobj_];
    
    for(auto i = 0; i<this->nobj_; i++) {
    	this->bars_[i] = nullptr;
    	this->xbars_[i] = 0.0f;
    }
    
    this->bg_		= nullptr;
    this->lmask_	= nullptr;
    this->rmask_    = nullptr;
    this->cbars_    = nullptr;
    this->border_   = nullptr;
}

InfiniteBar::~InfiniteBar(void) {
    delete[] this->bars_;
    delete[] this->xbars_;
}

void InfiniteBar::Create(void) {


    this->WaitShape();
    
    
    for(auto i = 0; i < this->nobj_; i++) {
    	this->bars_[i] = dtk_create_rectangle_hw(nullptr, 
						this->orig_x_, this->orig_y_,
						this->wobj_, this->height_,
						this->filled_, 
						dtk_aluminium2_dark);
    	this->xbars_[i] = this->orig_x_;
    }
    
    
    dtk_hshape scene[] = {
    	this->border_ = dtk_create_rectangle_hw(this->border_, 
    						this->orig_x_, this->orig_y_,
    						this->width_,  this->height_+0.01,
    						this->filled_, dtk_white), 
    	this->bg_ = dtk_create_rectangle_hw(this->bg_, 
    					    this->orig_x_, this->orig_y_,
					    this->width_,  this->height_,
    					    this->filled_, this->color_), 
    	
    	this->cbars_ = dtk_create_composite_shape(this->cbars_, 
    						this->nobj_, 
						this->bars_, 1),
    
    	this->lmask_ = dtk_create_rectangle_hw(this->lmask_, 
    					    this->orig_x_, this->orig_y_,
    					    this->wobj_+this->iobj_, 
					    this->height_,
    					    this->filled_, dtk_black), 
    
    
    	this->rmask_ = dtk_create_rectangle_hw(this->rmask_, 
					    this->orig_x_, this->orig_y_,
    					    this->wobj_+this->iobj_, 
					    this->height_,
    					    this->filled_, dtk_black)
    };

    float xstart = -this->width_/2.0f;
    for(auto i = 0; i < this->nobj_; i++) {
    	dtk_move_shape(this->bars_[i], xstart , 0.0f);
    	this->xbars_[i] = xstart;
    	xstart = xstart + (this->wobj_+this->iobj_);
    }
    
    dtk_move_shape(this->rmask_,  (this->width_+this->wobj_+this->iobj_)/2.0f, 0.0f);
    dtk_move_shape(this->lmask_, -(this->width_+this->wobj_+this->iobj_)/2.0f, 0.0f);
    
    this->shp_ptr_ = dtk_create_composite_shape(this->shp_ptr_, 
						sizeof(scene)/sizeof(scene[0]),
						scene, 1);
    this->PostShape();
}

void InfiniteBar::Update(float dx) {

    float back;
    this->WaitShape();
    
    for(auto i = 0; i<this->nobj_; i++) {
    	dtk_relmove_shape(this->bars_[i], dx, 0.0f);
    	this->xbars_[i] = this->xbars_[i] + dx;
    
	if(dx > 0.0f) {
	   if(this->xbars_[i] >= (this->width_/2.0f + this->wobj_/2.0f)) {
	      back = (this->nobj_)*(this->wobj_+this->iobj_);
    	      this->xbars_[i] = this->xbars_[i] - back;
    	      dtk_relmove_shape(this->bars_[i], - back, 0.0f);
    	   }
	} else if(dx < 0.0f) {
	   if(this->xbars_[i] <= -(this->width_/2.0f + this->wobj_/2.0f)) {
	      back = (this->nobj_)*(this->wobj_+this->iobj_);
    	      this->xbars_[i] = this->xbars_[i] + back;
    	      dtk_relmove_shape(this->bars_[i], + back, 0.0f);
    	   }
	}

    
    }
    
    this->PostShape();
}

	}
}

#endif
