#ifndef __GAMEPLAYSELECTION_H__
#define __GAMEPLAYSELECTION_H__

#include "Menu/MenuEntity.h"
#include "Menu/MenuManager.h"

class GamePlaySelection : public MenuEntity
{
private:
	static GamePlaySelection * m_Instance;

	MENULAYER p_MenuToSwitchTo;
	int idxFbBtn;
	std::thread p_threadCreateGame;
	bool IsFinishCreate;
public:
	GamePlaySelection();
	~GamePlaySelection();
	static GamePlaySelection * GetInstance()
	{
		if (!m_Instance)
		{
			m_Instance = new GamePlaySelection();
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

	//
	void CreateInitGamePlay();
};
#endif //__GAMEPLAYSELECTION_H__