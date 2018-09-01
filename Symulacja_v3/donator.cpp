#include "stdafx.h"
#include <iostream>
#include "donator.h"
#include "process.h"
#include "event.h"
#include "bloodCentre.h"
#include "bloodSample.h"
#include "bloodStorage.h"
#include "generator_01.h"
#include "generator_Exponential.h"

donator::donator(bloodCentre *goHere, AgendaList *List, double *whereIsClock) :process(whereIsClock, List) {
	whereToGo = goHere;
}

void donator::Execute() {
	std::cout << "Przybyl nowy dawca" << std::endl;
	whereToGo->storage->listOfSamples->addToList(300.0);
	donator *nextDonator = new donator(whereToGo, Agenda, clock);
	nextDonator->Activate(whereToGo->generatorForDonators->generate_Exponential());
}