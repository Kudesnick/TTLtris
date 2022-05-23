// VSMModel.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "vsm.hpp"
#include "k155re3.h"
#include "k155ru5.h"

#define VSMModel_key 0x00000000

extern "C" IDSIMMODEL __declspec (dllexport) *createdsimmodel(CHAR *device, ILICENCESERVER *ils) {
	if (ils->authorize(VSMModel_key))
	{
		if (strcmp(device, "k155re3") == 0) return new k155re3;
		if (strcmp(device, "k155ru5") == 0) return new k155ru5;
	}

	return NULL;
}

extern "C" VOID __declspec (dllexport) deletedsimmodel(IDSIMMODEL *model) {
	delete (k155re3 *)model;
}

// Exported constructor for active component models.
extern "C" IACTIVEMODEL __declspec(dllexport) * createactivemodel(CHAR * device, ILICENCESERVER * ils)
{
	return (IACTIVEMODEL *)createdsimmodel(device, ils);
}

// Exported destructor for active component models.
extern "C" VOID  __declspec(dllexport) deleteactivemodel(IACTIVEMODEL * model)
{
	deletedsimmodel((IDSIMMODEL *)model);
}
