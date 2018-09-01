#include "stdafx.h"
#include "generator_Bernoulli.h"
#include "generator_01.h"

generator_Bernoulli::generator_Bernoulli(double probability, double seedValue) {
	probabilityOfSuccess = probability;
	giveRandom = new generator_01(seedValue);
}

int generator_Bernoulli::generate_Bernoulli() {
	double x = giveRandom->newNumber();
	if (x <= probabilityOfSuccess)return 1;
	else return 0;
}