/*
 * history.h
 *
 *  Created on: Apr 11, 2009
 *      Author: Darren Golbourn
 *
 * Description:
 * This class defines a cache to record data of class <type>.  The data is placed at position index
 * in the data array and the index is then incremented modulo the array size. In this way, the array
 * 'loops back' on itself.
 *
 * Usage:
 * Initialize the cache: history<class> name(size)
 * Add a new element, replacing the current oldest: name.record(new_element)
 * Return the most recent element: class data = name.current()
 */

#ifndef HISTORY_H_
#define HISTORY_H_

#include<math.h>

template <class type> class history {
	unsigned long int index; // current array index
	unsigned long int size; // size of history array
	type *data;				//history array
	type var;				//variance
	type sq_total;			//sum of squares

	type temp_s;			//  1/size
	type temp_u;			//  1/(size-1)
public:
	type total;				// current total
	type sigma;				// sample standard deviation
	type average;			// mean

	history(unsigned long int);
	virtual ~history();

	void record(type);		// record a new element to the history, displacing the oldest.
	type current();			// get the most recent new element.
};

template <class type> history<type>::history(unsigned long int s) {
	size = s;
	temp_s = 1/(type)(s);
	temp_u = 1/(type)(s-1);

	data = new type[s];
	for(unsigned long int i = 0; i<size; i++) data[i] = 0;
	total = 0;
	sq_total = 0;
	index = 0;
	var = 0;
	sigma = 0;
	average = 0;
}

template <class type> history<type>::~history() {
	delete[] data;
}

/*
 * Record:  take input, calculate features, update history.
 */
template <class type> void history<type>::record(type input) {
	total = total + input - data[index]; // sum
	sq_total = sq_total + input*input - data[index]*data[index]; //sum of squares
	average = total * temp_s; // mean

	var = temp_u * (sq_total - temp_s*total*total); // variance
	sigma = sqrt(var); // sample standard deviation

	data[index]= input; // record new data
	index = (index + 1)%size; // update cyclic array index.
}

// get most recent new element.
template <class type> type history<type>::current() {
	return data[index];
}

#endif /* HISTORY_H_ */
