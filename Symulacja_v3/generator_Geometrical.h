#pragma once
#include "stdafx.h"
#include "Header.h"

class generator_Geometrical {
private:
	generator_Bernoulli * giveRandom;
public:
	generator_Geometrical(double BernoulliProbabillity, double seedValue);

	int generate_Geometrical();
};