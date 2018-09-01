#include "stdafx.h"
#include <iostream>
#include "bloodSample.h"
#include "AgendaList.h"
#include "bloodCentre.h"
#include "process.h"
#include "event.h"
#include "bloodStorage.h"

bloodSample::bloodSample(double TTL, bloodStorage *whereIsStorage, double *clock_adr, AgendaList *List) :process(clock_adr, List) {
	timeToLive = TTL;
	myEvent->eventTime = TTL;
	next = nullptr;
	previous = nullptr;
	storage = whereIsStorage;
}

void bloodSample::removeFromList()
{
	if (next) {
		next->previous = previous;
		previous->next = next;
	}
	else previous->next = nullptr;

	next = nullptr;
	previous = nullptr;
	delete this;
}


void bloodSample::addToList(double TTL) 
{
	bloodSample *toAdd = new bloodSample(TTL, storage, storage->whereAmI->clock,storage->whereAmI->Agenda);	//Próbka do dodania
	toAdd->Activate(toAdd->myEvent->eventTime);									//Zaplanowanie utylizacji
	bloodSample *helper = this;

	while (helper->next) {
		if (helper->next->timeToLive > toAdd->timeToLive)break;
		else helper = helper->next;
	}
	if (helper->next) {
		toAdd->next = helper->next;
		helper->next->previous = toAdd;
		toAdd->previous = helper;
		helper->next = toAdd;
	}
	else {
		helper->next = toAdd;
		toAdd->previous = helper;
	}

	helper = nullptr;
	delete helper;
}

void bloodSample::Execute() {		//Utylizacja
	std::cout << "Utylizacja jednostki krwi" << std::endl;
	this->removeFromList();
}