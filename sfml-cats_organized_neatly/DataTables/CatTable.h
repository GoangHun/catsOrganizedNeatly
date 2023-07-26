#pragma once
#include "DataTable.h"
class CatTable :
    public DataTable
{
protected:
	std::unordered_map<int, std::tuple<std::string, std::string, std::string>>  table;

public:
	CatTable() : DataTable(DataTable::Ids::Cat), table((int)CatTypes::COUNT) {}
	virtual ~CatTable() override { Release(); }

	virtual bool Load() override;
	virtual void Release() override;

	const std::tuple<std::string, std::string, std::string>& Get(int id) const;


};

