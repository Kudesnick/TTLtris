#include <stdlib.h>

#include "StdAfx.h"
#include "k155ru5.h"

using namespace std;

VOID k155ru5::initialize(ICOMPONENT* cpt)
{
}

ISPICEMODEL* k155ru5::getspicemodel(CHAR* primitive)
{
	return NULL;
}

IDSIMMODEL* k155ru5::getdsimmodel(CHAR* primitive)
{
	return this;
}

VOID k155ru5::plot(ACTIVESTATE state)
{
}

VOID k155ru5::animate(INT element, ACTIVEDATA* newstate)
{
}

BOOL k155ru5::actuate(WORD key, INT x, INT y, DWORD flags)
{
	return FALSE;
}

//----------------

INT k155ru5::isdigital (CHAR *pinname)
{
	return TRUE; // Indicates this is a digital model
}

VOID k155ru5::setup (IINSTANCE *instance, IDSIMCKT *dsimckt)
{
	inst = instance;
	ckt = dsimckt;
	
	auto i = '0';
	for (auto &pin : pin_A)
	{
		char name[] = {'A', i++, '\0'};
		pin = inst->getdsimpin(name, true);
	}
	
	pin_CS1 = inst->getdsimpin("$CS1$", true);
	pin_CS2 = inst->getdsimpin("$CS2$", true);
	pin_CS3 = inst->getdsimpin("CS3", true);
	pin_WE = inst->getdsimpin("$WE$", true);
	pin_DI = inst->getdsimpin("DI", true);
	pin_DO = inst->getdsimpin("DO", true);

	memset(memory, 0, sizeof(memory));

	if (inst->getstrval("file") != NULL)
		inst->loadmemory(inst->getstrval("file"), memory, _countof(memory), inst->getinitval("base"), inst->getinitval("shift"));
}

VOID k155ru5::runctrl (RUNMODES mode)
{
}

VOID k155ru5::actuate (REALTIME time, ACTIVESTATE newstate)
{
}

BOOL k155ru5::indicate (REALTIME time, ACTIVEDATA *data)
{
	return FALSE;
}

VOID k155ru5::simulate(ABSTIME time, DSIMMODES mode)
{
	STATE result = SHI;

	if (true
		&& islow(pin_CS1->istate())
		&& islow(pin_CS2->istate())
		&& ishigh(pin_CS3->istate())
		)
	{
		if (islow(pin_WE->istate()))
			memory[get_addr()] = memory[get_addr()] & 0xFE | ishigh(pin_DI->istate()) ? 1 : 0;
		else
			result = (memory[get_addr()] & 1) ? SHI : SLO;
	}

	pin_DO->setstate(time, 1, result);
}

VOID k155ru5::callback (ABSTIME time, EVENTID eventid)
{
}

// Private functions

UINT8 k155ru5::get_addr(VOID)
{
	UINT8 result = 0;

	for (auto &pin : pin_A)
	{
		result >>= 1;
		result |= (ishigh(pin->istate())) ? 1 << _countof(pin_A) >> 1 : 0;
	}

	return result;
}
