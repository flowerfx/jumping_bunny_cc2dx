#include "SocialManager.h"
#include "Utility/FileManager.h"
#include "FXFlatform/XFlatform.h"
#include "Menu/MainMenu.h"
#include "Menu/LeaderBoardMenu.h"

#include <mutex>          // std::mutex

namespace FXSocialService
{
	std::mutex mtx;           // mutex for critical section
	//static std::mutex       LoadAyncMutex;
	SocialManager* SocialManager::p_Instance = NULL;
	SocialManager::SocialManager()
	{
		p_IsGetScoreInit = false;
		p_IsThreadFinish = false;
		p_IsDownloadUserAvatar = false;
		p_IsDownloadAllAvatarGlobalPlayer = false;
	}
	SocialManager::~SocialManager()
	{
		delete p_FBClient;
		p_FBClient = NULL;
		delete p_Downloader;
		p_Downloader = NULL;
	}
	void SocialManager::InitSocialMgr()
	{
		p_FBClient = new FacebookProfile();
		p_Downloader = new DownloadHandler();
		p_leaderB = new LeaderboardManager();
		p_leaderB->InitLeaderBoard();
		//set the default username display on save data
		std::string namedisplay = SaveMgr->GetDisplayName();
		//get device udid;
		std::string udid = SaveMgr->GetUDID();
		p_leaderB->CurrentUserScore(CCUtf8ToUnicode(namedisplay.data(), (int)namedisplay.length()), (unsigned int)MenuMgr->GetHScore(), "", udid);
		MainMenu::GetInstance()->DisplayProfileFB(CCUtf8ToUnicode(namedisplay.data(), (int)namedisplay.length()), "");
		//get the global leaderboard
		p_leaderB->App42GetTopScorePlayer();
	}
	void SocialManager::Update(float dt)
	{
		//check the download complete
		if (p_Downloader && p_Downloader->GetCurrentStatus() == STATUS_DOWNLOAD::D_SUCCESS)
		{
			p_Downloader->ResetStatus();
			//
			if (p_IsDownloadUserAvatar)
			{
				MainMenu::GetInstance()->DisplayProfileFB(p_FBClient->NameFacebook(), p_FBClient->GetNameLocalPath());
				//
				LeaderBoardMenu::GetInstance()->DisplayProfileFBUser(p_FBClient->NameFacebook(), p_FBClient->GetNameLocalPath());
				//
				p_IsDownloadUserAvatar = false;
			}
		}
		//check the download all avatar of global player complete
		if (p_IsDownloadAllAvatarGlobalPlayer)
		{
			//after download complete just parse into the leaderboard menu
			LeaderBoardMenu::GetInstance()->DisplayProfileFBGlobal(p_GlobalListDisplayName, p_GlobalListNamePath);
			p_IsDownloadAllAvatarGlobalPlayer = false;
		}
		//check the get profile fb
		if (p_FBClient && !p_FBClient->IsGetProfileSuceed())
		{
			SetProfileFacebook();
		}
		//
		if (!p_IsGetScoreInit && p_leaderB->GetUserScoreFB() != "")
		{
			size_t score = (size_t)atoi(p_leaderB->GetUserScoreFB().c_str());
			//
			if (p_leaderB->IsChangeUserAccount()) //user change account
			{
				UpdateUserScore((int)score);
			}
			else // user not change account
			{
				if (score > p_leaderB->CurrentUserScore()->Score)
				{
					UpdateUserScore((int)score);
				}
				else if (score < p_leaderB->CurrentUserScore()->Score)
				{
					//the score fb is lower than the score store on game, maybe cause the connection corrupt, send the new score			
					if (XFlatform::IsHaveInternetConnection())
					{
						XFlatform::FB_UpdateUserScore(p_leaderB->CurrentUserScore()->Score);
					}
				}
			}
			//
			p_IsGetScoreInit = true;
		}
		//
		if (p_IsThreadFinish)
		{
			p_threadGetAsync.join();
			p_IsThreadFinish = false;
		}
	}

	void SocialManager::UpdateUserScore(int score)
	{
		//get device udid;
		std::string udid = SaveMgr->GetUDID();
		//update score to leaderbord
		p_leaderB->CurrentUserScore(p_FBClient->NameFacebook(), score, p_FBClient->IDFacebook(), udid);
		//update score global
		MenuMgr->SetHScore(score);
		//save the game
		SaveManager::GetInstance()->SaveTheGameSave();
		//
		LeaderBoardMenu::GetInstance()->OnParseUserScore();
	}

	void SocialManager::SetProfileFacebook()
	{
		std::wstring nameFB = XFlatform::FB_GetUserProfile("name");
		std::wstring t = XFlatform::FB_GetUserProfile("id");
		std::string ID = std::string(t.begin(), t.end());
		t = XFlatform::FB_GetUserProfile("token");
		std::string token = std::string(t.begin(), t.end());
		t = XFlatform::FB_GetUserProfile("imgUrl");
		std::string imageURL = std::string(t.begin(), t.end());
		//
		if (p_FBClient && nameFB != L"" && ID != "" && token != "")
		{
			//get device udid;
			std::string udid = SaveMgr->GetUDID();
			p_FBClient->InitFacebookProfile(nameFB, token, ID);
			p_FBClient->IsGetProfileSuceed(true);
			//
			SaveMgr->SetDisplayName(std::string(CCUnicodeToUtf8(nameFB.data())));
			SaveMgr->SaveTheGameSave();
			//download avatar
			OnDownloadAvatar();
			//get score from facebook
			p_IsThreadFinish = false;
			p_threadGetAsync = std::thread(&SocialManager::HandleThread,this, 0,"");
			//update the save score to leaderboard
			p_leaderB->CurrentUserScore(nameFB, (unsigned int)MenuMgr->GetHScore(), ID, udid);
		}
	}
	void SocialManager::OnDownloadAvatar()
	{
		std::wstring t = XFlatform::FB_GetUserProfile("imgUrl");
		std::string imageURL = std::string(t.begin(), t.end());
		if (p_Downloader && p_Downloader->GetCurrentStatus() == STATUS_DOWNLOAD::D_NONE)
		{
			p_Downloader->downLoadObject(imageURL, p_FBClient->GetSaveNameFB());
		}
		p_IsDownloadUserAvatar = true;
	}
	void SocialManager::OnDownloadSpecificAvatar(std::string url, std::string name_save)
	{
		if (p_Downloader && p_Downloader->GetCurrentStatus() == STATUS_DOWNLOAD::D_NONE)
		{
			p_Downloader->downLoadObject(url, name_save);
		}
	}
	//
	void SocialManager::HandleThread(int idxWork, std::string str)
	{
		if (idxWork == 0)
		{			
			p_leaderB->RequestScoreFromFB();
		}
		else if (idxWork == 1)
		{
			p_FBClient->ShareOnFacebook(str);
		}
		else if (idxWork == 2)
		{

			for (int i = 0; i < p_leaderB->GetTopPlayerScores().size(); i++)
			{
				if (p_leaderB->GetTopPlayerScores().at(i)->id_fb == "_" || p_leaderB->GetTopPlayerScores().at(i)->id_fb == "")
					continue;
				std::wstring w_id = CCUtf8ToUnicode(p_leaderB->GetTopPlayerScores().at(i)->id_fb.data(), (int)p_leaderB->GetTopPlayerScores().at(i)->id_fb.length());
				std::wstring wurl = XFlatform::FB_GetProfileImgURLFromID(w_id);
				std::string url = std::string(CCUnicodeToUtf8(wurl.data()));
				OnDownloadSpecificAvatar(url, str + p_leaderB->GetTopPlayerScores().at(i)->id_fb + ".png");
				//
				p_GlobalListNamePath.push_back(ResolveLocalNamePath(str + p_leaderB->GetTopPlayerScores().at(i)->id_fb + ".png"));
				p_GlobalListDisplayName.push_back(p_leaderB->GetTopPlayerScores().at(i)->Name);

			}
			p_IsDownloadAllAvatarGlobalPlayer = true;
		}
		p_IsThreadFinish = true;
	}
	//
	void SocialManager::ShareOnFB(std::string str)
	{
		p_IsThreadFinish = false;
		p_threadGetAsync = std::thread(&SocialManager::HandleThread, this, 1,str);
	}
	//
	void SocialManager::DownloadAvatarLeaderbordGlobal()
	{
		p_IsThreadFinish = false;
		p_threadGetAsync = std::thread(&SocialManager::HandleThread, this, 2, "global:");
	}
}