#ifndef _LEADERBOARD_MANAGER_H_
#define _LEADERBOARD_MANAGER_H_

#include "CoreAPI.h"

namespace FXSocialService
{
	struct ScoreType
	{
		unsigned int Score;
		std::string id_fb;
		std::wstring Name;
		std::string id_device;
		std::string id_score;
		ScoreType()
		{
			Score = 0;
			Name = L"";
			id_fb = "";
			id_device = "";
			id_score = "";
		}
		~ScoreType()
		{
			Score = 0;
			Name = L"";
			id_fb = "";
			id_device = "";
			id_score = "";
		}
	};
	class LeaderboardManager
	{
	private:
		ScoreType * p_currentUserScore;
		std::vector<ScoreType* > p_TopPlayerScores;
		int p_maxListPlayer;
		ScoreBoardService * p_App42ScoreBoardService;
		std::string p_App42ScoreUserID;
		bool p_IsChangeUserAccount;
	public:
		bool IsChangeUserAccount() { return p_IsChangeUserAccount; }
		void ResetChangeUserAccount() { p_IsChangeUserAccount = false; }
		//
		ScoreType * CurrentUserScore() { return p_currentUserScore; }
		void	  CurrentUserScore(ScoreType* score){ p_currentUserScore = score; }
		void	  CurrentUserScore(std::wstring name, unsigned int score, std::string id, std::string udid);
		std::vector<ScoreType* > GetTopPlayerScores() { return p_TopPlayerScores; }

		void UpdateScoreUserToFB();
		void RequestScoreFromFB();
		std::string GetUserScoreFB();
		void UpdateHighScoreUserToLB(unsigned int score);

		void App42GetTopScorePlayer();
		void App42OnGetTopScorePlayerComplete(void *response);
		void App42OnUpdateUserHightScore(int score);
		void App42OnUpdateUserHightScoreComplete(void *response);
		void App42OnEditUserHightScore(int score);
		void App42OnEditUserHightScoreComplete(void *response);

		bool IsUserScoreInTop(int score);
		bool IsUserExistInTop(int score);
		void UpdateScore(int score);
		void UpdateScoreBoard();
		void InitLeaderBoard();
		LeaderboardManager();
		~LeaderboardManager();
	};
}
#endif//_LEADERBOARD_MANAGER_H_