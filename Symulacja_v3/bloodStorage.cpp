#include "stdafx.h"
#include "bloodStorage.h"
#include "bloodSample.h"
#include "bloodCentre.h"
#include "AgendaList.h"

bloodStorage::bloodStorage(bloodCentre *hereYouAre) {
	whereAmI = hereYouAre;
	urgentFlag = false;
	normalFlag = false;
	howManySamples = 0;
	listOfSamples = new bloodSample(-1.0, this, whereAmI->clock, whereAmI->Agenda);
}

int bloodStorage::countSamples() {
	bloodSample *helper = nullptr;
	int counter = 0;
	if (listOfSamples->next == nullptr)return 0;
	else {
		helper = listOfSamples->next;
		counter = 1;
	}

	while (helper->next) {
		counter += 1;
		helper = helper->next;
	}

	return counter;
}