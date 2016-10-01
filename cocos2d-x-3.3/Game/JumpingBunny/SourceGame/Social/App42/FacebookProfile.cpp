#include "FacebookProfile.h"
#include "Utility/FileManager.h"
#include "FXFlatform/XFlatform.h"

namespace FXSocialService
{
	FacebookProfile::FacebookProfile()
	{
		 p_accessToken = "";
		 p_name = L"";
		 p_ID = "";
		 p_IsGetProfileSuceed = false;
	}
	FacebookProfile::~FacebookProfile()
	{

	}
	void FacebookProfile::InitFacebookProfile(std::wstring name, std::string token, std::string id)
	{
		p_name = name;
		p_accessToken = token;
		p_ID = id;
	}
	std::string FacebookProfile::GetSaveNameFB()
	{
		return "fb:" + p_ID + ".png";
	}

	std::string FacebookProfile::GetNameLocalPath()
	{
		return ResolveLocalNamePath(GetSaveNameFB());
	}

	void FacebookProfile::ShareOnFacebook(std::string str)
	{
		std::wstring wstr = CCUtf8ToUnicode(str.c_str(),(int)str.length());
		XFlatform::FB_SharePageString(wstr);
	}
}