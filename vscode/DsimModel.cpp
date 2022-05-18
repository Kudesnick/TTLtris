#include <filesystem>
#include <string>
#include <fstream>

#include "StdAfx.h"
#include "DsimModel.h"

using namespace std;

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

	fname = inst->getstrval("file");
	base = inst->getinitval("base");
	if (!fname.empty())
	{
		if (filesystem::exists(fname))
		{
			if (filesystem::path(fname).extension() == ".hex")
			{
				load_hex();
			}
			else
			{
				load_bin();
			}
		}
		else
		{
			inst->warning("File '%s' not exists.", fname.c_str());
		}
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
		result |= (ishigh(pin->istate())) ? 1 << _countof(pin_A) >> 1 : 0;
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

VOID DsimModel::load_bin(VOID)
{
	ifstream f(fname, ios::binary);
	if (f.is_open())
	{
		inst->log("Loading BIN file '%s'.", fname.c_str());

		// Get file size
		f.seekg(0, ios::end);
		auto fsize = f.tellg();

		// Load data
		if (fsize > base)
		{
			f.seekg(base, ios::beg);
			f.read(reinterpret_cast<char*>(memory), sizeof(memory));
		}

		inst->log("%d bytes loaded.", f.gcount());

		f.close();
	}
	else
	{
		inst->error("File '%s' can't opened!", fname.c_str());
	}
}

VOID DsimModel::load_hex(VOID)
{
	ifstream f(fname, ios::in);
	if (f.is_open())
	{
		inst->log("Loading HEX file '%s'.", fname.c_str());

		int addr_h = 0;
		int load_cnt = 0;
		// Reading files
		while (!f.eof())
		{
			string line;
			getline(f, line);

			// Parsing headers
			int len, addr_l, type;
			if (true
				&& sscanf_s(line.c_str(), ":02000004%4x", &addr_h) != 1
				&& sscanf_s(line.c_str(), ":%2x%4x%2x", &len, &addr_l, &type) == 3
				&& type == 0
				)
			{
				// Parse data
				for (auto i = 0; i < len; i++)
				{
					int d;
					if (sscanf_s(line.substr(9 + i * 2).c_str(), "%2x", &d) == 1)
					{
						int real_addr = (addr_h << 16) + addr_l + i - base;

						if (0 <= real_addr && real_addr < sizeof(memory))
						{
							memory[real_addr] = d;
							load_cnt++;
						}
					}
				}
			}
		}

		inst->log("%d bytes loaded.", load_cnt);

		f.close();
	}
	else
	{
		inst->error("File '%s' can't opened!", fname.c_str());
	}
}
