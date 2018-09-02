#pragma once
#include "stdafx.h"
#include "Header.h"
#include "process.h"

class delivery :public process
{
public:
	bool typeOfDelivery;	//true - urgent, false - normal
	bloodStorage *storage;
	bool bloodGroup;

	void Execute();

	delivery(bool deliveryType, bool groupOfBlood, bloodStorage *whereToDeliver, double *clock_adr, AgendaList *List);
};