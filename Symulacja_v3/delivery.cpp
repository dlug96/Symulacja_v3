#include "stdafx.h"
#include <iostream>
#include "delivery.h"
#include "process.h"
#include "event.h"
#include "AgendaList.h"
#include "bloodCentre.h"
#include "bloodStorage.h"
#include "bloodSample.h"

delivery::delivery(bool deliveryType, bloodStorage * whereToDeliver, double * clock_adr, AgendaList * List) :process(clock_adr, List)
{
	typeOfDelivery = deliveryType;
	storage = whereToDeliver;
}

void delivery::Execute() {
	
	int howManyToAdd = 0;
	double TTL = 0;

	if (typeOfDelivery) {
		std::cout << "Przybyla dostawa awaryjna" << std::endl;
		storage->urgentFlag = false;
		howManyToAdd = 12;
		TTL = 500.0;
	}
	else {
		std::cout << "Przybyla dostawa normalna" << std::endl;
		storage->normalFlag = false;
		howManyToAdd = 25;
		TTL = 300.0;
	}

	for (int i = 0; i < howManyToAdd; i++) {
		storage->listOfSamples->addToList(TTL);
	}
	storage->howManySamples += howManyToAdd;
	std::cout << "Dodano " << howManyToAdd << " jednostek krwi" << std::endl;
}
