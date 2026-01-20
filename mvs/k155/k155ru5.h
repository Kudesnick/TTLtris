#pragma once

#include "StdAfx.h"
#include "vsm.hpp"

#define InfoLog(__s__) inst->log(__s__);

class k155ru5 : public IDSIMMODEL
{
public:
	virtual INT isdigital (CHAR *pinname) override;
	virtual VOID setup (IINSTANCE *inst, IDSIMCKT *dsim) override;
	virtual VOID runctrl (RUNMODES mode) override;
	virtual VOID actuate (REALTIME time, ACTIVESTATE newstate) override;
	virtual BOOL indicate (REALTIME time, ACTIVEDATA *data) override;
	virtual VOID simulate (ABSTIME time, DSIMMODES mode) override;
	virtual VOID callback (ABSTIME time, EVENTID eventid) override;
private:
	IINSTANCE *inst;
	IDSIMCKT *ckt;
	IDSIMPIN *pin_A[8], *pin_CS1, *pin_CS2, *pin_CS3, *pin_WE, *pin_DI, *pin_DO;
	UINT8 memory[256];
	BOOL debug;
	UINT8 get_addr(VOID);
};
