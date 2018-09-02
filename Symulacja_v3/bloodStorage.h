#pragma once
#include "stdafx.h"
#include "Header.h"

class bloodStorage
{
public:
	bloodSample * listOfSamplesGroupA;
	bloodSample * listOfSamplesGroupB;
	int howManySamples;

	bloodCentre *whereAmI;

	bool urgentFlagA;
	bool normalFlagA;
	bool urgentFlagB;
	bool normalFlagB;

	bloodStorage(bloodCentre *hereYouAre);
	int countSamples(bool groupOfBlood);
};

