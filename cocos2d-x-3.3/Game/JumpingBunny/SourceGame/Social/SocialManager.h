#ifndef _SOCIAL_MANAGER_H_
#define _SOCIAL_MANAGER_H_

#include "App42/CoreAPI.h"
#include "App42/FacebookProfile.h"
#include "App42/LeaderboardManager.h"
#include "DownloadManager/DownloadHandler.h"

namespace FXSocialService
{
	class SocialManager
	{
	private:
		static SocialManager*	p_Instance;
		DownloadHandler *		p_Downloader;
		FacebookProfile *		p_FBClient;
		LeaderboardManager *	p_leaderB;
		bool					p_IsGetScoreInit;
		bool					p_IsDownloadUserAvatar;
		std::thread 			p_threadGetAsync;
		bool					p_IsThreadFinish;
		//
		bool					p_IsDownloadAllAvatarGlobalPlayer;
		std::vector<std::string> p_GlobalListNamePath;
		std::vector<std::wstring> p_GlobalListDisplayName;
	public:
		SocialManager();
		~SocialManager();
		void InitSocialMgr();
		void Update(float dt);
		static SocialManager* GetInstance() {
			if (!p_Instance)
			{
				p_Instance = new SocialManager();
			}
			return p_Instance;
		}
		//
		void SetProfileFacebook();
		//
		void OnDownloadAvatar();
		//
		void OnDownloadSpecificAvatar(std::string url,std::string name_save);
		//
		FacebookProfile * GetFacebookProfile() {
			return p_FBClient;
		}

		LeaderboardManager * GetLeaderB()
		{
			return p_leaderB;
		}
		//
		bool IsDownloading() { return p_Downloader->GetCurrentStatus() == STATUS_DOWNLOAD::D_DOWNLOADING; }
		//
		void HandleThread(int idxWork = 0, std::string str = "");
		//
		void ShareOnFB(std::string str);
		//
		void DownloadAvatarLeaderbordGlobal();
		//
		void UpdateUserScore(int score);
	};
}
#define SocialMgr SocialManager::GetInstance()
#endif//_SOCIAL_MANAGER_H_