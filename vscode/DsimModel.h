#pragma once

#include <string>
#include "StdAfx.h"
#include "sdk/vsm.hpp"

using namespace std;

#define InfoLog(__s__) inst->log(__s__);

class DsimModel : public IDSIMMODEL
{
public:
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
	string fname;
	INT base;
	UINT8 get_addr(VOID);
	VOID set_data(ABSTIME time, UINT8 data);
	VOID load_bin(VOID);
	VOID load_hex(VOID);
};