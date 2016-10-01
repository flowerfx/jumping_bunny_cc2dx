#include "MainMenu.h"
#include "PopUpMenu.h"
#include "GamePlay/ManageObjectAP.h"

#include "FXFlatform/XFlatform.h"

#if USE_CC_AUDIO
#include "Utility/SoundManager.h"
#endif
using namespace cocos2d;

PopUpMenu * PopUpMenu::m_Instance = NULL;

PopUpMenu::PopUpMenu()
{
	nameBtnProcess = "";
	p_NumberBtn = 0;
	p_IdxMenuData = 7;
}


PopUpMenu::~PopUpMenu()
{
}

void PopUpMenu::Init()
{
	InitEntity(p_listMenu[p_IdxMenuData]);
	p_strCallback = "";
	p_ObjectCallBack = NULL;
	p_Function = NULL;
}


void PopUpMenu::Update(float dt)
{
	onUpdate(dt);
	if (p_DeltaMoveTime <= 0)
	{
		p_DeltaMoveTime = 0;
		if (p_curStatus == STATUS_MENU::M_FADEOUT)
		{
			MenuMgr->IsPopUpAppear(false);
			if (nameBtnProcess == "OKBtn" || (p_NumberBtn == 1 && this->p_Function != NULL))
			{
				if (this->p_Function != NULL)
				{			
					callFunction(this->p_Function);
				}
			}
			//reset object posion to make sure the menu not stuck
			ResetPositionResource();
			//release
			p_ObjectCallBack = NULL;
			p_Function = NULL;
		}
		p_curStatus = STATUS_MENU::M_IDLE;
		//
		if (XFlatform::PopUp_GetClose() == true)
		{
			OnHide();
			XFlatform::PopUp_SetClose(false);
		}
		//
		if (IsBackKeyPress() && p_NumberBtn > 0 )
		{
			OnProcessButton("CloseBtn");
			SetBackkey(false);
		}
	}
	else
	{
		float initDeltaTime = XMLMgr->GetUIData()->dataMenu.find(p_listMenu[p_IdxMenuData])->second->deltaTime;
		UpdateFadeProcessItem(initDeltaTime, dt * 100.f, false, true);
		if (p_curStatus == STATUS_MENU::M_FADEIN && (int)p_DeltaMoveTime == (int)(initDeltaTime / 2.f) && p_IsPlaySoundFadeIn)
		{
#if USE_CC_AUDIO
			GetSound->PlaySoundEffect(XMLMgr->GetPathSound("SFX_POPUP_APPEAR"));
#endif
			p_IsPlaySoundFadeIn = false;
		}
	}
}
void PopUpMenu::Visit(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch )
{
	onVisit( renderer,  transform,  flags,  spriteBatch );
}
void PopUpMenu::Draw(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch)
{
	onDraw(renderer, transform, flags, spriteBatch);
}

void PopUpMenu::OnProcessButton(std::string name)
{
	OnHide();
	nameBtnProcess = name;
}

void PopUpMenu::OnFadeIn()
{
	p_DeltaMoveTime = XMLMgr->GetUIData()->dataMenu.find(p_listMenu[p_IdxMenuData])->second->deltaTime;
	p_curStatus = STATUS_MENU::M_FADEIN;
}
void PopUpMenu::OnFadeOut()
{
	p_DeltaMoveTime = XMLMgr->GetUIData()->dataMenu.find(p_listMenu[p_IdxMenuData])->second->deltaTime;
	p_curStatus = STATUS_MENU::M_FADEOUT;
}
void PopUpMenu::OnShowPopUp(std::string str, int numberBtn, std::string strCB, void * objectCallBack, void(*pt2Function)(void* pt2Object, std::string str))
{
	MenuMgr->IsPopUpAppear(true);
	OnFadeIn();
	//
	p_DisplayString.at("Text0")->SetStringName(str);
	if (numberBtn == 2)
	{
		p_ListButton.at(0)->Visible(true);
		p_ListButton.at(1)->Visible(true);
	}
	else if (numberBtn == 1)
	{
		p_ListButton.at(0)->Visible(true);
		p_ListButton.at(1)->Visible(false);
	}
	else
	{
		p_ListButton.at(0)->Visible(false);
		p_ListButton.at(1)->Visible(false);
	}
	p_NumberBtn = numberBtn;
	p_strCallback = strCB;
	p_ObjectCallBack = objectCallBack;
	p_Function = pt2Function;
}
void PopUpMenu::OnHide()
{
	OnFadeOut();
}

void PopUpMenu::callFunction(void(*pt2Function)(void* , std::string ))
{
	/* do something */
	pt2Function(p_ObjectCallBack,p_strCallback);  // make callback
	p_strCallback = "";
}