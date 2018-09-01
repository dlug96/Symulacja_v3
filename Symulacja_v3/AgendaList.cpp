#include "stdafx.h"
#include <iostream>
#include "AgendaList.h"
#include "event.h"

using namespace std;

AgendaList::AgendaList(event *toStore) {
	next = nullptr;
	previous = nullptr;
	eventStored = toStore;
	if (eventStored)eventStored->myAgenda = this;
}

/*
void AgendaList::schedule(event *toSchedule) {
	//Wstaw jako pierwsze
	if (next == nullptr) {
		next = new AgendaList(toSchedule);
		next->previous = this;
	}
	
	else if (next->eventStored->eventTime > toSchedule->eventTime) {
		AgendaList *toStore = new AgendaList(toSchedule);
		toStore->next = next;
		next->previous = toStore;
		next = toStore;
		toStore->previous = this;
		toStore = nullptr;
		delete toStore;
	}

	//Wstaw jako n-te
	else {
		int chooseCase = 0; //0 - nic dalej na liście, 1 - coś dalej jest

		AgendaList *helper = this;

		if (next) {
			while (helper->next) {
				if (helper->next->eventStored->eventTime > toSchedule->eventTime) {
					chooseCase = 1;
					break;
				}
				else helper = helper->next;
			}
		}

		switch (chooseCase) {
		case 0: {
			helper->next = new AgendaList(toSchedule);
			helper->next->eventStored = toSchedule;
			if (eventStored)eventStored->myAgenda = helper->next;
			next->previous = helper;
		}
				break;
		case 1: {
			AgendaList *store = new AgendaList(toSchedule);
			store->eventStored = toSchedule;
			//if(eventStored)eventStored->myAgenda = store;
			store->previous = helper;
			store->next = helper->next;
			helper->next = store;
			store = nullptr;
			delete store;
		}
				break;
		}
		helper = nullptr;
		delete helper;
	}
}
*/

void AgendaList::schedule(event *toSchedule) 
{
	if (next == nullptr) {
		next = new AgendaList(toSchedule);
		next->previous = this;
	}
	else if (next->eventStored->eventTime > toSchedule->eventTime)
	{
		AgendaList *store = new AgendaList(toSchedule);
		store->next = next;
		store->previous = this;
		next->previous = store;
		next = store;
	}
	else
	{
		AgendaList *store = this;
		while (store->next) {
			if (store->next->eventStored->eventTime > toSchedule->eventTime)break;
			else store = store->next;
		}
		if (store->next == nullptr) {
			store->next = new AgendaList(toSchedule);
			store->next->previous = store;
		}
		else
		{
			AgendaList *helper = new AgendaList(toSchedule);
			helper->next = store->next;
			helper->previous = store;
			helper->next->previous = helper;
			store->next = helper;
		}
		
	}
}

void AgendaList::unschedule(event *toUnschedule)
{
	AgendaList *helper = this;
	while (helper->next) {
		if (helper->next->eventStored == toUnschedule)break;
		else helper = helper->next;
	}

	int chooseCase = 0;
	if (helper->next->next)chooseCase = 1;

	AgendaList *remover = helper->next;

	switch (chooseCase) {
	case 0:
	{
		helper->next = nullptr;
		remover->previous = nullptr;

		break;
	}
	case 1:
	{
		helper->next = remover->next;
		remover->next->previous = helper;
		remover->next = nullptr;
		remover->previous = nullptr;

		break;
	}
	}

	remover->eventStored = nullptr;
	delete remover;
	helper = nullptr;
	delete helper;
}

void AgendaList::showAgenda() {
	AgendaList *helper = this;
	int counter = 0;
	if (helper->next) {
		while (helper->next) {
			cout << counter << ": " << helper->next->eventStored->eventTime << endl;
			helper = helper->next;
		}
	}
	else {
		cout << "Lista pusta" << endl;
	}
}


//Niepewne

event* AgendaList::returnfirst() {
	return this->next->eventStored;
}

void AgendaList::removeFirst() {

	AgendaList *helper = this->next;
	if (this->next->next != nullptr) {

		helper->next->previous = this;

		this->next = helper->next;
		helper->next = nullptr;
		helper->previous = nullptr;
		helper->eventStored = nullptr;
		delete helper;
		helper = nullptr;
	}
	else {
		this->next->eventStored = nullptr;
		this->next->previous = nullptr;
		delete this->next;
		this->next = nullptr;
	}
}

void AgendaList::removeThis(event * toRemove)
{
	bool success = true;
	AgendaList *helper = this;
	while (true) {
		if (helper->eventStored == toRemove)break;
		if (helper->next)helper = helper->next;
		else {
			success = false;
			break;
		}
	}

	if (success) {
		if (helper->next) {
			helper->next->previous = helper->previous;
			helper->previous->next = helper->next;
		}
		else helper->previous->next = nullptr;

		helper->next = nullptr;
		helper->previous = nullptr;
		helper->eventStored = nullptr;
		delete helper;
		helper = nullptr;
	}

	else {
		cout << "Brak takiego zdarzenia na liscie" << endl;
	}
	
}

int AgendaList::checkIfOK()
{
	AgendaList *helper = this;
	int counter = 0;
	while (helper->next) {
		counter += 1;
		helper = helper->next;
	}
	return counter;
}
