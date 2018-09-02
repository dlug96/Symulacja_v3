#include "stdafx.h"
#include <iostream>
#include "patient.h"
#include "process.h"
#include "bloodCentre.h"
#include "bloodSample.h"
#include "bloodStorage.h"
#include "generator_01.h"
#include "generator_Exponential.h"
#include "generator_Bernoulli.h"
#include "generator_Geometrical.h"
#include <queue>

using namespace std;

patient::patient(int howMany, bloodCentre * goHere, AgendaList * List, double *whereIsClock) :process(whereIsClock,List)
{
	bloodNeeded = howMany;
	whereToGo = goHere;
	if (whereToGo->generatorForBloodGroupPatients->generate_Bernoulli() == 1)bloodGroup = true;
	else bloodGroup = false;
}

void patient::Execute()
{
	std::cout << "Przybyl nowy pacjent" << std::endl;
	whereToGo->queueOfPatients.push(this);
	whereToGo->patients += 1;
	patient *nextPatient = new patient(whereToGo->generatorForBloodSamplesNeeded->generate_Geometrical(), whereToGo, Agenda, clock);
	double when = whereToGo->generatorForPatients->generate_Exponential();
	nextPatient->Activate(when);
}