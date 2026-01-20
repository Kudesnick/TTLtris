// VSMModel.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "vsm.hpp"
#include "ins1.h"

#define VSMModel_key 0x00000000

extern "C" {
	__declspec (dllexport) IDSIMMODEL* createdsimmodel(CHAR* device, ILICENCESERVER* ils) {
		return NULL;
	}
}

extern "C" {
	__declspec (dllexport) VOID deletedsimmodel(IDSIMMODEL* model) {
		delete (IDSIMMODEL*)model;
	}
}

extern "C" {
	__declspec(dllexport) IACTIVEMODEL* createactivemodel(CHAR* device, ILICENCESERVER* ils) {
		if (ils->authorize(VSMModel_key))
		{
			if (strcmp(device, "INS-1") == 0) return new ins1Active;
		}

		return NULL;
	}
}

extern "C" {
	VOID  __declspec(dllexport) deleteactivemodel(IACTIVEMODEL* model) {
		delete (IACTIVEMODEL*)model;
	}
}
