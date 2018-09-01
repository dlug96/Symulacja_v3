#include "stdafx.h"
#include "generator_Geometrical.h"
#include "generator_Bernoulli.h"



generator_Geometrical::generator_Geometrical(double BernoulliProbabillity, double seedValue) {
	giveRandom = new generator_Bernoulli(BernoulliProbabillity, seedValue);
}

int generator_Geometrical::generate_Geometrical() {
	int x = 1;
	while (true)
	{
		int generated = giveRandom->generate_Bernoulli();
		if (generated == 0)x += 1;
		else break;
	}
	return x;
}