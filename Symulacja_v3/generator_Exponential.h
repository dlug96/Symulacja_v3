#pragma once
#include "stdafx.h"
#include "Header.h"

class generator_Exponential
{
private:
	double mean;
	generator_01 *giveRandom;
public:
	generator_Exponential(double seedValue, double meanNumber);

	double generate_Exponential();
};