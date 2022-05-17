#include "StdAfx.h"
#include "DsimModel.h"

INT DsimModel::isdigital (CHAR *pinname)
{
	return TRUE; // Indicates this is a digital model
}

VOID DsimModel::setup (IINSTANCE *instance, IDSIMCKT *dsimckt)
{
	inst = instance;
	ckt = dsimckt;
	
	for (auto i = 0; i < 5; i++)
	{
		char name[] = {'A', '0' + i, '\0'};
		pin_A[i] = inst->getdsimpin(name, true);
	}
	
	for (auto i = 0; i < 8; i++)
	{
		char name[] = { 'D', '0' + i, '\0' };
		pin_D[i] = inst->getdsimpin(name, true);
	}

	pin_V = inst->getdsimpin("$V$", true);	// Creates pin V
}

VOID DsimModel::runctrl (RUNMODES mode)
{
}

VOID DsimModel::actuate (REALTIME time, ACTIVESTATE newstate)
{
}

BOOL DsimModel::indicate (REALTIME time, ACTIVEDATA *data)
{
	return FALSE;
}

VOID DsimModel::simulate(ABSTIME time, DSIMMODES mode) {
	if (ishigh(pin_V->istate()))
	{
		for (auto pin : pin_D)
		{
			pin->setstate(time, 1, SLO);
		}
	}
	else
	{
		for (auto i = 0; i < 5; i++)
		{
			pin_D[i]->setstate(time, 1, (ishigh(pin_A[i]->istate())) ? SHI : SLO);
		}
		for (auto i = 5; i < 8; i++)
		{
			pin_D[i]->setstate(time, 1, (i & 1) ? SHI : SLO);
		}
	}
}

VOID DsimModel::callback (ABSTIME time, EVENTID eventid)
{
}
