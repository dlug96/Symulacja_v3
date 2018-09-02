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

	research *researchFlagA;
	research *researchFlagB;

	int patients;
	int urgdeliveries;
	int nordeliveries;
	int donators;
	int researches;

	generator_Exponential *generatorForNormalTransport;

	generator_Gauss *generatorForUrgentTransport;

	generator_Exponential *generatorForDonators;

	generator_Exponential *generatorForPatients;

	generator_Geometrical *generatorForBloodSamplesNeeded;

	generator_5_10 *generatorForResearch;

	generator_Bernoulli *generatorForBloodGroupPatients;

	generator_Bernoulli *generatorForBloodGroupDonators;


	bloodCentre(double genNormalTransportStart, double genNormalTransportStartMean,
		double genUrgentTransportStart, double genUrgentTransportMean, double genUrgentTransportVar,
		double genPatientsStart, double genPatientsMean, 
		double genDonatorsStart, double genDonatorsMean, 
		double genSamplesNeededStart, double genSamplesNeededMean,
		double genResearchStart,
		double genBGPatientsStart,
		double genBGDonatorsStart,
		double *whereIsClock, AgendaList *List, double *clock_adr);
};