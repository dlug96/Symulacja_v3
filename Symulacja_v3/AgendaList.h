#pragma once
#include "stdafx.h"
#include "Header.h"

class AgendaList
{
public:
	event *eventStored;
	AgendaList *next;
	AgendaList *previous;

	void schedule(event *toSchedule);

	void unschedule(event *toUnschedule);

	void showAgenda();

	event* returnfirst();

	void removeFirst();

	void removeThis(event *toRemove);

	int checkIfOK();

	AgendaList(event *toStore);
};