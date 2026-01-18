#include <stdlib.h>

#include "StdAfx.h"
#include "k155re3.h"

using namespace std;

VOID k155re3::initialize(ICOMPONENT* cpt)
{
}

ISPICEMODEL* k155re3::getspicemodel(CHAR* primitive)
{
	return NULL;
}

IDSIMMODEL* k155re3::getdsimmodel(CHAR* primitive)
{
	return this;
}

VOID k155re3::plot(ACTIVESTATE state)
{
}

VOID k155re3::animate(INT element, ACTIVEDATA* newstate)
{
}

BOOL k155re3::actuate(WORD key, INT x, INT y, DWORD flags)
{
	return FALSE;
}

//----------------

INT k155re3::isdigital (CHAR *pinname)
{
	return TRUE; // Indicates this is a digital model
}

VOID k155re3::setup (IINSTANCE *instance, IDSIMCKT *dsimckt)
{
	inst = instance;
	ckt = dsimckt;
	
	auto i = '0';
	for (auto &pin : pin_A)
	{
		char name[] = {'A', i++, '\0'};
		pin = inst->getdsimpin(name, true);
	}
	
	i = '0';
	for (auto &pin : pin_D)
	{
		char name[] = {'D', i++, '\0'};
		pin = inst->getdsimpin(name, true);
	}

	pin_V = inst->getdsimpin("$V$", true);	// Creates pin V

	memset(memory, 0, sizeof(memory));

	if (inst->getstrval("file") != NULL)
		inst->loadmemory(inst->getstrval("file"), memory, _countof(memory), inst->getinitval("base"), inst->getinitval("shift"));
}

VOID k155re3::runctrl (RUNMODES mode)
{
}

VOID k155re3::actuate (REALTIME time, ACTIVESTATE newstate)
{
}

BOOL k155re3::indicate (REALTIME time, ACTIVEDATA *data)
{
	return FALSE;
}

VOID k155re3::simulate(ABSTIME time, DSIMMODES mode)
{
	set_data(time, (ishigh(pin_V->istate())) ? 0x00 : memory[get_addr()]);
}

VOID k155re3::callback (ABSTIME time, EVENTID eventid)
{
}

// Private functions

UINT8 k155re3::get_addr(VOID)
{
	UINT8 result = 0;

	for (auto &pin : pin_A)
	{
		result >>= 1;
		result |= (ishigh(pin->istate())) ? 1 << _countof(pin_A) >> 1 : 0;
	}

	return result;
}

VOID k155re3::set_data(ABSTIME time, UINT8 data)
{
	for (auto &pin : pin_D)
	{
		pin->setstate(time, 1, (data & 1) ? SLO : SHI); // Negative output
		data >>= 1;
	}
}
