#include "stdafx.h"
#include <iostream>
#include "delivery.h"
#include "process.h"
#include "event.h"
#include "AgendaList.h"
#include "bloodCentre.h"
#include "bloodStorage.h"
#include "bloodSample.h"

delivery::delivery(bool deliveryType, bool groupOfBlood, bloodStorage * whereToDeliver, double * clock_adr, AgendaList * List) :process(clock_adr, List)
{
	typeOfDelivery = deliveryType;
	storage = whereToDeliver;
	bloodGroup = groupOfBlood;
}

void delivery::Execute() {
	
	int howManyToAdd = 0;
	double TTL = 0;

	if (typeOfDelivery) {
		std::cout << "Przybyla dostawa awaryjna" << std::endl;
		if(bloodGroup)storage->urgentFlagA = false;
		else storage->urgentFlagB = false;
		howManyToAdd = 60;
		TTL = 500.0;
	}
	else {
		std::cout << "Przybyla dostawa normalna" << std::endl;
		if (bloodGroup)storage->normalFlagA = false;
		else storage->normalFlagB = false;
		howManyToAdd = 25;
		TTL = 300.0;
	}

	for (int i = 0; i < howManyToAdd; i++) {
		if(bloodGroup)storage->listOfSamplesGroupA->addToList(TTL, true);
		else storage->listOfSamplesGroupB->addToList(TTL, false);
	}

	std::cout << "Dodano " << howManyToAdd << " jednostek krwi";
	if (bloodGroup)std::cout << " grupy A" << std::endl;
	else std::cout << " grupy B" << std::endl;
}
