#include "LeaderboardManager.h"
#include "Utility/FileManager.h"
#include "FXFlatform/XFlatform.h"
#include "Social/SocialManager.h"
extern std::vector<std::string> split(const std::string& s, char seperator);
namespace FXSocialService
{
	LeaderboardManager::LeaderboardManager()
	{
		p_maxListPlayer = 0;
		p_IsChangeUserAccount = false;
	}

	LeaderboardManager::~LeaderboardManager()
	{
		delete p_currentUserScore;
		p_currentUserScore = NULL;
		delete p_App42ScoreBoardService;
		p_App42ScoreBoardService = NULL;
		for (int i = 0; i < p_TopPlayerScores.size(); i++)
		{
			delete p_TopPlayerScores.at(i);
			p_TopPlayerScores.at(i) = NULL;
		}
		p_TopPlayerScores.clear();
	}

	void LeaderboardManager::InitLeaderBoard()
	{
		p_maxListPlayer = 4;
		p_currentUserScore = new ScoreType();
		App42API::Initialize(API_KEY, SECRET_KEY);
		p_App42ScoreBoardService = App42API::BuildScoreBoardService();
	}

	void LeaderboardManager::UpdateScoreUserToFB()
	{
		if (p_currentUserScore && p_currentUserScore->id_fb != "" && p_currentUserScore->id_fb != "_")
		{
			XFlatform::FB_UpdateUserScore(p_currentUserScore->Score);
		}
	}

	void LeaderboardManager::RequestScoreFromFB()
	{
		XFlatform::FB_RequestUserScore();
	}

	std::string LeaderboardManager::GetUserScoreFB()
	{
		std::string score = XFlatform::FB_GetUserScore();
		return score;
	}

	void LeaderboardManager::UpdateHighScoreUserToLB(unsigned int score)
	{
		if (score > p_currentUserScore->Score && p_currentUserScore->id_fb != "" && p_currentUserScore->id_fb != "_")
		{
			p_currentUserScore->Score = score;
			//
			if (!XFlatform::IsHaveInternetConnection())
			{
				return;
			}
			UpdateScoreUserToFB();
		}
	}

	void LeaderboardManager::App42GetTopScorePlayer()
	{
		p_App42ScoreBoardService->GetTopNRankers(NAME_SERVICE.c_str(), p_maxListPlayer, CC_CALLBACK_1(LeaderboardManager::App42OnGetTopScorePlayerComplete, this));
	}

	void LeaderboardManager::App42OnGetTopScorePlayerComplete(void * response)
	{
		App42GameResponse *scoreResponse = (App42GameResponse*)response;
		if (scoreResponse->isSuccess)
		{
			for (std::vector<App42Score>::iterator it = scoreResponse->scores.begin(); it != scoreResponse->scores.end(); ++it)
			{
				ScoreType* type = new ScoreType();
				auto strs = split(it->getUserName(), '#');
				type->Name = CCUtf8ToUnicode(strs[0].c_str(), (int)strs[0].length());
				type->Score = it->getScoreValue();
				type->id_fb = strs[1].c_str();
				type->id_device = strs[3].c_str();
				type->id_score = it->getScoreId();
				if (type->id_device == SaveMgr->GetUDID() && GetPlatForm() == strs[2] && p_App42ScoreUserID == "")
				{
					p_App42ScoreUserID = it->getScoreId();
				}
				p_TopPlayerScores.push_back(type);
			}
			//
			SocialMgr->DownloadAvatarLeaderbordGlobal();
		}
	}

	void LeaderboardManager::App42OnUpdateUserHightScore(int score)
	{
		std::string fid = p_currentUserScore->id_fb;
		if (fid == "")
			fid = "_";
        std::string nameSaveApp42 = std::string(CCUnicodeToUtf8(p_currentUserScore->Name.data())) + "#" + fid + "#" + GetPlatForm() + "#" + SaveMgr->GetUDID();
		p_App42ScoreBoardService->SaveUserScore(NAME_SERVICE.c_str(), nameSaveApp42.c_str(), score, CC_CALLBACK_1(LeaderboardManager::App42OnUpdateUserHightScoreComplete, this));
	}

	void LeaderboardManager::App42OnUpdateUserHightScoreComplete(void *response)
	{
		App42GameResponse *scoreResponse = (App42GameResponse*)response;
		if (scoreResponse->isSuccess)
		{			
			//for (std::vector<App42Score>::iterator it = scoreResponse->scores.begin(); it != scoreResponse->scores.end(); ++it)
			//{

			//}
			//get the highest score;
			p_App42ScoreUserID = scoreResponse->scores.begin()->getScoreId();
			//save the score id;
			SaveMgr->SetApp42ScoreID(p_App42ScoreUserID);
			//save the game;
			SaveMgr->SaveTheGameSave();
		}
	}

	void LeaderboardManager::App42OnEditUserHightScore(int score)
	{
		if (p_App42ScoreUserID == "")
		{
			return;
		}
		p_App42ScoreBoardService->EditScoreValueById(p_App42ScoreUserID.c_str(), score, CC_CALLBACK_1(LeaderboardManager::App42OnEditUserHightScoreComplete, this));
	}

	void LeaderboardManager::App42OnEditUserHightScoreComplete(void *response)
	{
		App42GameResponse *scoreResponse = (App42GameResponse*)response;
		if (scoreResponse->isSuccess)
		{
			//reupdate the game leaderboard
		}
	}

	bool LeaderboardManager::IsUserScoreInTop(int score)
	{
		if (p_TopPlayerScores.size() == p_maxListPlayer)
		{
			if (score > p_TopPlayerScores.at(p_TopPlayerScores.size() - 1)->Score)
			{
				return true;
			}
			return false;
		}
		else
		{
			return true;
		}
	}
	void LeaderboardManager::UpdateScoreBoard()
	{
		unsigned int rank = 0;
		if (p_TopPlayerScores.size() == p_maxListPlayer)
		{
			for (int i = (int)p_TopPlayerScores.size() - 1; i >= 0; i--)
			{
				if (p_currentUserScore->Score < p_TopPlayerScores.at(i)->Score)
				{
					rank = i + 1;
					break;
				}
			}

			p_TopPlayerScores.insert(p_TopPlayerScores.begin() + rank , p_currentUserScore);
			p_TopPlayerScores.erase(p_TopPlayerScores.begin() + 4);
		}
		else
		{
			p_TopPlayerScores.push_back(p_currentUserScore);
		}
	}
	bool LeaderboardManager::IsUserExistInTop(int score)
	{
		if (p_TopPlayerScores.size() == 0 || p_App42ScoreUserID == "")
		{
			return true;
		}
		for (int i = 0; i < p_TopPlayerScores.size(); i++)
		{
			if (p_App42ScoreUserID != "" && p_App42ScoreUserID == p_TopPlayerScores.at(i)->id_score)
			{
				if (score > p_TopPlayerScores.at(i)->Score)
				{
					return true;
				}
				else
				{
					return false;
					//update the name
				}
			}
		}
		return false;
	}
	void LeaderboardManager::UpdateScore(int score)
	{
		//check the user on the top
		if (!IsUserScoreInTop(score))
		{
			return;
		}
		//check if user already exist on the top, so update the name and score if higher
		if (!IsUserExistInTop(score))
		{
			return;
		}
		p_currentUserScore->Score = score;
		//
		UpdateScoreBoard();
		//
		if (p_App42ScoreUserID == "")
		{
			App42OnUpdateUserHightScore(score);
		}
		else
		{
			App42OnEditUserHightScore(score);
		}
	}

	void LeaderboardManager::CurrentUserScore(std::wstring name, unsigned int score, std::string id, std::string udid)
	{
		p_currentUserScore->Name = name;
		p_currentUserScore->Score = score;
		p_currentUserScore->id_fb = id;
		if (SaveMgr->SocialID() != id && id != "")
		{
			if (SaveMgr->SocialID() != "")
			{
				//we have store a social id // mean the user change the social account to play 
				p_IsChangeUserAccount = true;
				//
			}
			//save the id
			SaveMgr->SetSocialID(id);
			SaveMgr->SaveTheGameSave();

		}
		p_currentUserScore->id_device = udid;
	}
}