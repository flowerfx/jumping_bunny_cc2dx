#include "FileManager.h"
#include "Init/DisplayView.h"
#ifdef WP8
#include "Direct3DInterop.h"
#include "Windows.h"
#endif
#include <vector>
extern std::vector<std::string> split(const std::string& s, char seperator);
#if defined WP8 || defined W8
std::wstring stows(const std::string& str)
{
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
	wchar_t *buffer = new wchar_t[size_needed];
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, buffer, size_needed);
	std::wstring wstrTo(buffer);
	delete[]buffer;
	return wstrTo;
}

std::string wstos(const std::wstring& wstr)
{
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
	char *buffer = new char[size_needed];
	WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, buffer, size_needed, NULL, NULL);
	std::string strTo(buffer);
	delete[]buffer;
	return strTo;
}

Platform::String ^stops(std::string s)
{
	std::wstring wText = stows(s.c_str());
	return ref new Platform::String(wText.c_str());
}

std::wstring pstows(Platform::String^ ps)
{
	std::wstring wst(ps->Data());
	return wst;
}

std::string pstos(Platform::String^ ps)
{
	std::wstring wst(ps->Data());
	std::string stRet(wst.begin(), wst.end());
	return stRet;
}

Platform::String ^atops(const char *text)
{
	std::wstring wText = stows(text);
	return ref new Platform::String(wText.c_str());
}

void pstoa(Platform::String^ ps, char* a_szOutPut)
{
	std::string stdString = pstos(ps);
	const char* szString = stdString.c_str();
	strcpy(a_szOutPut, szString);
}

char * SOCK_API_STRNEW(const char * string)
{
	if (string == NULL)
	{
		return NULL;
	}
	char * data = NULL;
	int len = strlen(string);

	data = new char[len + 1];
	memset(data, 0, len + 1);
	memcpy(data, string, len);
	return data;
}
#elif defined OS_IOS
#include "Init/../../../RKUtils/Code/RKFile_Code/RKFile.h"

#endif
//

std::string p_gLocalPath;
std::string p_gInstallPath;
const char * SecrectKey = "*@#&$FajJD&@342!";
void Initialize()
{
#if defined WP8 || defined W8
	auto LocalPath = Windows::Storage::ApplicationData::Current->LocalFolder->Path;
	p_gLocalPath = wstos(LocalPath->Data());

	auto InstallPath = Windows::ApplicationModel::Package::Current->InstalledLocation->Path;
	p_gInstallPath = wstos(InstallPath->Data());
#else
	p_gLocalPath = "";
	p_gInstallPath = "";
#endif
}

FILE * GetLocalFile(std::string namepath, std::string sub)
{
	std::string finalPath = p_gLocalPath  +"/"+ namepath;
	return  fopen(finalPath.c_str(), sub.c_str());
}

FILE * GetFile(std::string namepath, std::string sub)
{
	std::string finalPath = p_gInstallPath + DATA_DIR + namepath;
	return fopen(finalPath.c_str(), sub.c_str());
}

std::string ResolveNamePath(std::string namepath)
{
	std::string finalPath = "";
	if (p_gInstallPath != "")
	{
		finalPath = p_gInstallPath + DATA_DIR + namepath;
	}
	else
	{
#ifdef WP8
		auto InstallPath = Windows::ApplicationModel::Package::Current->InstalledLocation->Path;
		p_gInstallPath = wstos(InstallPath->Data());
#elif defined OS_IOS
        auto fullPath = cocos2d::FileUtils::getInstance()->fullPathForFilename(namepath);
        return fullPath;
#endif
		finalPath = p_gInstallPath + DATA_DIR + namepath;
	}

	return finalPath;
}

std::string ResolveLocalNamePath(std::string namepath)
{
    std::string finalPath = "";
    if (p_gLocalPath != "")
    {
#ifdef WP8
        finalPath = p_gLocalPath + "/" + namepath;
#elif defined OS_IOS
         finalPath = p_gLocalPath + namepath;
#endif
    }
    else
    {
#ifdef WP8
        auto InstallPath = Windows::Storage::ApplicationData::Current->LocalFolder->Path;
        p_gLocalPath = wstos(InstallPath->Data());
        finalPath = p_gLocalPath + "/" + namepath;
#elif defined OS_IOS
        char * p_LocalPath = NULL;
        p_LocalPath = (char*)RKUtils::RKFile_GetDocumentsPath();
        p_gLocalPath = p_LocalPath;
        finalPath = p_gLocalPath + namepath;
#endif
    }
	return finalPath;
}

std::string GenerateMipName(std::string namepath)
{
	auto strs = split(namepath, '.');
	return strs[0] + "_mip." + strs[1];
}

std::string ChangeExtensionName(std::string namepath, std::string ex)
{
	auto strs = split(namepath, '.');
	std::string re_string = "";
	re_string = strs[0] + ex;
	return re_string;
}

SaveManager * SaveManager::m_Instance = NULL;

SaveManager::SaveManager()
{
	p_DataSave = new DataSave();
	UDID = "";
	App42ScoreID = "";
	DisplayName = "";
	IsRemoveAds = "0";
	IsSoundEnable = "1";
	p_socialID = "";

}
SaveManager::~SaveManager()
{
	delete p_DataSave;
	p_DataSave = NULL;
}
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
void SaveManager::SaveTheGameSave()
{
#if defined W8 || defined WP8
	FILE * f = GetLocalFile(SAVE_NAME, "wb");
#else
	FILE * f = GetFile(SAVE_NAME, "wb");
#endif
	if (!f)
	{
		CCASSERT(false, "nodata to save!");
		return;
	}
	OnSaveData(f);
	fclose(f);
}
void SaveManager::LoadTheGameSave()
{
	auto str = OnLoadDataSave();
	if (str != "")
	{
		bool res = OnExtractData(str);
		if (!res)
		{
			//reset file save if save file is valid;
			FILE * f = GetFile(SAVE_NAME, "wb");
			std::string buffer = CreateBufferSave(true);
			std::string engrypt_buffer = OnEngryptString(buffer);
			fputs(engrypt_buffer.c_str(), f);
			fclose(f);
		}
	}
}
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
std::string SaveManager::OnLoadDataSave(bool level)
{
	int file_exist = 1;
#if defined W8 || defined WP8
	FILE * f = GetLocalFile(!level ? SAVE_NAME : SAVE_GAMEPLAY_NAME, "rb");
#else
	FILE * f = GetFile(SAVE_NAME, "rb");
#endif
	if (!f)
	{
#if defined W8 || defined WP8
		f = GetLocalFile(!level ? SAVE_NAME : SAVE_GAMEPLAY_NAME, "wb");
#else
		f = GetFile(SAVE_NAME, "wb");
#endif
		file_exist = 0;
	}
	if (!f)
	{
#ifndef OS_IOS
		CCASSERT(false, "error in load file save");
#endif
		return "";
	}
	//
	char * buffer_file = new char[1024];
	std::string buffer_data = "";
	if (file_exist == 0)
	{
		if (!level)
		{
			p_DataSave->SetCoin(60);
		}
		OnSaveData(f, level);
	}
	else
	{
		if (fgets(buffer_file, 1024, f))
		{
			buffer_data = OnDegryptString(std::string(buffer_file));
		}
	}
	//
	fclose(f);
	return buffer_data;

}
void SaveManager::OnSaveData(FILE * f, bool level)
{
	std::string buffer = CreateBufferSave(false,level);
	std::string engrypt_buffer = OnEngryptString(buffer);
	fputs(engrypt_buffer.c_str(), f);

}
std::string SaveManager::OnEngryptString(std::string in)
{
	std::string out = "";
	int idx = 0;
	int sizeOfSecrect = sizeof(SecrectKey) / sizeof(char);
	for (int i = 0; i < in.size(); i++)
	{
		char temp = (in[i] + SecrectKey[idx]);
		out += temp;
		idx++;
		if (idx >= sizeOfSecrect)
		{
			idx = 0;
		}
	}

	return out;
}
std::string SaveManager::OnDegryptString(std::string in)
{
	std::string out = "";
	int idx = 0;
	int sizeOfSecrect = sizeof(SecrectKey) / sizeof(char);
	for (int i = 0; i < in.size(); i++)
	{
		char temp = (in[i] - SecrectKey[idx]);
		out += temp;
		idx++;
		if (idx >= sizeOfSecrect)
		{
			idx = 0;
		}
	}

	return out;
}
std::string SaveManager::CreateBufferSave(bool Reset, bool LoadLevel)
{
	OnGenerateUserName();
	//
	std::string buffer_write_to = "";

	buffer_write_to += "Device Type:";
#if defined W8 || defined WP8
	buffer_write_to += "WP8";
#elif defined WIN32
	buffer_write_to += "WIN32";
#elif defined IOS
	buffer_write_to += "IOS";
#elif defined ANDROID
	buffer_write_to += "ANDROID";
#endif
	buffer_write_to += "#";
	buffer_write_to += "DeviceID:";
	buffer_write_to += GetUDID();
	if (LoadLevel)
	{
		buffer_write_to += "#";
		for (int i = 0; i < p_ListLevelGain.size(); i++)
		{
			auto lvl = GetLevelWithName(std::string("part_") + std::to_string(i));
			if (lvl)
			{
				buffer_write_to += lvl->name_level;
				buffer_write_to += "|";
				buffer_write_to += std::to_string(lvl->timePlay);
				buffer_write_to += "|";
				buffer_write_to += std::to_string(lvl->star_gain);
				buffer_write_to += "+";
			}
		}
	}
	else
	{
		buffer_write_to += "#";
		buffer_write_to += "DisplayName:";
		buffer_write_to += DisplayName;
		buffer_write_to += "#";
		buffer_write_to += "ScoreID:";
		buffer_write_to += App42ScoreID;
		buffer_write_to += "#";
		buffer_write_to += "HScore:";
		buffer_write_to += std::to_string(Reset ? 0 : p_DataSave->GetHScore());
		buffer_write_to += "#";
		buffer_write_to += "Coin:";
		buffer_write_to += std::to_string(Reset ? 0 : p_DataSave->GetCoin());
		buffer_write_to += "#";
		buffer_write_to += "GraveItem:";
		buffer_write_to += std::to_string(Reset ? 0 : p_DataSave->GetGraItem());
		buffer_write_to += "#";
		buffer_write_to += "JumpItem:";
		buffer_write_to += std::to_string(Reset ? 0 : p_DataSave->GetJumpItem());
		buffer_write_to += "#";
		buffer_write_to += "LifeItem:";
		buffer_write_to += std::to_string(Reset ? 0 : p_DataSave->GetLifeItem());
		buffer_write_to += "#IsRemoveAds:";
		buffer_write_to += Reset ? "0" : IsRemoveAds;
		buffer_write_to += "#IsSoundEnable:";
		buffer_write_to += Reset ? "1" : IsSoundEnable;
		buffer_write_to += "#SocialID:";
		buffer_write_to += Reset ? "" : p_socialID;
	}


	return buffer_write_to;
}
void SaveManager::OnGenerateUserName()
{
	if (DisplayName != "")
		return;
	std::string name = "";
	int random = GetRandom(0, 10);
	if (random == 0)
	{
		name = "User_";
	}
	else if (random == 1)
	{
		name = "Name_";
	}
	else if (random == 2)
	{
		name = "Tag_";
	}
	else if (random == 3)
	{
		name = "Player_";
	}
	else if (random == 4)
	{
		name = "Guy_";
	}
	else if (random == 5)
	{
		name = "Gamer_";
	}
	else if (random == 6)
	{
		name = "NameTag_";
	}
	else if (random == 7)
	{
		name = "Gosu_";
	}
	else if (random == 8)
	{
		name = "Handsome_";
	}
	else if (random == 9)
	{
		name = "Bunny_";
	}
	else if (random == 10)
	{
		name = "Jumper_";
	}
	char postfix[5];
	std::string p_UDID = GetUDID();
	postfix[0] = p_UDID.at(0); postfix[1] = p_UDID.at(1); postfix[2] = p_UDID.at(2); postfix[3] = p_UDID.at(3); postfix[4] = '\0';
	std::string p_postfix = std::string(postfix);
	name.append(p_postfix);
	//
	DisplayName = name;
}
bool SaveManager::OnExtractData(std::string str)
{
	bool res = true;

	auto ListString = split(str, '#');
	for (int i = 0; i < ListString.size(); i++)
	{
		auto detailStr = split(ListString[i], ':');
		if (detailStr[0] == "Device Type")
		{
			std::string type = "";
#if defined W8 || defined WP8
			type= "WP8";
#elif defined WIN32
			type= "WIN32";
#elif defined IOS
			type= "IOS";
#elif defined ANDROID
			type = "ANDROID";
#endif
			if (type != detailStr[1])
			{
				return false;
			}
		}
		else if (detailStr[0] == "DeviceID")
		{
			std::string id = detailStr[1];
			if (id != GetUDID())
			{
				return false;
			}
		}
		else if (detailStr[0] == "DisplayName")
		{
			DisplayName = detailStr[1];
		}
		else if (detailStr[0] == "ScoreID:")
		{
			App42ScoreID = detailStr[1];
		}
		else if (detailStr[0] == "Hash")
		{
			std::string hash = detailStr[1];
		}
		else if (detailStr[0] == "HScore")
		{
			std::string str_d = detailStr[1];
			p_DataSave->SetHScore(atoi(str_d.c_str()));
		}
		else if (detailStr[0] == "Coin")
		{
			std::string str_d = detailStr[1];
			p_DataSave->SetCoin(atoi(str_d.c_str()));
		}
		else if (detailStr[0] == "GraveItem")
		{
			std::string str_d = detailStr[1];
			p_DataSave->SetGraItem(atoi(str_d.c_str()));
		}
		else if (detailStr[0] == "JumpItem")
		{
			std::string str_d = detailStr[1];
			p_DataSave->SetJumpItem(atoi(str_d.c_str()));
		}
		else if (detailStr[0] == "LifeItem")
		{
			std::string str_d = detailStr[1];
			p_DataSave->SetLifeItem(atoi(str_d.c_str()));
		}
		else if (detailStr[0] == "IsRemoveAds")
		{
			IsRemoveAds = detailStr[1];			
		}
		else if (detailStr[0] == "IsSoundEnable")
		{
			IsSoundEnable = detailStr[1];
		}
		else if (detailStr[0] == "SocialID")
		{
			p_socialID = detailStr[1];
		}
	}

	return res;
}
std::string SaveManager::GetUDID()
{
#if defined WP8 
	auto str = std::string(CCUnicodeToUtf8(Direct3DInterop::Get_DeviceID()->Data()));
	return str;
#else
	return "";
#endif
}
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
bool SaveManager::InsertLevelGain(std::string name, int numberLevel, int sec, int star, bool IsIncreMax)
{
	if (numberLevel < p_ListLevelGain.size())
		return false;
	if (GetLevelWithName(name) != NULL)
		return false;
	LevelGamePlay * lvlgp = new LevelGamePlay();
	lvlgp->name_level = name;
	lvlgp->timePlay = sec;
	lvlgp->star_gain = star;
	p_ListLevelGain.insert(std::pair<std::string, LevelGamePlay*>(name, lvlgp));
	return true;
}

bool SaveManager::LoadLevelFromBufferData(std::string str)
{
	bool res = true;

	auto ListString = split(str, '#');
	auto detailStr = split(ListString[0], ':');
	if (detailStr[0] == "Device Type")
	{
		std::string type = "";
#if defined W8 || defined WP8
		type = "WP8";
#elif defined WIN32
		type = "WIN32";
#elif defined IOS
		type = "IOS";
#elif defined ANDROID
		type = "ANDROID";
#endif
		if (type != detailStr[1])
		{
			return false;
		}
	}
	detailStr = split(ListString[1], ':');
	if (detailStr[0] == "DeviceID")
	{
		std::string id = detailStr[1];
		if (id != GetUDID())
		{
			return false;
		}
	}

	//
	auto listlevel = split(ListString[2], '+');
	if (listlevel.size() > 0)
	{
		for (int i = 0; i < listlevel.size(); i++)
		{
			if (listlevel[i] != "")
			{
				auto detaillevel = split(listlevel[i], '|');
				bool res = InsertLevelGain(detaillevel[0], i, atoi(detaillevel[1].c_str()), atoi(detaillevel[2].c_str()));
				if (!res)
				{
					return false;
				}
			}
		}
	}
	return res;

}

void SaveManager::LoadTheLevelFromFile()
{
	auto str = OnLoadDataSave(true);
	if (str != "")
	{
		bool res = LoadLevelFromBufferData(str);
		if (!res)
		{
			//reset file save if save file is valid;
			FILE * f = GetFile(SAVE_GAMEPLAY_NAME, "wb");
			std::string buffer = CreateBufferSave(true,true);
			std::string engrypt_buffer = OnEngryptString(buffer);
			fputs(engrypt_buffer.c_str(), f);
			fclose(f);
		}
	}
}
void SaveManager::SaveTheLevelFromFile()
{
#if defined W8 || defined WP8
	FILE * f = GetLocalFile(SAVE_GAMEPLAY_NAME, "wb");
#else
	FILE * f = GetFile(SAVE_NAME, "wb");
#endif
	if (!f)
	{
		CCASSERT(false, "nodata to save!");
		return;
	}
	OnSaveData(f,true);
	fclose(f);
}