#include "stdafx.h"
#include "DataTableMgr.h"
#include "StringTable.h"
#include "CatTable.h"

void DataTableMgr::LoadAll()
{
	tables.insert({ DataTable::Ids::Cat, new CatTable()});
	for (auto pair : tables)
	{
		if (!pair.second->Load())
		{
			std::cout << "ERR: DATA TABLE LOAD FAIL" << std::endl;
			exit(-1);
		}
	}
}

void DataTableMgr::ReleaseAll()
{
	for (auto pair : tables)
	{
		//pair.second->Release();
		delete pair.second;
	}
	tables.clear();
}
