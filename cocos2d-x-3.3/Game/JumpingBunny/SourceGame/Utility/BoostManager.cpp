#include "BoostManager.h"
#include "FileManager.h"
#include "Init/DisplayView.h"
#include "Init/ParamTexture.h"
#include "Menu/BoostMenu.h"
BoostManager * BoostManager::m_Instance = NULL;
BoostManager::BoostManager()
{
    p_ItemXML = NULL;
}
BoostManager::~BoostManager()
{
    p_ItemXML = NULL;
}
//
void BoostManager::InitValue()
{
	if (p_listBoostItem.size() > 0)
	{
		for (size_t i = 0; i < p_listBoostItem.size(); i++)
		{
			if (p_listBoostItem.at(i)->p_name == ListItemName[0])
			{
				p_listBoostItem.at(i)->p_remain = SaveMgr->GetDataSave()->GetGraItem();
				//				
			}
			else if (p_listBoostItem.at(i)->p_name == ListItemName[1])
			{
				p_listBoostItem.at(i)->p_remain = SaveMgr->GetDataSave()->GetJumpItem();
			}
			else if (p_listBoostItem.at(i)->p_name == ListItemName[2])
			{
				p_listBoostItem.at(i)->p_remain = SaveMgr->GetDataSave()->GetLifeItem();
			}
		}
	}
	else
	{
		CCASSERT(false, "list item must be loaded before init!");
		return;
	}

	//parse value to boost menu
	for (size_t i = 0; i < p_listBoostItem.size(); i++)
	{
		BoostMenu::GetInstance()->OnParseDetailItem(p_listBoostItem.at(i));
	}

}

void BoostManager::LoadXMLData()
{
	if (!p_ItemXML)
	{
		p_ItemXML = new TiXmlDocument();
	}
#if USE_ARK_ARCHIVE
	if (p_ItemXML->LoadFileArk(std::string(XML_DIR) + std::string(ITEM_XML)) == 0)
	{
		OnParseData();
	}
#else
	FILE * f = GetFile((std::string(XML_DIR) + std::string(ITEM_XML)).c_str(), "rb");
	if (f)
	{
		p_ItemXML->LoadFile(f);
		OnParseData();
		fclose(f);
	}
#endif
}
void BoostManager::OnParseData()
{
	auto root = p_ItemXML->FirstChildElement("GAME");
	if (!root)
	{
		CCASSERT(false, "root data not found !");
		return;
	}
	auto ListBoostItem = root->FirstChildElement("BoostItem");
	if (ListBoostItem)
	{
		auto detail = ListBoostItem->FirstChildElement("Item");
		do
		{
			if (detail)
			{
				auto item = new BoostItem();
				item->p_name = detail->Attribute("name");
				item->p_cost = atoi(detail->Attribute("cost"));
				item->p_Number = atoi(detail->Attribute("number"));
				item->DisplayName = detail->Attribute("displayName");
				p_listName.push_back(item->p_name);
				p_listBoostItem.push_back(item);

				detail = detail->NextSiblingElement();
			}

		} while (detail);
	}
}

BoostItem* BoostManager::GetItem(std::string nameItem)
{
	for (size_t i = 0; i < p_listBoostItem.size(); i++)
	{
		if (p_listBoostItem.at(i)->p_name == nameItem)
		{
			return p_listBoostItem.at(i);
		}
	}
	return NULL;
}

int BoostManager::GetItemData(std::string nameItem)
{
	if (nameItem == ListItemName[0])
	{
		return SaveMgr->GetDataSave()->GetGraItem();
	}
	else if (nameItem == ListItemName[1])
	{
		return SaveMgr->GetDataSave()->GetJumpItem();
	}
	else if (nameItem == ListItemName[2])
	{
		return SaveMgr->GetDataSave()->GetLifeItem();
	}
	return 0;
	CCASSERT(false, "input invalid !");
}
void BoostManager::SetItemData(std::string nameItem, int number)
{
	if (nameItem == ListItemName[0])
	{
		SaveMgr->GetDataSave()->SetGraItem(number);
		for (size_t i = 0; i < p_listBoostItem.size(); i++)
		{
			if (p_listBoostItem.at(i)->p_name == nameItem)
			{
				p_listBoostItem.at(i)->p_remain = number;
				BoostMenu::GetInstance()->OnParseDetailItem(p_listBoostItem.at(i));
				break;
			}
		}
		return;
	}
	else if (nameItem == ListItemName[1])
	{
		SaveMgr->GetDataSave()->SetJumpItem(number);
		for (size_t i = 0; i < p_listBoostItem.size(); i++)
		{
			if (p_listBoostItem.at(i)->p_name == nameItem)
			{
				p_listBoostItem.at(i)->p_remain = number;
				BoostMenu::GetInstance()->OnParseDetailItem(p_listBoostItem.at(i));
				break;
			}
		}
		return;
	}
	else if (nameItem == ListItemName[2])
	{
		SaveMgr->GetDataSave()->SetLifeItem(number);
		for (size_t i = 0; i < p_listBoostItem.size(); i++)
		{
			if (p_listBoostItem.at(i)->p_name == nameItem)
			{
				p_listBoostItem.at(i)->p_remain = number;
				BoostMenu::GetInstance()->OnParseDetailItem(p_listBoostItem.at(i));
				break;
			}
		}
		return;
	}
	CCASSERT(false, "input invalid !");
}