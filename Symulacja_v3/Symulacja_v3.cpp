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
class saveToFile
{
public:
	saveToFile(double x, double y) : m_x(x), m_y(y) {}

	friend istream& operator >> (istream& in, saveToFile& obj);
	friend ostream& operator << (ostream& out, const saveToFile& obj);

private:
	double m_x;
	double m_y;
};

istream& operator >> (istream& in, saveToFile& obj)
{
	in >> obj.m_x;
	in >> obj.m_y;
	return in;
}

ostream& operator << (ostream& out, const saveToFile& obj)
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
	int R = 15;		//Poziom jednostek krwi dla wezwania dostawy normalnej
	int N = 20;		//Liczba jednostek krwi w dostawie normalnej
	double Z = 1900.0;	//Średnia rozkładu dla zamówienia normalnego
	double T1 = 300.0;	//Czas utylizacji T1
	double T2 = 500.0;	//Czas utylizacji T2
	double P = 200.0;	//Średnia rozkładu dla pojawiania się kolejnych pacjentów
	double W = 0.23;	//Odwrotność średniej rozkładu jednostek krwi dla pacjenta
	double E = 600.0;	//Średnia i wariancja rozkładu dla zamówienia awaryjnego
	double EW = 0.1;
	int Q = 60;			//Liczba jednostek krwi w zamówieniu awaryjnym
	double L = 850.0;	//Średnia rozkładu dla zgłaszania się dawców
	double A = 0.07;	//Poziom prawdopodobieństwa dostawy awaryjnej

		//Zegar systemowy i wskaźnik do niego
	double clock = 0.0;
	double *clock_adr = &clock;
	AgendaList *Agenda = new AgendaList(nullptr);

		//Ziarna generatorów
	double generatorSeed = 33846533.0;


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

	double stats[200];

	int lastIndex = -1;

	bool zeroStats = false;

	//----------------------------------Pętla główna-------------------------------------//
	while (Centre->patients < 200)
	{
		//Zdejmij zdarzenie z listy
		current = Agenda->returnfirst()->proc;
		clock = Agenda->returnfirst()->eventTime;
		Agenda->removeFirst();
		current->Execute();
		
		//Grupa krwi A

		//Przejście pacjenta do kolejki
		if (Centre->transfusionPointA == nullptr) {
			if (Centre->queueOfPatientsA.empty() != true) {
				Centre->transfusionPointA = Centre->queueOfPatientsA.front();
				Centre->queueOfPatientsA.pop();
				cout << "Pacjent z grupa krwi A opuscil kolejke" << endl;
			}
		}
		
		//Transfuzja

		//Starcza jednostek krwi
		if (Centre->transfusionPointA != nullptr) {
			if (storage->countSamples(true) >= Centre->transfusionPointA->bloodNeeded) {
				//Grupa krwi A
				
					for (int i = 0; i < Centre->transfusionPointA->bloodNeeded; i++) {
						storage->listOfSamplesGroupA->next->myEvent->unscheduleMe();
						storage->listOfSamplesGroupA->next->removeFromList();
					}
					cout << "Pacjent pobral " << Centre->transfusionPointA->bloodNeeded << " jednostek krwi grupy A" << endl;



				//Usunięcie pacjenta ze stanowiska
				delete Centre->transfusionPointA;
				Centre->transfusionPointA = nullptr;
				cout << "Pacjent z grupa krwi A zakonczyl leczenie" << endl;

				//Nowy pacjent wychodzi z kolejki
				if (Centre->queueOfPatientsA.empty() != true) {
					Centre->transfusionPointA = Centre->queueOfPatientsA.front();
					Centre->queueOfPatientsA.pop();
					cout << "Pacjent opuscil kolejke" << endl;
				}
				else cout << "Kolejka pusta (grupa A)" << endl;
			}

			//Nie starcza jednostek krwi - wyślij dostawę awaryjną
			else if (storage->countSamples(true) < Centre->transfusionPointA->bloodNeeded)
			{
				if (storage->urgentFlagA == false) {
					storage->urgentFlagA = true;
					delivery *sendUrgentDelivery = new delivery(true, true, storage, clock_adr, Agenda);
					sendUrgentDelivery->Activate(Centre->generatorForUrgentTransport->generate_Gauss());
					sendUrgentDelivery = nullptr;
					delete sendUrgentDelivery;
					Centre->urgdeliveries += 1;
					cout << "Wyslano dostawe awaryjna na grupe krwi A" << endl;
				}

				if (storage->countSamples(true) > 0) {
					Centre->transfusionPointA->bloodNeeded -= storage->countSamples(true);
					int takeAllBlood = storage->countSamples(true);

					for (int i = 0; i < takeAllBlood; i++) {
						storage->listOfSamplesGroupA->next->myEvent->unscheduleMe();
						storage->listOfSamplesGroupA->next->removeFromList();
					}
					cout << "Pacjent pobral " << takeAllBlood << " jednostek krwi grupy A" << endl;
				}
			}
		}

		//Grupa B

		//Przejście pacjenta do kolejki
		if (Centre->transfusionPointB == nullptr) {
			if (Centre->queueOfPatientsB.empty() != true) {
				Centre->transfusionPointB = Centre->queueOfPatientsB.front();
				Centre->queueOfPatientsB.pop();
				cout << "Pacjent z grupa krwi B opuscil kolejke" << endl;
			}
		}

		//Transfuzja

		//Starcza jednostek krwi
		if (Centre->transfusionPointB != nullptr) {
			if (storage->countSamples(false) >= Centre->transfusionPointB->bloodNeeded) {

				for (int i = 0; i < Centre->transfusionPointB->bloodNeeded; i++) {
					storage->listOfSamplesGroupB->next->myEvent->unscheduleMe();
					storage->listOfSamplesGroupB->next->removeFromList();
				}
				cout << "Pacjent pobral " << Centre->transfusionPointB->bloodNeeded << " jednostek krwi grupy B" << endl;


				//Usunięcie pacjenta ze stanowiska
				delete Centre->transfusionPointB;
				Centre->transfusionPointB = nullptr;
				cout << "Pacjent z grupa krwi B zakonczyl leczenie" << endl;

				//Nowy pacjent wychodzi z kolejki
				if (Centre->queueOfPatientsB.empty() != true) {
					Centre->transfusionPointB = Centre->queueOfPatientsB.front();
					Centre->queueOfPatientsB.pop();
					cout << "Pacjent z grupa krwi B opuscil kolejke" << endl;
				}
				else cout << "Kolejka pusta (grupa B)" << endl;
			}

			//Nie starcza jednostek krwi - wyślij dostawę awaryjną
			else if (storage->countSamples(false) < Centre->transfusionPointB->bloodNeeded)
			{
				if (storage->urgentFlagB == false) {
					storage->urgentFlagB = true;
					delivery *sendUrgentDelivery = new delivery(true, false, storage, clock_adr, Agenda);
					sendUrgentDelivery->Activate(Centre->generatorForUrgentTransport->generate_Gauss());
					sendUrgentDelivery = nullptr;
					delete sendUrgentDelivery;
					Centre->urgdeliveries += 1;
					cout << "Wyslano dostawe awaryjna na grupe krwi B" << endl;
				}

				if (storage->countSamples(false) > 0) {
					Centre->transfusionPointB->bloodNeeded -= storage->countSamples(false);
					int takeAllBlood = storage->countSamples(false);

					for (int i = 0; i < takeAllBlood; i++) {
						storage->listOfSamplesGroupB->next->myEvent->unscheduleMe();
						storage->listOfSamplesGroupB->next->removeFromList();
					}
					cout << "Pacjent pobral " << takeAllBlood << " jednostek krwi grupy B" << endl;
				}
			}
		}

		//Sprawdź czy trzeba wyslać dostawę normalną
		if (storage->countSamples(true) < R) {

			if (storage->normalFlagA == false) {
				storage->normalFlagA = true;
				delivery *sendNormalDelivery = new delivery(false, true, storage, clock_adr, Agenda);
				sendNormalDelivery->Activate(Centre->generatorForNormalTransport->generate_Exponential());
				sendNormalDelivery = nullptr;
				delete sendNormalDelivery;
				Centre->nordeliveries += 1;
				cout << "Wyslano dostawe zwykla na grupe krwi A" << endl;
			}

		}

		//Sprawdź czy trzeba wyslać dostawę normalną
		if (storage->countSamples(false) < R) {

			if (storage->normalFlagB == false) {
				storage->normalFlagB = true;
				delivery *sendNormalDelivery = new delivery(false, false, storage, clock_adr, Agenda);
				sendNormalDelivery->Activate(Centre->generatorForNormalTransport->generate_Exponential());
				sendNormalDelivery = nullptr;
				delete sendNormalDelivery;
				Centre->nordeliveries += 1;
				cout << "Wyslano dostawe zwykla na grupe krwi B" << endl;
			}

		}

		//Badania naukowe
			//Czy wysłać zgłoszenie

			//Grupa A
		if (storage->countSamples(true) > 30) {
			if (Centre->researchFlagA == nullptr) {
				Centre->researchFlagA = new research(Centre, true, Agenda, clock_adr);
				Centre->researchFlagA->Activate(300.0);
				cout << "Zaplanowano pobranie probek krwi grupy A na badania" << endl;
			}
		}
			//Grupa B
		if (storage->countSamples(false) > 30) {
			if (Centre->researchFlagB == nullptr) {
				Centre->researchFlagB = new research(Centre, false, Agenda, clock_adr);
				Centre->researchFlagB->Activate(300.0);
				cout << "Zaplanowano pobranie probek krwi grupy B na badania" << endl;
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
		if (storage->countSamples(false) <= 30) {
			if (Centre->researchFlagB != nullptr) {
				Centre->researchFlagB->myEvent->unscheduleMe();
				delete Centre->researchFlagB;
				Centre->researchFlagB = nullptr;
				cout << "Odwolano pobranie probek krwi grupy B na badania" << endl;
			}
		}


		
		

		/*if (Centre->patients == 199 && zeroStats == false) {
			Centre->patients = 0;
			Centre->urgdeliveries = 0;
			Centre->nordeliveries = 0;
			zeroStats = true;
			system("pause");
		}*/

		if (!zeroStats) {
			if (lastIndex != Centre->patients) {
				lastIndex = Centre->patients;
				stats[lastIndex] = ((double)Centre->urgdeliveries) / ((double)Centre->urgdeliveries + (double)Centre->nordeliveries);
			}
		}
		if (stepMode)system("pause");
	}

	cout << "Zegar systemowy: " << clock << endl;
	cout << "Jednostki krwi grupy A w magazynie: " << storage->countSamples(true) << endl;
	cout << "Jednostki krwi grupy B w magazynie: " << storage->countSamples(false) << endl;
	cout << "Pacjenci z grupa A w kolejce: " << Centre->queueOfPatientsA.size() << endl;
	cout << "Pacjenci z grupa B w kolejce: " << Centre->queueOfPatientsB.size() << endl;
	cout << "Zdarzen na liscie: " << Agenda->checkIfOK() << endl;
	cout << "Pacjenci: " << Centre->patients << endl;
	cout << "Dawcy: " << Centre->donators << endl;
	cout << "Badania: " << Centre->researches << endl;
	cout << "Zamowienia awaryjne: " << Centre->urgdeliveries << endl;
	cout << "Zamowienia zwykle: " << Centre->nordeliveries << endl;
	cout << endl;
	cout << "Procent zamowien awaryjnych: " << ((double)Centre->urgdeliveries) / ((double)Centre->urgdeliveries + (double)Centre->nordeliveries);
	cout << endl;

	ofstream outFile("fazaPoczatkowa.dat");
	for (int i = 0; i < 200; i++) {
		saveToFile myObj(i, stats[i]);
		outFile << myObj;
	}
	outFile.close();
	
	system("pause");
    return 0;
}

