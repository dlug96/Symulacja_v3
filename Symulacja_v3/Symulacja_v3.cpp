// Symulacja_v3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include "Header.h"

#include "process.h"
#include "event.h"
#include "AgendaList.h"

#include "patient.h"
#include "donator.h"
#include "bloodCentre.h"
#include "bloodSample.h"
#include "bloodStorage.h"
#include "delivery.h"
#include "research.h"

#include "generator_01.h"
#include "generator_Exponential.h"
#include "generator_Gauss.h"
#include "generator_Bernoulli.h"
#include "generator_Geometrical.h"
#include "generator_5_10.h"

using namespace std;

//---------------------------------Klasa do zapisu do pliku-------------------------------//
class stats
{
public:
	stats(double x, double y);

	friend istream& operator >> (istream& in, stats& obj);
	friend ostream& operator << (ostream& out, const stats& obj);

private:
	double m_x;
	double m_y;
};

istream& operator >> (istream& in, stats& obj)
{
	in >> obj.m_x;
	in >> obj.m_y;
	return in;
}

ostream& operator << (ostream& out, const stats& obj)
{
	out << obj.m_x << ' ';
	out << obj.m_y << endl;
	return out;
}
//Grupy krwi
//A - true
//B - false



//---------------------------------------------------------------------------------------//
int main()
{
	//Wybór trybu pracy
	bool stepMode;
	string workMode;
	while (true)
	{
		cout << "Czy uruchomic tryb krokowy? (T/N)" << endl;
		getline(cin, workMode);
		if (workMode == "t" || workMode == "T") {
			stepMode = true;
			break;
		}
		else if (workMode == "n" || workMode == "N") {
			stepMode = false;
			break;
		}
		else continue;
	}
	

	//-------------------------------------------------------------------------------------------------//
	//Podanie danych

		//Stałe
	int R = 10;		//Poziom jednostek krwi dla wezwania dostawy normalnej
	int N = 30;		//Liczba jednostek krwi w dostawie normalnej
	double Z = 1900.0;	//Średnia rozkładu dla zamówienia normalnego
	double T1 = 300.0;	//Czas utylizacji T1
	double T2 = 500.0;	//Czas utylizacji T2
	double P = 200.0;	//Średnia rozkładu dla pojawiania się kolejnych pacjentów
	double W = 0.23;	//Odwrotność średniej rozkładu jednostek krwi dla pacjenta
	double E = 400.0;	//Średnia i wariancja rozkładu dla zamówienia awaryjnego
	double EW = 0.1;
	int Q = 12;			//Liczba jednostek krwi w zamówieniu awaryjnym
	double L = 850.0;	//Średnia rozkładu dla zgłaszania się dawców
	double A = 0.07;	//Poziom prawdopodobieństwa dostawy awaryjnej

		//Zegar systemowy i wskaźnik do niego
	double clock = 0.0;
	double *clock_adr = &clock;
	AgendaList *Agenda = new AgendaList(nullptr);

		//Ziarna generatorów
	double generatorSeed = 100.0;


	//-------------------------------------------------------------------------------------------------//
	//Tworzenie obiektów symulacji

		//Centrum krwiodawstwa i magazyn
	bloodCentre *Centre = new bloodCentre(generatorSeed, Z, 
		generatorSeed, E, EW, 
		generatorSeed, P, 
		generatorSeed, L, 
		generatorSeed, W,
		generatorSeed,
		generatorSeed,
		generatorSeed,
		clock_adr, Agenda, clock_adr);
	bloodStorage *storage = Centre->storage;

		//Zainicjowanie pierwszego pacjenta i pierwszego dawcy
	patient *sourcePatient = new patient(Centre->generatorForBloodSamplesNeeded->generate_Geometrical(), Centre, Agenda, clock_adr);
	donator *sourceDonator = new donator(Centre, Agenda, clock_adr);
	sourcePatient->Activate(0.0);
	sourceDonator->Activate(Centre->generatorForDonators->generate_Exponential());

	double tillEnd = 15000.0;

	process *current = nullptr;


	//----------------------------------Pętla główna-------------------------------------//
	while (clock < tillEnd)
	{
		//Zdejmij zdarzenie z listy
		current = Agenda->returnfirst()->proc;
		clock = Agenda->returnfirst()->eventTime;
		Agenda->removeFirst();
		current->Execute();
		
		//Przejście pacjenta do kolejki
		if (Centre->transfusionPoint == nullptr) {
			if (Centre->queueOfPatients.empty() != true) {
				Centre->transfusionPoint = Centre->queueOfPatients.front();
				Centre->queueOfPatients.pop();
				cout << "Pacjent opuscil kolejke" << endl;
			}
		}
		
		//Transfuzja

		//Starcza jednostek krwi
		if (Centre->transfusionPoint != nullptr) {
			if (storage->countSamples(Centre->transfusionPoint->bloodGroup) >= Centre->transfusionPoint->bloodNeeded) {
				//Grupa krwi A
				if (Centre->transfusionPoint->bloodGroup) {
					for (int i = 0; i < Centre->transfusionPoint->bloodNeeded; i++) {
						storage->listOfSamplesGroupA->next->myEvent->unscheduleMe();
						storage->listOfSamplesGroupA->next->removeFromList();
					}
					cout << "Pacjent pobral " << Centre->transfusionPoint->bloodNeeded << " jednostek krwi grupy A" << endl;
				}
				//Grupa krwi B
				else {
					for (int i = 0; i < Centre->transfusionPoint->bloodNeeded; i++) {
						storage->listOfSamplesGroupB->next->myEvent->unscheduleMe();
						storage->listOfSamplesGroupB->next->removeFromList();
					}
					cout << "Pacjent pobral " << Centre->transfusionPoint->bloodNeeded << " jednostek krwi grupy B" << endl;
				}

				//Sprawdź czy trzeba wyslać dostawę normalną
				if (storage->countSamples(Centre->transfusionPoint->bloodGroup) < R) {
					if (Centre->transfusionPoint->bloodGroup) {
						if (storage->normalFlagA == false) {
							storage->normalFlagA = true;
							delivery *sendNormalDelivery = new delivery(false, Centre->transfusionPoint->bloodGroup, storage, clock_adr, Agenda);
							sendNormalDelivery->Activate(Centre->generatorForNormalTransport->generate_Exponential());
							sendNormalDelivery = nullptr;
							delete sendNormalDelivery;
							Centre->nordeliveries += 1;
							cout << "Wyslano dostawe zwykla na grupe krwi A" << endl;
						}
					}
					else {
						storage->normalFlagB = true;
						delivery *sendNormalDelivery = new delivery(false, Centre->transfusionPoint->bloodGroup, storage, clock_adr, Agenda);
						sendNormalDelivery->Activate(Centre->generatorForNormalTransport->generate_Exponential());
						sendNormalDelivery = nullptr;
						delete sendNormalDelivery;
						Centre->nordeliveries += 1;
						cout << "Wyslano dostawe zwykla na grupe krwi B" << endl;
					}
				}
				//Usunięcie pacjenta ze stanowiska
				delete Centre->transfusionPoint;
				Centre->transfusionPoint = nullptr;
				cout << "Pacjent zakonczyl leczenie" << endl;

				//Nowy pacjent wychodzi z kolejki
				if (Centre->queueOfPatients.empty() != true) {
					Centre->transfusionPoint = Centre->queueOfPatients.front();
					Centre->queueOfPatients.pop();
					cout << "Pacjent opuscil kolejke" << endl;
				}
				else cout << "Kolejka pusta" << endl;
			}

			//Nie starcza jednostek krwi - wyślij dostawę awaryjną
			else if (storage->countSamples(Centre->transfusionPoint->bloodGroup) < Centre->transfusionPoint->bloodNeeded)
			{

				if (Centre->transfusionPoint->bloodGroup) {
					if (storage->urgentFlagA == false) {
						storage->urgentFlagA = true;
						delivery *sendUrgentDelivery = new delivery(true, Centre->transfusionPoint->bloodGroup, storage, clock_adr, Agenda);
						sendUrgentDelivery->Activate(Centre->generatorForUrgentTransport->generate_Gauss());
						//sendUrgentDelivery->Activate(5.0);
						sendUrgentDelivery = nullptr;
						delete sendUrgentDelivery;
						Centre->urgdeliveries += 1;
						cout << "Wyslano dostawe awaryjna na grupe krwi A" << endl;
					}
				}
				else {
					if (storage->urgentFlagB == false) {
						storage->urgentFlagB = true;
						delivery *sendUrgentDelivery = new delivery(true, Centre->transfusionPoint->bloodGroup, storage, clock_adr, Agenda);
						sendUrgentDelivery->Activate(Centre->generatorForUrgentTransport->generate_Gauss());
						//sendUrgentDelivery->Activate(5.0);
						sendUrgentDelivery = nullptr;
						delete sendUrgentDelivery;
						Centre->urgdeliveries += 1;
						cout << "Wyslano dostawe awaryjna na grupe krwi B" << endl;
					}
				}
				if (storage->countSamples(Centre->transfusionPoint->bloodGroup) > 0) {
					Centre->transfusionPoint->bloodNeeded -= storage->countSamples(Centre->transfusionPoint->bloodGroup);
					int takeAllBlood = storage->countSamples(Centre->transfusionPoint->bloodGroup);
					if (Centre->transfusionPoint->bloodGroup) {
						for (int i = 0; i < takeAllBlood; i++) {
							storage->listOfSamplesGroupA->next->myEvent->unscheduleMe();
							storage->listOfSamplesGroupA->next->removeFromList();
						}
						cout << "Pacjent pobral " << takeAllBlood << " jednostek krwi grupy A" << endl;
					}
					else {
						for (int i = 0; i < takeAllBlood; i++) {
							storage->listOfSamplesGroupB->next->myEvent->unscheduleMe();
							storage->listOfSamplesGroupB->next->removeFromList();
						}
						cout << "Pacjent pobral " << takeAllBlood << " jednostek krwi grupy B" << endl;
					}
				}
			}
		}

		//Badania naukowe
			//Czy wysłać zgłoszenie
		if (storage->countSamples(true) > 30) {
			if (Centre->researchFlagA == nullptr) {
				Centre->researchFlagA = new research(Centre, true, Agenda, clock_adr);
				Centre->researchFlagA->Activate(300.0);
				cout << "Zaplanowano pobranie probek krwi grupy A na badania" << endl;
			}
		}
		if (storage->countSamples(false) > 30) {
			if (Centre->researchFlagB == nullptr) {
				Centre->researchFlagB = new research(Centre, true, Agenda, clock_adr);
				Centre->researchFlagB->Activate(300.0);
				cout << "Zaplanowano pobranie probek krwi grupy A na badania" << endl;
			}
		}

			//Czy odwołać zgłoszenie
		if (storage->countSamples(true) <= 30) {
			if (Centre->researchFlagA != nullptr) {
				Centre->researchFlagA->myEvent->unscheduleMe();
				delete Centre->researchFlagA;
				Centre->researchFlagA = nullptr;
				cout << "Odwolano pobranie probek krwi grupy A na badania" << endl;
			}
		}




		cout << "Zegar systemowy: " << clock << endl;
		cout << "Jednostki krwi grupy A w magazynie: " << storage->countSamples(true) << endl;
		cout << "Jednostki krwi grupy B w magazynie: " << storage->countSamples(false) << endl;
		cout << "Pacjenci w kolejce: " << Centre->queueOfPatients.size() << endl;
		cout << "Zdarzen na liscie: " << Agenda->checkIfOK() << endl;
		cout << "Pacjenci: " << Centre->patients << endl;
		cout << "Dawcy: " << Centre->donators << endl;
		cout << "Badania: " << Centre->researches << endl;
		cout << "Zamowienia awaryjne: " << Centre->urgdeliveries << endl;
		cout << "Zamowienia zwykle: " << Centre->nordeliveries << endl;
		cout << endl;

		if (stepMode)system("pause");
	}

	
	system("pause");
    return 0;
}

