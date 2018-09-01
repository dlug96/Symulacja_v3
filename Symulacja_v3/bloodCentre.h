#pragma once
#include "stdafx.h"
#include "Header.h"
#include <queue>

class bloodCentre
{
public:

	bloodStorage *storage;					//Magazyn jednostek krwi
	patient *transfusionPoint;				//Punkt transfuzji
	std::queue<patient*> queueOfPatients;   //Kolejka pacjentów

	double *clock;
	AgendaList *Agenda;

	int patients;
	int urgdeliveries;
	int nordeliveries;

	generator_Exponential *generatorForNormalTransport;

	generator_Gauss *generatorForUrgentTransport;

	generator_Exponential *generatorForDonators;

	generator_Exponential *generatorForPatients;

	generator_Geometrical *generatorForBloodSamplesNeeded;


	bloodCentre(double genNormalTransportStart, double genNormalTransportStartMean,
		double genUrgentTransportStart, double genUrgentTransportMean, double genUrgentTransportVar,
		double genPatientsStart, double genPatientsMean, 
		double genDonatorsStart, double genDonatorsMean, 
		double genSamplesNeededStart, double genSamplesNeededMean, 
		double *whereIsClock, AgendaList *List, double *clock_adr);
};