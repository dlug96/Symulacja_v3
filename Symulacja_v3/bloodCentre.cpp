#include "stdafx.h"
#include "bloodCentre.h"
#include "patient.h"
#include <queue>
#include "bloodStorage.h"
#include "generator_01.h"
#include "generator_Bernoulli.h"
#include "generator_Exponential.h"
#include "generator_Gauss.h"
#include "generator_Geometrical.h"
#include "generator_5_10.h"
#include "research.h"

bloodCentre::bloodCentre(double genNormalTransportStart, double genNormalTransportStartMean, 
	double genUrgentTransportStart, double genUrgentTransportMean, double genUrgentTransportVar, 
	double genPatientsStart, double genPatientsMean, 
	double genDonatorsStart, double genDonatorsMean, 
	double genSamplesNeededStart, double genSamplesNeededMean,
	double genResearchStart,
	double genBGPatientsStart,
	double genBGDonatorsStart,
	double *whereIsClock, AgendaList *List, double *clock_adr)
{
	Agenda = List;
	clock = clock_adr;

	researchFlagA = nullptr;
	researchFlagB = nullptr;

	generatorForNormalTransport = new generator_Exponential(genNormalTransportStart, genNormalTransportStartMean);
	generatorForUrgentTransport = new generator_Gauss(genUrgentTransportMean, genUrgentTransportVar, genUrgentTransportStart);
	generatorForPatients = new generator_Exponential(genPatientsStart, genPatientsMean);
	generatorForDonators = new generator_Exponential(genDonatorsStart, genDonatorsMean);
	generatorForBloodSamplesNeeded = new generator_Geometrical(genSamplesNeededMean, genSamplesNeededStart);
	generatorForResearch = new generator_5_10(genResearchStart);
	generatorForBloodGroupPatients = new generator_Bernoulli(0.4, genBGPatientsStart);
	generatorForBloodGroupDonators = new generator_Bernoulli(0.4, genBGDonatorsStart);

	storage = new bloodStorage(this);
	transfusionPoint = nullptr;

	//Do statystyk
	patients = 0;
	urgdeliveries = 0;
	nordeliveries = 0;
	donators = 0;
	researches = 0;

}