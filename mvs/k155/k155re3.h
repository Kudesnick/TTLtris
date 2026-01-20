#pragma once

#include "StdAfx.h"
#include "vsm.hpp"

#define InfoLog(__s__) inst->log(__s__);

class k155re3 : public IDSIMMODEL
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
	IINSTANCE *inst;
	IDSIMCKT *ckt;
	IDSIMPIN * pin_A[5], * pin_D[8], * pin_V;
	UINT8 memory[32];
	UINT8 get_addr(VOID);
	VOID set_data(ABSTIME time, UINT8 data);
};
