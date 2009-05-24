/*
 * graphicalEqualiser.h
 *
 *  Created on: May 19, 2009
 *      Author: Darren Golbourn
 */

#ifndef GRAPHICALEQUALISER_H_
#define GRAPHICALEQUALISER_H_
#include "gsl/gsl_spline.h"

class graphicalEqualiser {
	gsl_spline *function;
	gsl_interp_accel *accel;
public:
	graphicalEqualiser( const double xa[], const double ya[], size_t size );
	virtual ~graphicalEqualiser();
	double get(double x);

	static double eq(double freq, void* e);
};

#endif /* GRAPHICALEQUALISER_H_ */
