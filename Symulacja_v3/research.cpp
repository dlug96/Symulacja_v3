#include "stdafx.h"
#include <iostream>
#include "research.h"
#include "process.h"
#include "event.h"
#include "AgendaList.h"
#include "bloodCentre.h"
#include "bloodStorage.h"
#include "bloodSample.h"
#include "generator_01.h"
#include "generator_5_10.h"

using namespace std;

research::research(bloodCentre *goHere, bool groupOfBlood, AgendaList *List, double *whereIsClock) :process(whereIsClock, List)
{
	whereToGo = goHere;
	bloodGroup = groupOfBlood;
}

void research::Execute() {
	int samplesForResearch = whereToGo->generatorForResearch->newNumber();
	if (bloodGroup) {
		for (int i = 0; i < samplesForResearch; i++) {
			whereToGo->storage->listOfSamplesGroupA->next->myEvent->unscheduleMe();
			whereToGo->storage->listOfSamplesGroupA->next->removeFromList();
		}
		cout << "Pobrano " << samplesForResearch << " probek krwi grupy A na badania" << endl;
	}
	else {
		for (int i = 0; i < samplesForResearch; i++) {
			whereToGo->storage->listOfSamplesGroupB->next->myEvent->unscheduleMe();
			whereToGo->storage->listOfSamplesGroupB->next->removeFromList();
		}
		cout << "Pobrano " << samplesForResearch << " probek krwi grupy B na badania" << endl;
	}
	whereToGo->researches += 1;
	if(bloodGroup)whereToGo->researchFlagA = nullptr;
	else whereToGo->researchFlagB = nullptr;
	
}