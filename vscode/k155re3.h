#pragma once

#include "StdAfx.h"
#include "vsm.hpp"

#define InfoLog(__s__) inst->log(__s__);

class k155re3 : public IACTIVEMODEL, public IDSIMMODEL
{
public:
	// IACTIVEMODEL override
	VOID initialize(ICOMPONENT* cpt);
	ISPICEMODEL* getspicemodel(CHAR* primitive);
	IDSIMMODEL* getdsimmodel(CHAR* primitive);
	VOID plot(ACTIVESTATE state);
	VOID animate(INT element, ACTIVEDATA* newstate);
	BOOL actuate(WORD key, INT x, INT y, DWORD flags);
	// IDSIMMODEL override
	INT isdigital (CHAR *pinname);
	VOID setup (IINSTANCE *inst, IDSIMCKT *dsim);
	VOID runctrl (RUNMODES mode);
	VOID actuate (REALTIME time, ACTIVESTATE newstate);
	BOOL indicate (REALTIME time, ACTIVEDATA *data);
	VOID simulate (ABSTIME time, DSIMMODES mode);
	VOID callback (ABSTIME time, EVENTID eventid);
private:
	IINSTANCE *inst;
	IDSIMCKT *ckt;
	IDSIMPIN * pin_A[5], * pin_D[8], * pin_V;
	IDEBUGPOPUP *myPopup;
	UINT8 memory[32];
	UINT8 get_addr(VOID);
	VOID set_data(ABSTIME time, UINT8 data);
};
