/*
 * audefnmacros.h
 *
 *  Created on: May 2, 2009
 *      Author: Darren Golbourn
 */

#ifndef AUDIO_STREAM_FMT_MACROS
#define AUDIO_STREAM_FMT_MACROS
// Macros for real/imaginary right/left parts of data stream
	#define REAL(z,i) ((z)[2*(i)])
	#define IMAG(z,i) ((z)[2*(i)+1])
	#define RIGHT(z,i) ((z)[2*(i)])
	#define LEFT(z,i) ((z)[2*(i)+1])

// Macros for positive/negative frequencies in frequency domain data
	#define POSITIVE(i,size) (i)
	#define NEGATIVE(i,size) ((i)>0?((size)-(i)):i)
#endif /* AUDEFNMACROS_H_ */
