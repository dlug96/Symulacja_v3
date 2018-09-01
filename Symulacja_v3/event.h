#pragma once
#include "stdafx.h"
#include "Header.h"

class event
{
public:
	double eventTime;
	process *proc;
	AgendaList *myAgenda;
	
	event(process *ptr);

	void unscheduleMe();
};