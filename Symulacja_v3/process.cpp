#include "stdafx.h"
#include <iostream>
#include "process.h"
#include "event.h"
#include "AgendaList.h"

using namespace std;

process::process(double *clock_adr, AgendaList *List) {
	myEvent = new event(this);
	clock = clock_adr;
	Agenda = List;
}

void process::Activate(double time) {
	//cout << "When: " << time << endl;
	myEvent->eventTime = *clock + time;
	Agenda->schedule(myEvent);
}
