#pragma once
#include "stdafx.h"
#include "Header.h"

class generator_Bernoulli
{
private:
	double probabilityOfSuccess;
	generator_01 *giveRandom;
public:
	generator_Bernoulli(double probability, double seedValue);

	int generate_Bernoulli();
};