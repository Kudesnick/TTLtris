#include <stdlib.h>

#include "StdAfx.h"
#include <sstream>
#include "ins1.h"

using namespace std;

#define GRID (100)
#define TRIGMIN (500ULL * 1000) // «адержка активации пикселей после активации анода в пикосекундах

// IDSIMMODEL


std::vector<std::vector<STATE>> matrix, buff;

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
			pinAnode.push_back(pin);
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
			pinCathode.push_back(pin);
		}
		else
		{
			break;
		}
	}

	matrix.resize(pinAnode.size());
	for (auto& row : matrix)
	{
		row.resize(pinCathode.size());
		row.shrink_to_fit();
		for (auto& pixel : row) pixel = UNDEFINED;
	}
	matrix.shrink_to_fit();

	buff.resize(pinAnode.size());
	for (auto& row : buff)
	{
		row.resize(pinCathode.size());
		row.shrink_to_fit();
		for (auto& pixel : row) pixel = UNDEFINED;
	}
	buff.shrink_to_fit();

	debug = inst->getboolval("debug", false);

	inst->log("matrix created with %d anodes and %d cathodes", pinAnode.size(), pinCathode.size());
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
	for (size_t i = 0; i < pinAnode.size(); i++)
	{
		auto& pin = pinAnode[i];
		if (pin.pin->istate() != pin.lastState)
		{
			pin.lastState = pin.pin->istate();
			if (islow(pin.lastState))
			{
				ckt->setcallback(TRIGMIN, this, i);
			}
		}
	}
}

VOID ins1Dsim::callback(ABSTIME time, EVENTID eventid)
{
	if (eventid < matrix.size())
	{
		for (int i = pinCathode.size() - 1; i >= 0; i--)
		{
			matrix[eventid][i] = pinCathode[i]->istate();
		}
	}
}

// IACTIVEMODEL

VOID ins1Active::initialize(ICOMPONENT* cpt)
{
	this->cpt = cpt;
	this->cpt->setpenwidth(0);
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
	for (size_t a = 0; a < buff.size(); a++)
	{
		for (size_t k = 0; k < buff[a].size(); k++)
		{
			if (buff[a][k] != matrix[a][k])
			{
				buff[a][k] = matrix[a][k];
				cpt->setbrushcolour(ishigh(buff[a][k]) ? YELLOW : BLACK);
				cpt->drawcircle(GRID * (k + 1), -GRID * (buff.size() - a), GRID / 2);
			}
		}
	}
}

BOOL ins1Active::actuate(WORD key, INT x, INT y, DWORD flags)
{
	return FALSE;
}

// Private functions
