#include "stdafx.h"
#include "process.h"
#include "event.h"
#include "AgendaList.h"

event::event(process *ptr) {
	eventTime = 0.0;
	proc = ptr;
	myAgenda = nullptr;
}

void event::unscheduleMe() {
	AgendaList *helper = this->myAgenda;
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