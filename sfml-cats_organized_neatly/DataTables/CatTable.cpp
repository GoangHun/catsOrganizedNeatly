#include "stdafx.h"
#include "rapidcsv.h"
#include "CatTable.h"
const std::tuple<std::string, std::string, std::string>& CatTable::Get(int id) const
{
	auto find = table.find(id);
	if (find == table.end())
	{
		std::cout << "ERR: Undefined ID" << std::endl;
	}
	return find->second;
}

bool CatTable::Load()
{

	rapidcsv::Document doc("scripts/cat_info.csv");
	std::vector<int> types = doc.GetColumn<int>(0);
	std::vector<std::tuple<std::string, std::string, std::string>> infos;
	std::vector<std::string> aniPaths = doc.GetColumn<std::string>(1);
	std::vector<std::string> spritePaths = doc.GetColumn<std::string>(2);
	std::vector<std::string> boxInfos = doc.GetColumn<std::string>(3);
	for (int i = 0; i < types.size(); i++)
	{
		infos.push_back({ aniPaths[i], spritePaths[i], boxInfos[i] });
	}

	for (int j = 0; j < types.size(); j++)
	{
		table.insert({ types[j], infos[j] });
	}

	return true;
}

void CatTable::Release()
{
	table.clear();
}