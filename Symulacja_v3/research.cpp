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

research::research(bloodCentre *goHere, AgendaList *List, double *whereIsClock) :process(whereIsClock, List)
{
	whereToGo = goHere;
}

void research::Execute() {
	int samplesForResearch = whereToGo->generatorForResearch->newNumber();
	for (int i = 0; i < samplesForResearch; i++) {
		whereToGo->storage->listOfSamples->next->myEvent->unscheduleMe();
		whereToGo->storage->listOfSamples->next->removeFromList();
	}
	whereToGo->researches += 1;
	whereToGo->researchFlag = nullptr;
	cout << "Pobrano " << samplesForResearch << " probek krwi na badania" << endl;
}