#include "MenuEntity.h"
#include "Control/InputManager.h"
#include "Utility/ObjectTexture.h"
#if USE_CC_AUDIO
#include "Utility/SoundManager.h"
#endif
//////////////////////////////////////////////////////////////////////////////////////
//
// __RESOURCE_ENTITY______________
//
//////////////////////////////////////////////////////////////////////////////////////
void ResourceEntity::VisitResource(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch)
{
	CCASSERT(p_Resource, "Resource invaid");
	if (p_IsVisible)
	{
		if (spriteBatch == NULL)
		{
			p_Resource->visit(renderer, transform, flags);
		}
		else
		{
			spriteBatch->addChild(p_Resource);		
		}
	}
}

void ResourceEntity::DrawResource(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch)
{
	CCASSERT(p_Resource, "Resource invaid");
	if (p_IsVisible)
	{
		if (spriteBatch == NULL)
		{
			p_Resource->draw(renderer, transform, flags);
		}
		else
		{
			spriteBatch->addChild(p_Resource);			
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////
//
// __STRING_ENTITY______________
//
//////////////////////////////////////////////////////////////////////////////////////
void StringEntity::UpdateString(float dt) 
{
}
void StringEntity::DrawString(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch)
{
	if (p_IsVisible)
	{
		if (spriteBatch == NULL)
		{
			str->visit(renderer, transform, flags);
		}
		else
		{
			spriteBatch->addChild(str);
		}
	}
}
void StringEntity::VisitString(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch )
{
	if (p_IsVisible)
	{
		if (spriteBatch == NULL)
		{
			str->visit(renderer, transform, flags);
		}
		else
		{
			spriteBatch->addChild(str);
		}
	}
}
float StringEntity::GetRotate()
{
	CCASSERT(str, "string invalid!");
	return str->getRotation();
}
void StringEntity::SetRotate(float r)
{
	CCASSERT(str, "string invalid!");
	str->setRotation(r);
	p_Rotate = r;
}
bool StringEntity::IsVisible()
{
	return p_IsVisible;
}
Point StringEntity::GetDirMove()
{
	return p_dirMove;
}
void StringEntity::SetDirMove(Point p)
{
	p_dirMove = p;
}
void StringEntity::IsVisible(bool b)
{
	p_IsVisible = b;
}
void StringEntity::SetPosition(Point p)
{
	CCASSERT(str, "string invalid!");
	str->setPosition(p);
	p_Pos = p;
}
Point StringEntity::GetPosition()
{
	CCASSERT(str, "string invalid!");
	return str->getPosition();
}
void StringEntity::SetTint(Color4B tint)
{
	p_tint = tint;
	str->setColor(Color3B(p_tint.r, p_tint.g, p_tint.b));
}
void StringEntity::SetTintBorder(Color4B tintBorder, int bordersize)
{
	if (XMLMgr->GetHarwardProfile() == PLOW)
	{
		//not set border when memory low
		return;
	}
	p_tintBorder = tintBorder;
	borderSize = bordersize;
	if (!TextureMgr->IsUseMipMap())
	{
		str->enableOutline(p_tintBorder, bordersize);
	}
}
void StringEntity::SetString(Label * p, ResUILayout * res, bool IsUnicode)
{
	CCASSERT(p, "string invalid!");
	auto posRatio = ConvertPos(res->PosRatio);
	auto pos = Point(GetWinSize().width * posRatio.x, GetWinSize().height * posRatio.y);
	auto scale = res->ScaleRatio;
#ifdef OS_IOS
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-variable"
#endif
	p_idxFont = res->fontIdx;
#ifdef OS_IOS
#pragma clang diagnostic pop   
#endif
	p_IsUnicode = IsUnicode;
	p_originSize = scale.x*GetScaleFactor();
	TTFConfig config(p->getTTFConfig().fontFilePath.c_str(), 40, IsUnicode ? GlyphCollection::ASCII : GlyphCollection::DYNAMIC);
	str = p->createWithTTF(config, res->source);
	p_curStr = res->source;
	str->retain();
	SetPosition(pos);
	p_OriginPos = pos;
	SetTint( Color4B(res->Tint[0], res->Tint[1], res->Tint[2], res->Tint[3]));
	SetTintBorder(Color4B(res->TintBorder[0], res->TintBorder[1], res->TintBorder[2], res->TintBorder[3]), (int)(res->BorderWid * GetScaleFactor()));

	SetDirMove(res->DeltaMove);
	SetRotate(res->Rotate);
	float scale_i = p_originSize / 40.f;
	SetScale(Vec2(scale_i, scale_i));

}
void StringEntity::ReloadString()
{
	auto p = TextureMgr->GetFontData(p_idxFont);
	TTFConfig config(p->getTTFConfig().fontFilePath.c_str(), 40, p_IsUnicode ? GlyphCollection::ASCII : GlyphCollection::DYNAMIC);
	str = Label::createWithTTF(config, GetStringName());
	str->retain();
	SetPosition(p_Pos);
	SetTint(p_tint);
	SetTintBorder(p_tintBorder, borderSize);
	SetRotate(p_Rotate);
	float scale_i = p_originSize / 40.f;
	SetScale(Vec2(scale_i, scale_i));
}
void StringEntity::SetStringName(std::string text)
{
	str->setString(text);
	p_curStr = text;
}
std::string StringEntity::GetStringName()
{
	return p_curStr;
}
//////////////////////////////////////////////////////////////////////////////////////
//
// __BUTTON_ENTITY______________
//
//////////////////////////////////////////////////////////////////////////////////////

ButtonEntity::ButtonEntity()
{
	p_IsPress = false;
	p_ScaleNormal = Point(0, 0);
}
ButtonEntity::~ButtonEntity()
{
	//CC_SAFE_RELEASE(p_Button);
}
void ButtonEntity::Init(std::string name_button, std::string name_source, Point pos, Point scale, Point movedir, float rotate)
{
#if USE_SPRITE_STORE
	auto te = TextureMgr->GetSpriteFromMemory(name_source);
	CCASSERT(te, "texture invalid or not loadable");
	LoadButtonItem(3, "",NULL, te);
#else
	auto te = TextureMgr->GetTexureFromMemory(name_source);
	CCASSERT(te, "texture invalid or not loadable");
	LoadButtonItem(2, "",te);
#endif
	p_Name = name_button;
	p_ScaleNormal = scale;
	SetPosition(pos);
	p_originPos = pos;
	SetScale(scale);
	p_dirMove = movedir;
	SetRotate(rotate);
}

void ButtonEntity::LoadButtonItem(int typeOfLoad, std::string name_button, Texture2D * t, Sprite * p)
{
	bool loaded = false;
	if (typeOfLoad == 0)
	{
		CCASSERT(false, "this method not support anymore !");
		LoadResourceFromSingleImage(name_button);
		loaded = true;
	}
	else if (typeOfLoad == 1)
	{
		CCASSERT(false, "this method not support anymore !");
		LoadResourceFromData(TYPE_TEXTURE::MENU_TEXRURE, name_button);
		loaded = true;
	}
	else if (typeOfLoad == 2)
	{
		LoadResourceFromSingleTexture(t);
		loaded = true;
	}
	else if (typeOfLoad == 3)
	{
		parseSprite(p);
		loaded = true;
	}
	//no load image available
	/*if (p_Resource->getParent() != nullptr)
	{
		TextureMgr->GetGlobalSpriteBatch(TYPE_TEXTURE::MENU_TEXRURE)->addChild(p_Resource);
	}*/
	CCASSERT(loaded, "wrong method load button or error in loading button !");
}

void ButtonEntity::SetScale(Point s)
{
	SetScaleResource(s);
}

Point ButtonEntity::GetScale()
{
	return GetScaleResource();
}

void ButtonEntity::SetPosition(Point p)
{
	SetPositionResource(p);
}

Point ButtonEntity::GetPosition()
{
	return GetPositionResource();
}

bool ButtonEntity::IsPress()
{
	return p_IsPress;
}
void ButtonEntity::IsPress(bool b)
{
	p_IsPress = b;
}

void ButtonEntity::UpdatePress()
{
	CCASSERT(p_Resource, "button invaid");
	Point s = p_ScaleNormal;
	if (p_IsPress)
	{
		s = p_ScaleNormal * 3.f / 4.f;
	}
	SetScale(s);
}

void ButtonEntity::VisitButton(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch)
{
	VisitResource(renderer, transform, flags, spriteBatch);
}
void ButtonEntity::DrawButton(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch)
{
	DrawResource(renderer, transform, flags,spriteBatch);
}

//////////////////////////////////////////////////////////////////////////////////////
//
// __BACKGROUND_ENTITY______________
//
//////////////////////////////////////////////////////////////////////////////////////

BackgroundEntity::BackgroundEntity()
{
	p_Name = "";
}
BackgroundEntity::~BackgroundEntity()
{
	//CC_SAFE_RELEASE(p_Button);
}
void BackgroundEntity::Init(std::string name_button, std::string name_source, Point pos, Point scale, Point movedir, float rotate)
{
#if USE_SPRITE_STORE
	auto te = TextureMgr->GetSpriteFromMemory(name_source);
	CCASSERT(te, "texture invalid or not loadable");
	LoadBackgroundItem(3, "",NULL, te);
#else
	auto te = TextureMgr->GetTexureFromMemory(name_source);	
	CCASSERT(te, "texture invalid or not loadable");
	LoadBackgroundItem(2, "", te);
#endif
	p_Name = name_button;
	SetPosition(pos);
	p_originPos = pos;
	SetScale(scale);
	p_dirMove = movedir;
	SetRotate(rotate);
}

void BackgroundEntity::LoadBackgroundItem(int typeOfLoad, std::string name_button, Texture2D * t,  Sprite * p )
{
	bool loaded = false;
	if (typeOfLoad == 0)
	{
		CCASSERT(false, "this method not support anymore !");
		LoadResourceFromSingleImage(name_button);
		loaded = true;
	}
	else if (typeOfLoad == 1)
	{
		CCASSERT(false, "this method not support anymore !");
		LoadResourceFromData(TYPE_TEXTURE::MENU_TEXRURE, name_button);
		loaded = true;
	}
	else if (typeOfLoad == 2)
	{
		LoadResourceFromSingleTexture(t);
		loaded = true;
	}
	else if (typeOfLoad == 3)
	{
		parseSprite(p);
		loaded = true;
	}
	//no load image available
	CCASSERT(loaded, "wrong method load button or error in loading button !");
}

void BackgroundEntity::VisitBackground(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch)
{
	//spriteBatch = TextureMgr->GetGlobalSpriteBatch(TYPE_TEXTURE::MENU_LAYOUT);
	VisitResource(renderer, transform, flags, spriteBatch);
}
void BackgroundEntity::DrawBackground(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch)
{
	DrawResource(renderer, transform, flags,spriteBatch);
}

//////////////////////////////////////////////////////////////////////////////////////
//
// __MENU_ENTITY______________
//
//////////////////////////////////////////////////////////////////////////////////////

MenuEntity::MenuEntity()
{
	p_curStatus = STATUS_MENU::M_IDLE;

	p_DeltaMoveDir = Point(0, 0);
	p_DeltaMoveTime = 0;
	p_DistanceDelta = 0;

	p_DeltaPosWithBGMain = Point(0, 0);
	p_IsPlaySoundFadeIn = false;
	p_IsPlaySoundFadeOut = false;
	p_IsPlaySoundPress = false;

	p_IdxMenuData = -1;

	p_SpriteBatchButton = NULL;
	p_SpriteBatchUI = NULL;
}


MenuEntity::~MenuEntity()
{
	if (p_SpriteBatchButton)
	{
		p_SpriteBatchButton->removeAllChildrenWithCleanup(true);
		p_SpriteBatchButton->release();
	}

	if (p_SpriteBatchUI)
	{
		p_SpriteBatchUI->removeAllChildrenWithCleanup(true);
		p_SpriteBatchUI->release();
	}
}

void MenuEntity::onDraw(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch)
{
	if (p_SpriteBatchButton)
	{
		p_SpriteBatchButton->removeAllChildrenWithCleanup(true);
	}
	if (p_SpriteBatchUI)
	{
		p_SpriteBatchUI->removeAllChildrenWithCleanup(true);
	}

	for (int i = 0; i < p_ListBackground.size(); i++)
	{
		p_ListBackground.at(i)->DrawBackground(renderer, transform, flags, p_SpriteBatchUI);
	}
	for (int i = 0; i < p_ListButton.size(); i++)
	{
		p_ListButton.at(i)->DrawButton(renderer, transform, flags, p_SpriteBatchButton);
	}

	if (p_DisplayString.size() > 0)
	{
		for (auto it = p_DisplayString.begin(); it != p_DisplayString.end(); ++it)
		{
			it->second->DrawString(renderer, transform, flags, spriteBatch);
		}
	}
	if (p_ListSpecificBG.size() > 0)
	{
		for (auto it = p_ListSpecificBG.begin(); it != p_ListSpecificBG.end(); ++it)
		{
			it->second->DrawBackground(renderer, transform, flags, p_SpriteBatchUI);
		}
	}
	//
	if (p_SpriteBatchButton)
	{
		p_SpriteBatchButton->draw(renderer, transform, flags);
	}
	if (p_SpriteBatchUI)
	{
		p_SpriteBatchUI->draw(renderer, transform, flags);
	}
}
void MenuEntity::onVisit(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch)
{
	for (int i = 0; i < p_ListBackground.size(); i++)
	{
		p_ListBackground.at(i)->VisitBackground(renderer, transform, flags, p_SpriteBatchUI);
	}
	for (int i = 0; i < p_ListButton.size(); i++)
	{
		p_ListButton.at(i)->VisitButton(renderer, transform, flags, p_SpriteBatchButton);
	}
	if (p_DisplayString.size() > 0)
	{
		for (auto it = p_DisplayString.begin(); it != p_DisplayString.end(); ++it)
		{
			it->second->VisitString(renderer, transform, flags, spriteBatch);
		}
	}
	if (p_ListSpecificBG.size() > 0)
	{
		for (auto it = p_ListSpecificBG.begin(); it != p_ListSpecificBG.end(); ++it)
		{
			it->second->VisitBackground(renderer, transform, flags, p_SpriteBatchUI);
		}
	}
	//
	if (p_SpriteBatchButton)
	{
		p_SpriteBatchButton->visit(renderer, transform, flags);
	}
	if (p_SpriteBatchUI)
	{
		p_SpriteBatchUI->visit(renderer, transform, flags);
	}
}
void MenuEntity::onUpdate(float dt)
{
	onCheckPress();
	for (int i = 0; i < p_ListButton.size(); i++)
	{
		p_ListButton.at(i)->UpdatePress();
	}
}

void MenuEntity::onCheckPress()
{
	if (InputMgr->GetNumberTouch() > 0)
	{
		auto p = InputMgr->GetPosAtId(0);
		for (int i = 0; i < p_ListButton.size(); i++)
		{
			auto p = p_ListButton.at(i)->GetPosition();
			auto s = p_ListButton.at(i)->GetNormalSize();
			Rect rec(p.x - s.width / 2, p.y - s.height / 2, s.width, s.height);
			if (p_ListButton.at(i)->Visible() && InputMgr->IsPressOnButton(rec))
			{
				p_ListButton.at(i)->IsPress(true);
				InputMgr->SetIsTouchOnScreen();
#if USE_CC_AUDIO
				if (p_IsPlaySoundPress)
				{
					GetSound->PlaySoundEffect(XMLMgr->GetPathSound("SFX_BTN_CLICK"));
					p_IsPlaySoundPress = false;
				}
#endif
			}
			else
			{
				p_ListButton.at(i)->IsPress(false);
			}
		}
	}
	else
	{
		for (int i = 0; i < p_ListButton.size(); i++)
		{
			if (p_ListButton.at(i)->Visible() && p_ListButton.at(i)->IsPress() == true)
			{
				auto p = p_ListButton.at(i)->GetPosition();
				auto s = p_ListButton.at(i)->GetNormalSize();
				Rect rec(p.x - s.width / 2, p.y - s.height / 2, s.width, s.height);
				if (InputMgr->IsReleaseOnbutton(rec))
				{
					this->OnProcessButton(p_ListButton.at(i)->GetName());
				}
				InputMgr->OnClearTouchReleaseList();
			}
			p_ListButton.at(i)->IsPress(false);
		}
		p_IsPlaySoundPress = true;
	}
		
}

Point MenuEntity::CalculateMoveDir(Point pos, Point moveDir)
{

	float distanceX = sqrt((p_DeltaPosWithBGMain.x * p_DeltaPosWithBGMain.x * GetWinSize().width * GetWinSize().width));
	float distanceY = sqrt((p_DeltaPosWithBGMain.y * p_DeltaPosWithBGMain.y * GetWinSize().height * GetWinSize().height));

	Point p(pos.x, pos.y);
	if (moveDir == Point(1, 0))
	{
		p = Point(pos.x - distanceX, pos.y);
		p_DistanceDelta = distanceX;
	}
	else if (moveDir == Point(-1, 0))
	{
		p = Point(distanceX + pos.x, pos.y);
		p_DistanceDelta = distanceX;
	}
	else if (moveDir == Point(0, 1))
	{
		p = Point(pos.x, pos.y - distanceY);
		p_DistanceDelta = distanceY;
	}
	else if (moveDir == Point(0, -1))
	{
		p = Point(pos.x, distanceY + pos.y);
		p_DistanceDelta = distanceY;
	}
	return p;
}

void  MenuEntity::CalculateFirstPosItem()
{
	for (int i = 0; i < p_ListButton.size(); i++)
	{
		Point curPoint = p_ListButton.at(i)->GetPosition();
		curPoint = CalculateMoveDir(curPoint, p_ListButton.at(i)->GetMoveDir());
		p_ListButton.at(i)->SetPosition(curPoint);
		p_ListButton.at(i)->SetFirstPos(curPoint);
	}
	for (int i = 0; i < p_ListBackground.size(); i++)
	{
		Point curPoint = p_ListBackground.at(i)->GetPosition();
		curPoint = CalculateMoveDir(curPoint, p_ListBackground.at(i)->GetMoveDir());
		p_ListBackground.at(i)->SetPosition(curPoint);
		p_ListBackground.at(i)->SetFirstPos(curPoint);
	}
	if (p_DisplayString.size() > 0)
	{
		for (auto it = p_DisplayString.begin(); it != p_DisplayString.end(); ++it)
		{
			Point curPoint = it->second->GetPosition();
			curPoint = CalculateMoveDir(curPoint, it->second->GetDirMove());
			it->second->SetPosition(curPoint);
			it->second->SetFirstPos(curPoint);
		}
	}
	if (p_ListSpecificBG.size() > 0)
	{
		for (auto it = p_ListSpecificBG.begin(); it != p_ListSpecificBG.end(); ++it)
		{
			Point curPoint = it->second->GetPosition();
			curPoint = CalculateMoveDir(curPoint, it->second->GetMoveDir());
			it->second->SetPosition(curPoint);
			it->second->SetFirstPos(curPoint);
		}
	}
}

void  MenuEntity::UpdateFadeProcessItem(float initdeltatime, float dt, bool ForceFadeTime, bool IsPopUp)
{
//#ifdef _DEBUG
//	dt = SyncWithFPS(dt);
//#else
//	if (ForceFadeTime)
//	{
//		dt = GetCpuCoreCount() <= 2 ? dt * 2.f : 1.25f;
//	}
//	else
//	{
//		dt = GetCpuCoreCount() <= 2 ? dt * 2.f : SyncWithFPS(dt);
//	}
//#endif
	if (p_DeltaMoveTime == initdeltatime)
	{
		if (p_curStatus == STATUS_MENU::M_FADEIN)
			p_IsPlaySoundFadeIn = true;
		if (p_curStatus == STATUS_MENU::M_FADEOUT)
			p_IsPlaySoundFadeOut = true;
	}
	p_DeltaMoveTime -= (dt / 2);
	if (p_curStatus == STATUS_MENU::M_FADEIN)
	{
		if (!IsPopUp && (int)p_DeltaMoveTime == (int)(initdeltatime / 2.f) && p_IsPlaySoundFadeIn)
		{
#if USE_CC_AUDIO
			GetSound->PlaySoundEffect(XMLMgr->GetPathSound("SFX_MENU_SHOW"));
#endif
			p_IsPlaySoundFadeIn = false;
		}
		for (int i = 0; i < p_ListBackground.size(); i++)
		{
			Point curPoint = p_ListBackground.at(i)->GetPosition();
			curPoint = curPoint + (p_ListBackground.at(i)->GetMoveDir() * SyncWithFPS(dt * p_DistanceDelta / initdeltatime));
			curPoint = CalibrateMenuItem(curPoint, p_ListBackground.at(i)->GetOriginPos(), p_ListBackground.at(i)->GetMoveDir());
			p_ListBackground.at(i)->SetPosition(curPoint);
		}
		for (int i = 0; i < p_ListButton.size(); i++)
		{
			Point curPoint = p_ListButton.at(i)->GetPosition();
			curPoint = curPoint + (p_ListButton.at(i)->GetMoveDir() * SyncWithFPS(dt * p_DistanceDelta / initdeltatime));
			curPoint = CalibrateMenuItem(curPoint, p_ListButton.at(i)->GetOriginPos(), p_ListButton.at(i)->GetMoveDir());
			p_ListButton.at(i)->SetPosition(curPoint);
		}
		if (p_DisplayString.size() > 0)
		{
			for (auto it = p_DisplayString.begin(); it != p_DisplayString.end(); ++it)
			{
				Point curPoint = it->second->GetPosition();
				curPoint = curPoint + (it->second->GetDirMove()* SyncWithFPS(dt * p_DistanceDelta / initdeltatime));
				curPoint = CalibrateMenuItem(curPoint, it->second->GetOriginPos(), it->second->GetDirMove());
				it->second->SetPosition(curPoint);
			}
		}
		if (p_ListSpecificBG.size() > 0)
		{
			for (auto it = p_ListSpecificBG.begin(); it != p_ListSpecificBG.end(); ++it)
			{
				Point curPoint = it->second->GetPosition();
				curPoint = curPoint + (it->second->GetMoveDir()* SyncWithFPS(dt * p_DistanceDelta / initdeltatime));
				curPoint = CalibrateMenuItem(curPoint, it->second->GetOriginPos(), it->second->GetMoveDir());
				it->second->SetPosition(curPoint);
			}
		}
		//check is at the design position

	}
	else if (p_curStatus == STATUS_MENU::M_FADEOUT)
	{
		if ((int)p_DeltaMoveTime == (int)(initdeltatime / 2.f) && p_IsPlaySoundFadeOut)
		{
#if USE_CC_AUDIO
			GetSound->PlaySoundEffect(XMLMgr->GetPathSound("SFX_MENU_HIDE"));
#endif
			p_IsPlaySoundFadeOut = false;
		}
		for (int i = 0; i < p_ListBackground.size(); i++)
		{
			Point curPoint = p_ListBackground.at(i)->GetPosition();
			curPoint = curPoint - (p_ListBackground.at(i)->GetMoveDir()* SyncWithFPS(dt * p_DistanceDelta / initdeltatime));
			curPoint = CalibrateMenuItem(curPoint, p_ListBackground.at(i)->GetFirstPos(), p_ListBackground.at(i)->GetMoveDir(),false);
			p_ListBackground.at(i)->SetPosition(curPoint);
		}
		for (int i = 0; i < p_ListButton.size(); i++)
		{
			Point curPoint = p_ListButton.at(i)->GetPosition(); 
			curPoint = curPoint - (p_ListButton.at(i)->GetMoveDir()* SyncWithFPS(dt * p_DistanceDelta / initdeltatime));
			curPoint = CalibrateMenuItem(curPoint, p_ListButton.at(i)->GetFirstPos(), p_ListButton.at(i)->GetMoveDir(), false);
			p_ListButton.at(i)->SetPosition(curPoint);
		}
		if (p_DisplayString.size() > 0)
		{
			for (auto it = p_DisplayString.begin(); it != p_DisplayString.end(); ++it)
			{
				Point curPoint = it->second->GetPosition();
				curPoint = curPoint - (it->second->GetDirMove()* SyncWithFPS(dt * p_DistanceDelta / initdeltatime));
				curPoint = CalibrateMenuItem(curPoint, it->second->GetFirstPos(), it->second->GetDirMove(), false);
				it->second->SetPosition(curPoint);
			}
		}
		if (p_ListSpecificBG.size() > 0)
		{
			for (auto it = p_ListSpecificBG.begin(); it != p_ListSpecificBG.end(); ++it)
			{
				Point curPoint = it->second->GetPosition();
				curPoint = curPoint - (it->second->GetMoveDir()* SyncWithFPS(dt * p_DistanceDelta / initdeltatime));
				curPoint = CalibrateMenuItem(curPoint, it->second->GetFirstPos(), it->second->GetMoveDir(), false);
				it->second->SetPosition(curPoint);
			}
		}
	}
}

void  MenuEntity::InitTheValue(std::string str, ResUILayout * re)
{
	//
	//
	BackgroundEntity * bg = new BackgroundEntity();
	std::string name_res = str;
	auto layout = re;
	auto posRatio = ConvertPos(layout->PosRatio);
	if (layout->type == "Button")
	{
		ButtonEntity * btn = new ButtonEntity();
		auto pos = Point(GetWinSize().width * posRatio.x, GetWinSize().height * posRatio.y);
		auto scale = layout->ScaleRatio;
		btn->Init(name_res, re->source,pos, scale, layout->DeltaMove,layout->Rotate);
		p_ListButton.pushBack(btn);	
		//
		if (!p_SpriteBatchButton)
		{
			//p_SpriteBatchButton = SpriteBatchNode::createWithTexture(btn->GetResource()->getTexture());
			//p_SpriteBatchButton->retain();
		}
		//
		CC_SAFE_RELEASE_NULL(btn);
	}
	else if (layout->type == "Background")
	{
		BackgroundEntity * bg = new BackgroundEntity();
		auto pos = Point(GetWinSize().width * posRatio.x, GetWinSize().height * posRatio.y);
		auto scale = layout->ScaleRatio;
		bg->Init(name_res, re->source, pos, scale, layout->DeltaMove, layout->Rotate);
		p_ListBackground.pushBack(bg);
		if (!p_SpriteBatchUI)
		{
			//p_SpriteBatchUI = SpriteBatchNode::createWithTexture(bg->GetResource()->getTexture());
			//p_SpriteBatchUI->retain();
		}
		CC_SAFE_RELEASE_NULL(bg);
	}
	else if (layout->type == "SpecificBG")
	{
		BackgroundEntity * bg = new BackgroundEntity();
		auto pos = Point(GetWinSize().width * posRatio.x, GetWinSize().height * posRatio.y);
		auto scale = layout->ScaleRatio;
		bg->Init(name_res, re->source, pos, scale, layout->DeltaMove, layout->Rotate);
		p_ListSpecificBG.insert(name_res,bg);
		if (!p_SpriteBatchUI)
		{
			//p_SpriteBatchUI = SpriteBatchNode::createWithTexture(bg->GetResource()->getTexture());
		}
		CC_SAFE_RELEASE_NULL(bg);
	}
	else if (layout->type == "font")
	{
		auto scale = layout->ScaleRatio;
		int fontIdx = layout->fontIdx;
		//auto stringLabel = Label::createWithTTF(re->source, ResolveNamePath(GamefontList[fontIdx]), 1/*scale.x * GetScaleFactor()*/);	
		auto StrEntity = new StringEntity();
		bool IsUnicode = false;
		if (fontIdx == 5)
		{
			IsUnicode = true;
		}
		StrEntity->SetString(TextureMgr->GetFontData(fontIdx)/*stringLabel*/, layout, IsUnicode);
		InsertString(name_res, StrEntity);

		CC_SAFE_RELEASE_NULL(StrEntity);
	}

}

void MenuEntity::InitEntity(std::string str)
{
	auto list = XMLMgr->GetListResourceUI(str);
	p_DeltaMoveTime = XMLMgr->GetUIData()->dataMenu.find(str)->second->deltaTime ;
	p_DeltaPosWithBGMain = XMLMgr->GetUIData()->dataMenu.find(str)->second->distanceMoveDir;
	for (auto it = list.begin(); it != list.end(); ++it)
	{
		InitTheValue(it->first, it->second);
	}
	CalculateFirstPosItem();
}

void MenuEntity::ResetPositionResource()
{
	return;
	//not need this
	auto list = XMLMgr->GetListResourceUI(p_listMenu[p_IdxMenuData]);

	for (auto it = list.begin(); it != list.end(); ++it)
	{
		auto layout = it->second;
		auto posRatio = ConvertPos(layout->PosRatio);
		std::string name_res = it->first;
		if (layout->type == "Button")
		{
			auto pos = Point(GetWinSize().width * posRatio.x, GetWinSize().height * posRatio.y);
			for (int i = 0; i < p_ListButton.size(); i++)
			{
				if (p_ListButton.at(i)->GetName() == name_res)
				{
					p_ListButton.at(i)->SetPosition(pos);
					break;
				}
			}
		}
		else if (layout->type == "Background")
		{
			auto pos = Point(GetWinSize().width * posRatio.x, GetWinSize().height * posRatio.y);
			for (int i = 0; i < p_ListBackground.size(); i++)
			{
				if (p_ListBackground.at(i)->GetName() == name_res)
				{
					p_ListBackground.at(i)->SetPosition(pos);
					break;
				}
			}
		}
		else if (layout->type == "SpecificBG")
		{
			auto pos = Point(GetWinSize().width * posRatio.x, GetWinSize().height * posRatio.y);
			if (p_ListSpecificBG.find(name_res) != p_ListSpecificBG.end())
			{
				p_ListSpecificBG.find(name_res)->second->SetPosition(pos);
			}

		}
		else if (layout->type == "font")
		{
			auto pos = Point(GetWinSize().width * posRatio.x, GetWinSize().height * posRatio.y);
			if (p_DisplayString.find(name_res) != p_DisplayString.end())
			{
				p_DisplayString.find(name_res)->second->SetPosition(pos);
			}
		}
	}
	CalculateFirstPosItem();
}

void MenuEntity::OnReloadString()
{
	for (auto it = p_DisplayString.begin(); it != p_DisplayString.end(); it++)
	{
		it->second->ReloadString();
	}
}
Point MenuEntity::CalibrateMenuItem(Point currentPoint, Point OriginPoint, Point MoveDir, bool FadeIn)
{
	if (MoveDir.x > 0)
	{
		if (FadeIn)
		{
			if (currentPoint.x >= OriginPoint.x)
				currentPoint.x = OriginPoint.x;
		}
		else
		{
			if (currentPoint.x <= OriginPoint.x)
				currentPoint.x = OriginPoint.x;
		}
	}
	else if (MoveDir.x < 0)
	{
		if (FadeIn)
		{
			if (currentPoint.x <= OriginPoint.x)
				currentPoint.x = OriginPoint.x;
		}
		else
		{
			if (currentPoint.x >= OriginPoint.x)
				currentPoint.x = OriginPoint.x;
		}
	}
	else if (MoveDir.y > 0)
	{
		if (FadeIn)
		{
			if (currentPoint.y >= OriginPoint.y)
				currentPoint.y = OriginPoint.y;
		}
		else
		{
			if (currentPoint.y <= OriginPoint.y)
				currentPoint.y = OriginPoint.y;
		}
	}
	else if (MoveDir.y < 0)
	{
		if (FadeIn)
		{
			if (currentPoint.y <= OriginPoint.y)
				currentPoint.y = OriginPoint.y;
		}
		else
		{
			if (currentPoint.y >= OriginPoint.y)
				currentPoint.y = OriginPoint.y;
		}
	}
	return currentPoint;
}