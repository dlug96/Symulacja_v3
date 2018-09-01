#include "stdafx.h"
#include <cmath>
#include "generator_01.h"
#include "generator_Exponential.h"

generator_Exponential::generator_Exponential(double seedValue, double meanNumber) {
	mean = meanNumber;
	giveRandom = new generator_01(seedValue);
}

double generator_Exponential::generate_Exponential() {
	double r = giveRandom->newNumber();
	double x = (-mean)*log(r);
	return x;

}