#include <filesystem>

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

	char *fname = inst->getstrval("file");
	if (fname != NULL)
	{
		if (std::filesystem::exists(fname))
		{
			auto f = fopen(fname, "r");
			if (f != NULL)
			{
				inst->log("Loading file '%s'.", fname);

				// Loading data

				fclose(f);
			}
			else
			{
				inst->error("File '%s' can't opened!", fname);
			}
		}
		else
		{
			inst->warning("File '%s' not exists.", fname);
		}
	}

	for (auto i = 0; i < sizeof(memory); i++)
	{
		memory[i] = i;
	}
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

VOID DsimModel::simulate(ABSTIME time, DSIMMODES mode)
{
	set_data(time, (ishigh(pin_V->istate())) ? 0x00 : memory[get_addr()]);
}

VOID DsimModel::callback (ABSTIME time, EVENTID eventid)
{
}

// Private functions

UINT8 DsimModel::get_addr(VOID)
{
	UINT8 result = 0;

	for (auto &pin : pin_A)
	{
		result >>= 1;
		result |= (ishigh(pin->istate())) ? (1 << _countof(pin_A) - 1) : 0;
	}

	return result;
}

VOID DsimModel::set_data(ABSTIME time, UINT8 data)
{
	for (auto &pin : pin_D)
	{
		pin->setstate(time, 1, (data & 1) ? SLO : SHI); // Negative output
		data >>= 1;
	}
}
