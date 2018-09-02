#pragma once
#include "stdafx.h"
#include "Header.h"
#include "process.h"

class donator :public process
{
public:
	bloodCentre *whereToGo;
	bool bloodGroup;

	void Execute();

	donator(bloodCentre *goHere, AgendaList *List, double *whereIsClock);
};