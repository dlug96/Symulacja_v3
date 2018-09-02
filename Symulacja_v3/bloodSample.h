#pragma once
#include "stdafx.h"
#include "Header.h"
#include "process.h"

class bloodSample :public process
{
public:
	double timeToLive;
	bloodSample *next;
	bloodSample *previous;
	bloodStorage *storage;
	bool bloodGroup;

	void Execute();

	void addToList(double TTL, bool groupOfBlood);

	void removeFromList();

	bloodSample(double TTL, bool groupofBlood,bloodStorage *whereIsStorage, double *clock_adr, AgendaList *List);

};