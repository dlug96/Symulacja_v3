#pragma once
#include "stdafx.h"
#include "Header.h"

#define PI 3.14159265358979323846

class generator_Gauss
{
private:
	double mean;
	double variance;
	generator_01 *giveRandom;

public:
	generator_Gauss(double E, double var, double seedValue);
	double generate_Gauss();

};