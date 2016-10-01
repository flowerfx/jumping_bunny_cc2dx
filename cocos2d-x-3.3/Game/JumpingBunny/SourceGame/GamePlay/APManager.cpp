#include "APManager.h"
#include "Menu/BoostMenu.h"
APManager * APManager::p_Instance = NULL;
APManager::APManager()
{
#if ! USE_SKY_MENU
	p_SkyObject = new BGObject();
#endif
	p_IsUseGravityBoost = false;
	p_IsUseJumpBoost = false;
	p_IsUseLifeBoost = false;

	p_currentPartPlay = 0;

	p_CurrentGamePlay = TYPE_GAMEPLAY::GAMEPLAY_ENDLESS_RUN;
}


APManager::~APManager()
{
#if ! USE_SKY_MENU
	CC_SAFE_RELEASE(p_SkyObject);
#endif
}

bool APManager::init()
{
//	int state = ManageObjectAP::GetInstance()->InitTheObjectGame(GetCurrentGamePlay(),true);
#if ! USE_SKY_MENU
	p_SkyObject->InitTheValue("sky");
#endif
//	if (state == 3)
//	{
//		return true;
//	}
//	return false;
	return true;
}

void APManager::InitWithGamePlay(TYPE_GAMEPLAY tp)
{
	do
	{
		int state = OBJAPMgr->InitTheObjectGame(tp, true);

		if (state >= 3)
		{
			OBJAPMgr->ResetStateLoad();
			return;
		}
	} while (true);

}

void APManager::update(float dt)
{
	ManageObjectAP::GetInstance()->Update(dt);
#if ! USE_SKY_MENU
	p_SkyObject->onUpdate(dt);
#endif
}


void APManager::OnVisit(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch)
{
#if ! USE_SKY_MENU
	p_SkyObject->onVisit(renderer, transform, flags, spriteBatch);
#endif
	ManageObjectAP::GetInstance()->OnVisit(renderer, transform, flags, spriteBatch);
}

void APManager::OnDraw(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch)
{
#if ! USE_SKY_MENU
	p_SkyObject->onDraw(renderer, transform, flags,spriteBatch);
#endif
	ManageObjectAP::GetInstance()->OnDraw(renderer, transform, flags, spriteBatch);
}

void APManager::ResetAllBoost(bool ForceReset)
{
	if (ForceReset)
	{
		p_IsUseGravityBoost = false;
		p_IsUseJumpBoost = false;
		p_IsUseLifeBoost = false;
	}
	else
	{
		if (p_IsUseGravityBoost)
		{
			if (BoostMenu::GetInstance()->IsCanUse("Item_gravity", false))
			{
				BoostMenu::GetInstance()->ProcessUse("Item_gravity");
				p_IsUseGravityBoost = true;
			}
			else
			{
				p_IsUseGravityBoost = false;
			}
		}
		if (p_IsUseJumpBoost)
		{
			if (BoostMenu::GetInstance()->IsCanUse("Item_Jump", false))
			{
				BoostMenu::GetInstance()->ProcessUse("Item_Jump");
				p_IsUseJumpBoost = true;
			}
			else
			{
				p_IsUseJumpBoost = false;
			}
		}
		if (p_IsUseLifeBoost)
		{
			if (BoostMenu::GetInstance()->IsCanUse("Item_life", false))
			{
				BoostMenu::GetInstance()->ProcessUse("Item_life");
				p_IsUseLifeBoost = true;
			}
			else
			{
				p_IsUseLifeBoost = false;
			}
		}
	}
}
