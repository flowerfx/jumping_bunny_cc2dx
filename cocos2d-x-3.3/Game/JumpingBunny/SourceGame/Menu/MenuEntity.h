#ifndef __MENUENTITY_H__
#define __MENUENTITY_H__

#include "Init/DisplayView.h"
#include "Init/ParamTexture.h"
#include "cocos2d.h"
USING_NS_CC;

const std::string p_listMenu[] = { "main", "option", "info", "HUD", "IGM", "ResultMenu","IAPMenu", "PopUpMenu" , "BoostMenu","leaderboardMenu","SplashScreen","GamePlaySelectMenu","LevelSelectionMenu" };

class ResourceEntity : public Ref
{
protected:
	Sprite* p_Resource;
	Point p_dirMove;
	bool p_IsVisible;
	Point p_originPos;
	Point p_firstPos;
public:
	ResourceEntity()
	{
		p_dirMove = Point(0, 0);
		p_originPos = Point(0, 0);
		p_firstPos = Point(0, 0);
		p_IsVisible = true;
		p_Resource = NULL;
	}
	~ResourceEntity()
	{
		CC_SAFE_RELEASE(p_Resource);
	}
	Sprite * GetResource()
	{
		return p_Resource;
	}
protected:
	void SetScaleResource(Point s)
	{
		CCASSERT(p_Resource, "Resource invaid");
		Point p_scale(1, 1);
		if (TextureMgr->IsUseMipMap())
		{ 
			p_scale.x = s.x * 2;
			p_scale.y = s.y * 2;
		}
		else
		{
			p_scale.x = s.x;
			p_scale.y = s.y;
		}
		p_Resource->setScaleX(p_scale.x);
		p_Resource->setScaleY(p_scale.y);
	}

	bool VisibleResource()
	{
		CCASSERT(p_Resource, "Resource invaid");
		return p_IsVisible;
		//p_Resource->isVisible();
	}

	void VisibleResource(bool b)
	{
		CCASSERT(p_Resource, "Resource invaid");
		//p_Resource->setVisible(b);
		p_IsVisible = b;
	}

	Point GetScaleResource()
	{
		CCASSERT(p_Resource, "Resource invaid");
		Point p_Size(1, 1);
		if (TextureMgr->IsUseMipMap())
		{
			p_Size.x = p_Resource->getScaleX() / 2.f;
			p_Size.y = p_Resource->getScaleY() / 2.f;
		}
		else
		{
			p_Size.x = p_Resource->getScaleX();
			p_Size.y = p_Resource->getScaleY();
		}
		return p_Size;
	}

	Point GetSizeResource()
	{
		CCASSERT(p_Resource, "Resource invaid");
		Point p_Size(1, 1);
		p_Size.x = p_Resource->getContentSize().width * p_Resource->getScaleX();
		p_Size.y = p_Resource->getContentSize().height * p_Resource->getScaleY();
		return p_Size;
	}

	void SetSizeResource(Point s)
	{
		CCASSERT(p_Resource, "Resource invaid");
		Point p_scale(1, 1);
		p_scale.x = s.x / p_Resource->getContentSize().width;
		p_scale.y = s.y / p_Resource->getContentSize().height;
		p_Resource->setScaleX(p_scale.x);
		p_Resource->setScaleY(p_scale.y);
	}

	void SetRotateResource(float r)
	{
		CCASSERT(p_Resource, "Resource invaid");
		p_Resource->setRotation(r);
	}

	float GetRotateResource()
	{
		CCASSERT(p_Resource, "Resource invaid");
		return p_Resource->getRotation();
	}

	void SetPositionResource(Point p)
	{
		CCASSERT(p_Resource, "Resource invaid");
		p_Resource->setPosition(p);
	}

	Point GetPositionResource()
	{
		CCASSERT(p_Resource, "Resource invaid");
		return p_Resource->getPosition();
	}

	void VisitResource(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch = NULL);
	void DrawResource(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch = NULL);
public:
	void LoadResourceFromSingleImage(std::string name)
	{
		p_Resource = Sprite::create(name);
		p_Resource->retain();
	}
	/////////////////////////////////////////////////////
	//--------------------------------------------------
	/////////////////////////////////////////////////////
	void LoadResourceFromSingleTexture(Texture2D * tx)
	{
		p_Resource = Sprite::createWithTexture(tx);
		if (p_Resource)
		{		
			p_Resource->retain();
		}
	}
	/////////////////////////////////////////////////////
	//--------------------------------------------------
	/////////////////////////////////////////////////////
	void LoadResourceFromData(TYPE_TEXTURE t, std::string str)
	{
		p_Resource = ObjectTexture::GetInstance()->LoadSpriteFromTexture(t, str);
		if (p_Resource)
		{	
			p_Resource->retain();
		}

	}
	/////////////////////////////////////////////////////
	//--------------------------------------------------
	/////////////////////////////////////////////////////
	void parseSprite(Sprite * tx)
	{
#ifdef USE_SPRITE_TEXTURE_CACHE
		p_Resource = tx;
#else
		p_Resource = tx->createWithTexture(tx->getTexture(), tx->getTextureRect());
#endif
#if USE_ANTIALIAS
		p_Resource->getTexture()->generateMipmap();
		Texture2D::TexParams texParams = { GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE };
		p_Resource->getTexture()->setTexParameters(&texParams);
#endif
		p_Resource->retain();
	}
	/////////////////////////////////////////////////////
	//--------------------------------------------------
	/////////////////////////////////////////////////////
	Point GetOriginPos()
	{
		return p_originPos;
	}
	Point GetFirstPos()
	{
		return p_firstPos;
	}
	void SetFirstPos(Point p)
	{
		p_firstPos = p;
	}

};

class ButtonEntity : public ResourceEntity
{
private:
	bool		p_IsPress;
	Point		p_ScaleNormal;
	std::string p_Name;
public:
	ButtonEntity();
	~ButtonEntity();
	Point GetMoveDir() { return p_dirMove; }
	Size GetNormalSize()
	{
		return Size(p_Resource->getContentSize().width * p_ScaleNormal.x, 
			p_Resource->getContentSize().height * p_ScaleNormal.y);
	}

	Point GetSize() { return GetSizeResource(); }
	void SetSize(Point p) { SetSizeResource(p); }

	void SetScale(Point s);
	Point GetScale();
	void SetPosition(Point p);
	Point GetPosition();
	bool IsPress();
	void IsPress(bool b);

	float GetRotate(){ return GetRotateResource(); }
	void  SetRotate(float r){ return SetRotateResource(r); }

	void Visible(bool b) { VisibleResource(b); }
	bool Visible() { return VisibleResource(); }

	void UpdatePress();

	void VisitButton(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch = NULL);
	void DrawButton(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch = NULL);

	void Init(std::string name_button, std::string name_source, Point pos, Point scale, Point movedir = Point(0,0), float rotate = 0.0f);

	void LoadButtonItem(int typeOfLoad = 0, std::string name_button = "", Texture2D * t = NULL, Sprite * p = NULL);
	std::string GetName()
	{
		return p_Name;
	}
	
};

class BackgroundEntity : public ResourceEntity
{
private:
	std::string p_Name;
public:
	BackgroundEntity();
	~BackgroundEntity();

	Point GetSize() { return GetSizeResource(); }
	void SetSize(Point p) { SetSizeResource(p); }
	Point GetMoveDir() { return p_dirMove; }
	void Visible(bool b) { VisibleResource(b); }
	bool Visible() { return VisibleResource(); }
	void SetScale(Point s) 
	{ 
		Point si(1, 1);
		si.x = s.x * GetWinSize().width;
		si.y = s.y * GetWinSize().height;
		if (s.y == 0)
		{
			si.y = si.x;
		}
		else if (s.x == 0)
		{
			si.x = si.y;
		}
		SetSize(si);
	}
	Point GetScale() { return GetScaleResource(); }
	void SetRealScale(Point p) { SetScaleResource(p); }
	void SetPosition(Point p) { SetPositionResource(p); }
	Point GetPosition() {return GetPositionResource(); }

	float GetRotate(){ return GetRotateResource(); }
	void  SetRotate(float r){ return SetRotateResource(r); }

	void VisitBackground(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch = NULL);
	void DrawBackground(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch = NULL);

	void Init(std::string name_background, std::string name_source,Point pos, Point scale, Point movedir = Point(0, 0), float rotate = 0.0f);

	void LoadBackgroundItem(int typeOfLoad = 0, std::string name_button = "", Texture2D * t = NULL, Sprite * p = NULL);
	std::string GetName()
	{
		return p_Name;
	}
};

class StringEntity : public Ref
{
private:
	Label* str;

	Point p_dirMove;
	bool p_IsVisible;
	std::string p_curStr;
	Point p_Pos;
	float p_Rotate;
	Color4B p_tint;
	Color4B p_tintBorder;
	int borderSize;
	float p_originSize;
	bool p_IsUnicode;
	int p_idxFont;

	Point p_OriginPos;
	Point p_firstPos;
public:
	StringEntity()
	{ 
		p_Pos = Point(0, 0);
		p_Rotate = 0.f;
		p_dirMove = Point(0, 0); 
		p_firstPos = Point(0, 0);
		p_OriginPos = Point(0, 0);
		p_IsVisible = true; 
		borderSize = 0;
		str = NULL;
	}
	~StringEntity() 
	{
		CC_SAFE_RELEASE(str);
	}
	float GetRotate();
	void SetRotate(float r);
	//
	Vec2 GetScale() { return Vec2(str->getScaleX(), str->getScaleY()); }
	void SetScale(Vec2 v) { str->setScaleX(v.x); str->setScaleY(v.y); }
	//
	bool IsVisible();
	void IsVisible(bool b);
	//
	Point GetDirMove();
	void SetDirMove(Point p);
	//
	void SetPosition(Point p);
	Point GetPosition();
	//
	void SetTint(Color4B tint);
	void SetTintBorder(Color4B tintBorder, int bordersize);
	//
	void SetString(Label * p, ResUILayout * res, bool IsUnicode = false);
	void SetStringName(std::string text);
	void ReloadString();
	std::string GetStringName();
	//
	void UpdateString(float dt);
	Label* GetLabel() { return str; }
	void VisitString(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch = NULL);
	void DrawString(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch = NULL);
	//
	Point GetOriginPos() { return p_OriginPos; }
	Point GetFirstPos() { return p_firstPos; }
	void SetFirstPos(Point p) { p_firstPos = p; }
};

enum STATUS_MENU
{
	M_IDLE = 0,
	M_FADEIN,
	M_FADEOUT
};

class MenuEntity : public Ref
{
protected:
	Vector<ButtonEntity*>				p_ListButton;
	Vector<BackgroundEntity*>		    p_ListBackground;
	Map<std::string, StringEntity*>	    p_DisplayString;
	Map<std::string, BackgroundEntity*>	p_ListSpecificBG;
	STATUS_MENU		p_curStatus;

	float p_DeltaMoveTime;
	Point p_DeltaMoveDir;
	float p_DistanceDelta;

	Point p_DeltaPosWithBGMain;

	void InitEntity(std::string str);

	bool p_IsPlaySoundFadeIn;
	bool p_IsPlaySoundFadeOut;
	bool p_IsPlaySoundPress;

	int p_IdxMenuData;

	SpriteBatchNode * p_SpriteBatchUI;
	SpriteBatchNode * p_SpriteBatchButton;


public:
	MenuEntity();
	virtual ~MenuEntity();

	void InitTheValue(std::string str, ResUILayout* re);
	void ResetPositionResource();
	virtual void onDraw(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch = NULL);
	virtual void onVisit(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch = NULL);
	void onUpdate(float dt);

	virtual void OnFadeIn() {};
	virtual void OnFadeOut(){};
	virtual void OnShow(){};
	virtual void OnHide(){};

	STATUS_MENU GetCurrentStatus() { return p_curStatus; }

	void onCheckPress();
	virtual void OnReloadString();
	virtual void OnProcessButton(std::string name) {}

	Point CalculateMoveDir(Point pos, Point moveDir);
	void  CalculateFirstPosItem();
	void  UpdateFadeProcessItem(float initdeltatime, float dt, bool ForceFadeTime = false, bool IsPopUp = false);
	Point CalibrateMenuItem(Point currentPoint, Point OriginPoint, Point MoveDir , bool FadeIn = true);
	void InsertString(std::string name_str, StringEntity* str_data)
	{
		p_DisplayString.insert(name_str, str_data);
	}
	StringEntity* GetString(std::string name_str)
	{
		return p_DisplayString.find(name_str)->second;
	}
	BackgroundEntity* GetBackgroundByName(std::string name)
	{
		for (int i = 0; i < p_ListBackground.size(); i++)
		{
			if (p_ListBackground.at(i)->GetName() == name)
			{
				return p_ListBackground.at(i);
			}
		}
		return NULL;
	}
	ButtonEntity* GetButtonByName(std::string name)
	{
		for (int i = 0; i < p_ListButton.size(); i++)
		{
			if (p_ListButton.at(i)->GetName() == name)
			{
				return p_ListButton.at(i);
			}
		}
		return NULL;
	}
	//
};

#endif //__MENUENTITY_H__

