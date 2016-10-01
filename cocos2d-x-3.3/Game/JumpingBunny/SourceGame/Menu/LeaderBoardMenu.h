#ifndef __LEADERBOARDMENU_H__
#define __LEADERBOARDMENU_H__

#include "Menu/MenuEntity.h"
#include "Menu/MenuManager.h"
#include "Social/SocialManager.h"
class LeaderBoardMenu : public MenuEntity
{
private:
	LeaderBoardMenu();
	~LeaderBoardMenu();
	static LeaderBoardMenu * m_Instance;

	bool p_IsDisplayGlobalScore; //true : display global score, false: display friend score !
	MENULAYER p_MenuToSwitchTo;
	//
	Sprite * p_Avatar;
	std::vector<Sprite *> p_ListAvatarGlobal;
	std::vector<Sprite *> p_ListAvatarFriends;
	unsigned int numberDisplayScore;
public:
	static LeaderBoardMenu * GetInstance()
	{
		if (!m_Instance)
		{
			m_Instance = new LeaderBoardMenu();
		}
		return m_Instance;
	}

	void Init();
	void Update(float dt);
	void Visit(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch = NULL);
	void Draw(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch = NULL);

	void OnFadeIn();
	void OnFadeOut();
	void OnShow();
	void OnHide();

	// a selector callback
	virtual void OnProcessButton(std::string name);
	//get score from data and network
	void OnParseUserScore();
	//display profile fb image
	void DisplayProfileFB(std::wstring name, std::string imgPath, std::string namesource, Point pos, Point sizeDesign, Sprite *(&sp));
	//display profile fb user
	void DisplayProfileFBUser(std::wstring name, std::string imgPath);
	//display list player global score
	void DisplayProfileFBGlobal(std::vector<std::wstring> names, std::vector<std::string> imgPaths);
	void DisplayNameAndScoreGlobal();
	//display list player friends score
	void DisplayProfileFBFriends(std::vector<std::wstring> names, std::vector<std::string> imgPaths);
};
#endif //__LEADERBOARDMENU_H__