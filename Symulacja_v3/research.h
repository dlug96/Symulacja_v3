#pragma once
#include "stdafx.h"
#include "Header.h"
#include "process.h"

class research :public process
{
public:
	bloodCentre * whereToGo;
	bool bloodGroup;

	void Execute();

	research(bloodCentre *goHere, bool groupOfBlood,AgendaList *List, double *whereIsClock);
};