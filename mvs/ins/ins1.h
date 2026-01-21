#pragma once

#include <vector>
#include "StdAfx.h"
#include "vsm.hpp"

#define InfoLog(__s__) inst->log(__s__);

typedef uint16_t row_t;

class ins1Dsim : public IDSIMMODEL
{
public:
	virtual INT isdigital(CHAR* pinname) override;
	virtual VOID setup(IINSTANCE* inst, IDSIMCKT* dsim) override;
	virtual VOID runctrl(RUNMODES mode) override;
	virtual VOID actuate(REALTIME time, ACTIVESTATE newstate) override;
	virtual BOOL indicate(REALTIME time, ACTIVEDATA* data) override;
	virtual VOID simulate(ABSTIME time, DSIMMODES mode) override;
	virtual VOID callback(ABSTIME time, EVENTID eventid) override;
private:
	IINSTANCE* inst = NULL;
	IDSIMCKT* ckt = NULL;
	struct anode {
		IDSIMPIN* pin;
		REALTIME activeTime;
		STATE lastState;
		anode(IDSIMPIN* _pin) : pin(_pin), activeTime(0), lastState(UNDEFINED) {};
	};
	std::vector<anode> pinAnode;
	std::vector<IDSIMPIN*> pinCathode;
	BOOL debug;
};

class ins1Active : public IACTIVEMODEL
{
public:
	virtual VOID initialize(ICOMPONENT* cpt) override;
	virtual ISPICEMODEL* getspicemodel(CHAR* primitive) override;
	virtual IDSIMMODEL* getdsimmodel(CHAR* primitive) override;
	virtual VOID plot(ACTIVESTATE state) override;
	virtual VOID animate(INT element, ACTIVEDATA* newstate) override;
	virtual BOOL actuate(WORD key, INT x, INT y, DWORD flags) override;
private:
	ICOMPONENT* cpt = NULL;
};
