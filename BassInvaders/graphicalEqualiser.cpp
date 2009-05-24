/*
 * graphicalEqualiser.cpp
 *
 *  Created on: May 19, 2009
 *      Author: Darren Golbourn
 */

#include "graphicalEqualiser.h"

graphicalEqualiser::graphicalEqualiser( const double xa[], const double ya[], size_t size ){
	accel = gsl_interp_accel_alloc();
	function = gsl_spline_alloc(gsl_interp_cspline, size);
	gsl_spline_init(function, xa, ya, size);
}

graphicalEqualiser::~graphicalEqualiser() {
	gsl_spline_free(function);
	gsl_interp_accel_free(accel);
}

double graphicalEqualiser::get(double x){
	return gsl_spline_eval(function, x, accel);
}

double graphicalEqualiser::eq(double freq, void* e)
{
	return ((graphicalEqualiser*)e)->get(freq);
}
