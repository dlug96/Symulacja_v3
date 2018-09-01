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

bloodCentre::bloodCentre(double genNormalTransportStart, double genNormalTransportStartMean, double genUrgentTransportStart, double genUrgentTransportMean, double genUrgentTransportVar, double genPatientsStart, double genPatientsMean, double genDonatorsStart, double genDonatorsMean, double genSamplesNeededStart, double genSamplesNeededMean, double *whereIsClock, AgendaList *List, double *clock_adr)
{
	Agenda = List;
	clock = clock_adr;

	generatorForNormalTransport = new generator_Exponential(genNormalTransportStart, genNormalTransportStartMean);
	generatorForUrgentTransport = new generator_Gauss(genUrgentTransportMean, genUrgentTransportVar, genUrgentTransportStart);
	generatorForPatients = new generator_Exponential(genPatientsStart, genPatientsMean);
	generatorForDonators = new generator_Exponential(genDonatorsStart, genDonatorsMean);
	generatorForBloodSamplesNeeded = new generator_Geometrical(genSamplesNeededMean, genSamplesNeededStart);

	storage = new bloodStorage(this);
	transfusionPoint = nullptr;

	//Do statystyk
	int patients = 0;
	int urgdeliveries = 0;
	int nordeliveries = 0;

}