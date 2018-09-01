#pragma once
#include "stdafx.h"
#include "Header.h"
#include "process.h"

class delivery :public process
{
public:
	bool typeOfDelivery;	//true - urgent, false - normal
	bloodStorage *storage;

	void Execute();

	delivery(bool deliveryType, bloodStorage *whereToDeliver, double *clock_adr, AgendaList *List);
};