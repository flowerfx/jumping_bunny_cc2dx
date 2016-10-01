#ifndef _FACEBOOK_PROFILE_H_
#define _FACEBOOK_PROFILE_H_

#include "CoreAPI.h"

namespace FXSocialService
{
	class FacebookProfile
	{
	private:
		std::string p_accessToken;
		std::wstring p_name;
		std::string p_ID;
		bool p_IsGetProfileSuceed;
		SocialService * socialService;

	public:
		FacebookProfile();
		~FacebookProfile();

		void InitFacebookProfile(std::wstring name, std::string token, std::string id);
		void ShareOnFacebook(std::string str);

		bool IsGetProfileSuceed() { return p_IsGetProfileSuceed; }
		void IsGetProfileSuceed(bool b) { p_IsGetProfileSuceed = b; }

		std::string AccessToken() { return p_accessToken; }
		void AccessToken(std::string str) { p_accessToken = str; }
		std::wstring NameFacebook() { return p_name; }
		void NameFacebook(std::wstring str) { p_name = str; }
		std::string IDFacebook() { return p_ID; }
		void IDFacebook(std::string str) { p_ID = str; }

		std::string GetNameLocalPath();
		std::string GetSaveNameFB();
	};
}

#endif//_FACEBOOK_PROFILE_H_