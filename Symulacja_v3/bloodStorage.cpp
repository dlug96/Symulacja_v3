#include "stdafx.h"
#include "bloodStorage.h"
#include "bloodSample.h"
#include "bloodCentre.h"
#include "AgendaList.h"

bloodStorage::bloodStorage(bloodCentre *hereYouAre) {
	whereAmI = hereYouAre;
	urgentFlagA = false;
	normalFlagA = false;
	urgentFlagB = false;
	normalFlagB = false;
	howManySamples = 0;
	listOfSamplesGroupA = new bloodSample(-1.0, true, this, whereAmI->clock, whereAmI->Agenda);
	listOfSamplesGroupB = new bloodSample(-1.0, false, this, whereAmI->clock, whereAmI->Agenda);
}

int bloodStorage::countSamples(bool groupOfBlood) {
	bloodSample *helper = nullptr;
	bloodSample *list;
	if (groupOfBlood)list = listOfSamplesGroupA;
	else list = listOfSamplesGroupB;
	int counter = 0;
	if (list->next == nullptr)return 0;
	else {
		helper = list->next;
		counter = 1;
	}

	while (helper->next) {
		counter += 1;
		helper = helper->next;
	}

	list = nullptr;
	delete list;
	helper = nullptr;
	delete helper;

	return counter;
}