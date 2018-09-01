#include "stdafx.h"
#include "generator_01.h"
#include <math.h>

generator_01::generator_01(double lastValue) {
	a = 16807.0;
	m = pow(2, 31) - 1;
	X = lastValue;
	q = 127773.0;
	r = 2836.0;
	h = X / q;
}

double generator_01::newNumber() {
	/*int f = X - q * h;
	int rh = r * h;
	int af = a * f;
	int afrh = af - rh;
	X = afrh;*/
	X = a * (X - q * h) - r * h;
	if (X < 0)X += m;
	h = X / q;
	return (X*1.0) / (m*1.0);

}