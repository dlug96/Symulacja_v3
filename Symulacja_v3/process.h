#pragma once
#include "stdafx.h"
#include "Header.h"

class process
{
public:
	event *myEvent;

	virtual void Execute() = 0;
	void Activate(double time);
	double *clock;
	AgendaList *Agenda;

	process(double *whereIsClock, AgendaList *List);
};