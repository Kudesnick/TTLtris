#include <stdlib.h>

#include "StdAfx.h"
#include "k155ru5.h"

using namespace std;

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

	debug = inst->getboolval("debug", false);
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
	STATE result = FLT;

	if (true
		&& islow(pin_CS1->istate())
		&& islow(pin_CS2->istate())
		&& ishigh(pin_CS3->istate())
		)
	{
		int addr = get_addr();
		int byte = addr >> 3;
		int mask = 1 << (addr & 0x7);
		if (islow(pin_WE->istate()))
		{
			if (ishigh(pin_DI->istate())) memory[byte] |= mask;
			else memory[byte] &= ~mask;

			if (debug)
			{
				inst->log("0x%04X = 0x%02X", byte, memory[byte]);
			}
		}
		else if ((memory[byte] & mask) == 0) result = SLO;
	}

	pin_DO->setstate(time, 50000, result);
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
