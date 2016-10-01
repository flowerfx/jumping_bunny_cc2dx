#ifndef _BOOST_MANAGER_H_
#define _BOOST_MANAGER_H_
#pragma once
#include "LoadDataXML.h"
struct BoostItem
{
	std::string p_name;
	int p_cost;
	int p_remain; 
	int p_Number;
	std::string DisplayName;
public:
	BoostItem() {
		p_name = "";
		p_cost = 0;
		p_remain = 0;
		p_Number = 0;
		DisplayName = "";
	}
};

static std::string ListItemName[] = { "Item_gravity", "Item_Jump", "Item_life" };
class BoostManager
{
private:
	static BoostManager * m_Instance;
	std::vector<BoostItem*> p_listBoostItem;
	std::vector<std::string> p_listName;
	TiXmlDocument*				 p_ItemXML;
public:
	static BoostManager * GetInstance()
	{
		if (!m_Instance)
		{
			m_Instance = new BoostManager();
		}
		return m_Instance;
	}

	BoostManager();
	~BoostManager();
//
	void InitValue();

	void LoadXMLData();

	BoostItem* GetItem(std::string nameItem);

	int GetItemData(std::string nameItem);
	void SetItemData(std::string nameItem, int number);
protected:
	void OnParseData();

};

#define BoostMgr BoostManager::GetInstance()
#endif //_BOOST_MANAGER_H_