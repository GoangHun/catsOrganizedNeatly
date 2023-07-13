#pragma once

class DataTable
{
public:
	enum class Ids
	{
		None = -1,
		String,
		Zombie,
	};

	DataTable(const DataTable& other) = delete;
	bool operator==(const DataTable& other) const = delete;

protected:
	Ids tableId = Ids::None;

public:
	DataTable(DataTable::Ids id) : tableId(id) {}
	virtual ~DataTable() {}

	virtual bool Load() = 0;
	virtual void Release() = 0;
};

