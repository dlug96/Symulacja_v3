#pragma once
#include "stdafx.h"
#include "Header.h"
#include "process.h"

class patient :public process
{
public:
	int bloodNeeded;
	bloodCentre *whereToGo;

	void Execute();

	patient(int howMany, bloodCentre *goHere, AgendaList *List, double *whereIsClock);
	
};