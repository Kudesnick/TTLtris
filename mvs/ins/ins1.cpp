#include <stdlib.h>

#include "StdAfx.h"
#include <sstream>
#include "ins1.h"

using namespace std;

#define GRID (100)

// IDSIMMODEL

INT ins1Dsim::isdigital(CHAR* pinname)
{
	return TRUE; // Indicates this is a digital model
}

VOID ins1Dsim::setup(IINSTANCE* instance, IDSIMCKT* dsimckt)
{
	inst = instance;
	ckt = dsimckt;

	IDSIMPIN* pin;

	for (auto i = 0; ; i++)
	{
		char name[] = "A\0\0\0";
		if (i >= 10)
		{
			name[1] = i / 10 + '0';
			name[2] = i % 10 + '0';
		}
		else
		{
			name[1] = i + '0';
		}
		pin = inst->getdsimpin(name, false);
		if (pin != NULL)
		{
			pin_anode.push_back(pin);
		}
		else
		{
			break;
		}
	}

	for (auto i = 0; ; i++)
	{
		char name[] = "K\0\0\0";
		if (i >= 10)
		{
			name[1] = i / 10 + '0';
			name[2] = i % 10 + '0';
		}
		else
		{
			name[1] = i + '0';
		}
		pin = inst->getdsimpin(name, false);
		if (pin != NULL)
		{
			pin_cathode.push_back(pin);
		}
		else
		{
			break;
		}
	}

	debug = inst->getboolval("debug", false);

	inst->log("matrix created with %d anodes and %d cathodes", pin_anode.size(), pin_cathode.size());
}

VOID ins1Dsim::runctrl(RUNMODES mode)
{
}

VOID ins1Dsim::actuate(REALTIME time, ACTIVESTATE newstate)
{
}

BOOL ins1Dsim::indicate(REALTIME time, ACTIVEDATA* data)
{
	return TRUE;
}

VOID ins1Dsim::simulate(ABSTIME time, DSIMMODES mode)
{
}

VOID ins1Dsim::callback(ABSTIME time, EVENTID eventid)
{
}

// IACTIVEMODEL

VOID ins1Active::initialize(ICOMPONENT* cpt)
{
	this->cpt = cpt;
}

ISPICEMODEL* ins1Active::getspicemodel(CHAR* primitive)
{
	return NULL;
}

IDSIMMODEL* ins1Active::getdsimmodel(CHAR* primitive)
{
	return new ins1Dsim;
}

VOID ins1Active::plot(ACTIVESTATE state)
{
	cpt->drawcircle(GRID, -GRID, GRID / 2);
}

VOID ins1Active::animate(INT element, ACTIVEDATA* newstate)
{
}

BOOL ins1Active::actuate(WORD key, INT x, INT y, DWORD flags)
{
	return FALSE;
}

// Private functions
