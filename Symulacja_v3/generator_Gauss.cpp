#include "stdafx.h"
#include "generator_Gauss.h"
#include "generator_01.h"
#include <math.h>

generator_Gauss::generator_Gauss(double E, double var, double seedValue) {
	mean = E;
	variance = var;
	giveRandom = new generator_01(seedValue);

}

double generator_Gauss::generate_Gauss() {
	double r = giveRandom->newNumber();
	double x = (sqrt(variance*3.14 / 8.0)*log((1 + r) / (1 - r)) + mean);
	return (1 / (sqrt(variance * 2 * PI)))*exp(-(pow(x - mean, 2)) / (2 * variance));
}