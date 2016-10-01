#ifndef _FILE_MANAGER_H_
#define _FILE_MANAGER_H_
#include <string>
#include "cocos2d.h"
USING_NS_CC;
#if defined WP8 || defined W8
#define DATA_DIR "/Data/"
#else
#define DATA_DIR ""
#endif

void Initialize();
FILE * GetFile(std::string namepath, std::string sub);
std::string ResolveNamePath(std::string namepath);
std::string ChangeExtensionName(std::string namepath, std::string ex);
std::string GenerateMipName(std::string namepath);
std::string ResolveLocalNamePath(std::string namepath);
#define SAVE_NAME "Data.sav"
#define SAVE_GAMEPLAY_NAME "Level.sav"

struct DigiData
{
	float data1;
	float data2;
public:
	DigiData() { data1 = 0.f; data2 = 0.f; }
	void SetData(int p)
	{
		data1 = ((float)p / 2.f);
		data2 = data1;
	}
	int GetData()
	{
		return (int)(data2 + data1);
	}
};

class DataSave
{
private:
	DigiData p_Hscore;
	DigiData p_Coin;
	DigiData p_CurPoint;
	DigiData p_CurGravityItem;
	DigiData p_CurLifeItem;
	DigiData p_CurJumpItem;
public:
	DataSave() {
		p_Hscore.SetData(0);
		p_Coin.SetData(0);
		p_CurPoint.SetData(0);
		p_CurGravityItem.SetData(0);
		p_CurLifeItem.SetData(0);
		p_CurJumpItem.SetData(0);
	}

	int GetHScore() { return p_Hscore.GetData(); }
	void SetHScore(int s) { p_Hscore.SetData(s); }

	int GetCoin() { return p_Coin.GetData(); }
	void SetCoin(int s){ p_Coin.SetData(s); }

	int GetCurPoint() { return p_CurPoint.GetData(); }
	void SetCurPoint(int s){ p_CurPoint.SetData(s); }
	//
	int GetGraItem() { return p_CurGravityItem.GetData(); }
	void SetGraItem(int s) { p_CurGravityItem.SetData(s); }

	int GetLifeItem() { return p_CurLifeItem.GetData(); }
	void SetLifeItem(int s){ p_CurLifeItem.SetData(s); }

	int GetJumpItem() { return p_CurJumpItem.GetData(); }
	void SetJumpItem(int s){ p_CurJumpItem.SetData(s); }
};

struct LevelGamePlay 
{
	float timePlay;
	int star_gain;
	std::string name_level;
	LevelGamePlay()
	{
		timePlay = 0;
		star_gain = 0;
		name_level = "";
	}
};

class SaveManager
{
private:
	static SaveManager * m_Instance;
	DataSave *	p_DataSave;
	std::string UDID;
	std::string App42ScoreID;
	std::string DisplayName;
	std::string IsRemoveAds;
	std::string IsSoundEnable;
	std::string p_socialID;
	//
	std::map<std::string, LevelGamePlay *> p_ListLevelGain;
public:
	static SaveManager * GetInstance()
	{
		if (!m_Instance)
		{
			m_Instance = new SaveManager();
		}
		return m_Instance;
	}
	SaveManager();
	~SaveManager();

	DataSave * GetDataSave()
	{
		return p_DataSave;
	}

	LevelGamePlay * GetLevelWithName(std::string name)
	{
		auto it = p_ListLevelGain.find(name);
		if (it != p_ListLevelGain.end())
		{
			return it->second;
		}
		return NULL;
	}

	bool InsertLevelGain(std::string name, int numberLevel, int sec, int star, bool IsIncreasMaxLevel = true);
	void LoadTheLevelFromFile();
	void SaveTheLevelFromFile();

	void SaveTheGameSave();
	void LoadTheGameSave();

	//
	void SetApp42ScoreID(std::string str) { App42ScoreID = str; }
	void SetDisplayName(std::string name){ DisplayName = name; }
	std::string GetDisplayName(){ return DisplayName; }
	std::string GetUDID();
	std::string isRemoveAds() { return IsRemoveAds; }
	void SetRemoveAds(std::string str) { IsRemoveAds = str; }
	std::string isSoundEnable() { return IsSoundEnable; }
	void SetSoundEnable(std::string str) { IsSoundEnable = str; }
	std::string SocialID() { return p_socialID; }
	void SetSocialID(std::string str) { p_socialID = str; }

	int GetMaxLevelReach(){ 
		return (int)p_ListLevelGain.size(); 
	}
protected:
	std::string OnLoadDataSave(bool level = false);
	void OnSaveData(FILE * f, bool level = false);

	std::string CreateBufferSave(bool Reset = false, bool LoadLevl = false);

	std::string OnEngryptString(std::string in);
	std::string OnDegryptString(std::string in);

	bool OnExtractData(std::string str);

	bool LoadLevelFromBufferData(std::string str);

	bool OnResetFileSile();
	void OnGenerateUserName();

};
#define SaveMgr SaveManager::GetInstance()
#endif//_FILE_MANAGER_H_