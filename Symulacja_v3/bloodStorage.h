#pragma once
#include "stdafx.h"
#include "Header.h"

class bloodStorage
{
public:
	bloodSample * listOfSamples;
	int howManySamples;

	bloodCentre *whereAmI;

	bool urgentFlag;
	bool normalFlag;

	bloodStorage(bloodCentre *hereYouAre);
	int countSamples();
};

