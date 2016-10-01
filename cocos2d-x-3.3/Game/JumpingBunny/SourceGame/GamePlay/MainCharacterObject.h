#ifndef _MAINCHAR_OBJECT_H_
#define _MAINCHAR_OBJECT_H_

#include "cocos2d.h"
#include "EntityObject.h"
#include "ManageObjectAP.h"
USING_NS_CC;
class AnimateSprite
{
private:
	Vector<Sprite*> p_ListActionSprite;
	float			p_DeltaTime;
	float			p_RunTime;
	float			p_previousTIme;
	int				p_idxSprite;
public:
	AnimateSprite() {
		p_DeltaTime = 0;
		p_RunTime = 0;
		p_previousTIme = 0;
		p_idxSprite = 0;
	}
	~AnimateSprite() { p_ListActionSprite.clear(); }
	void PushSpriteToListAnimate(Sprite * p);
	void InitValue();
	void SetPosition(Point p);
	void SetSize(Point s);
	void SetRotate(float r);
	void OnDrawAnimate();
	void OnDrawAnimate(Renderer *renderer, const Mat4& transform, uint32_t flags);
	void Update(float dt);
	
};


#define GetChar MainCharacterObject::GetInstance()
#define FreeChar MainCharacterObject::ReleaseInstance();
#define USE_JUMP_PARTICLE 1
class MainCharacterObject : public EntityObject
{
private:
	static MainCharacterObject * m_Instance;
	int		p_currentOrbitIdx;
	int		p_previousOrbitIdx;
	bool	p_IsMoveToOrbitInit;
	bool    p_IsMoveToOrbit;
	float   previousDistance;
	EntityObject*		  p_ArrowPoint;
	Point	DeltaPosArrowWithCharacter;
	//AnimateSprite * p_Animate;

	float p_CalcuRunDeltaTime; 
	float p_CalcuFlyDeltaTime;

	bool p_IsOnReviving;
public:

	EntityObject*		GetArrowPoint() { return p_ArrowPoint; }
	static MainCharacterObject * GetInstance()
	{
		if (!m_Instance)
		{
			m_Instance = new MainCharacterObject();
		}
		return m_Instance;
	}
	static void ReleaseInstance()
	{
		if (m_Instance)
		{
			delete m_Instance;
			m_Instance = NULL;
		}
	}
	MainCharacterObject();
	~MainCharacterObject();
//////some public method/////////////////////////////////////////////////
	bool IsAlive()
	{
		if (p_IsOnReviving)
			return true;
		if (GetPosMainChar().x > -200.f && GetPosMainChar().y > -100.f)
			return true;
		return false;
	}
	bool IsOnReviving() { return p_IsOnReviving; }
	bool IsMoveToOrbitInit()
	{
		return p_IsMoveToOrbitInit;
	}

	int GetCurrentOrbitIdx()
	{
		return p_currentOrbitIdx;
	}
	void SetCurrentOrbitIdx(int i)
	{
		p_currentOrbitIdx = i;
	}
	void SetPreviousOrbitIdx()
	{
		p_previousOrbitIdx -=1;
		if (p_previousOrbitIdx <= 0)
		{
			p_previousOrbitIdx = 0;
		}
	}
	int GetPreviousOrbitIdx() { return p_previousOrbitIdx; }

	void SetSizeMainChar(Size s)
	{
		s = s * GetScaleFactor();
		SetSizeObject(s);
	}
	Size GetSizeMainChar()
	{
		return GetSizeObject();
	}
	void SetPosMainChar(Point p)
	{
		SetPosObject(p);
	}
	Point GetPosMainChar()
	{
		return GetPosObject();
	}

	//
	void LoadMainChar();
	void ReloadMainChar();

	float GetRotation();
	void SetRotation(float r);
	void Update(float dt);
	//revive the character
	void OnReviveChar();
	//
	void onDrawMainChar(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch = NULL);
	void onVisitMainChar(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch = NULL);
	//move as bezier curve
	void MoveBezierCurve(Point directVector, float distance);
	void BeginMoveBezier(OrbitObject* orb);
	//jump gravity
	void UpdateJumpGravity(float dt);
	void BeginJump(OrbitObject* orb);
	//update move character
	void MoveCharacterToPoint(Point p, bool IsOnMoveToOrbit = false, EntityObject* orb = NULL);
	void UpdateMoveCharacter(float dt,OrbitObject* orb = NULL, bool UpdatePosition = true);
	//load animation from editor
	/////////////////////////////////////////////////////
	//--------------------------------------------------
	/////////////////////////////////////////////////////
	virtual void LoadAnimateFromEditor() override;
private:
	std::vector<Point> m_ListBezierMove;
	bool			   m_isStopMoveBezier;
	Point GetHalfPoint(Point a, Point b)
	{
		return Point((a.x + b.x) / 2.f, (a.y + b.y) / 2.f);
	}
	Point				m_directToJump;
	bool				m_isStopJumpGravity;

	float				m_SpeedJumpDuration;
	float				m_SpeedJump;
	float				m_GravityPress;
#if USE_JUMP_PARTICLE
	std::string			p_name_jump_particle;
#else
	MotionStreak *		p_Streak;
#endif
	void InitValueMainChar();

	float				p_RatioSizeDistance;
public:
	float				GetRatioSizeDistance() { return p_RatioSizeDistance; }
	void StopJumpGravity(bool b)
	{
		m_isStopJumpGravity = b;
	}
#if USE_JUMP_PARTICLE
	std::string			GetNameJump() { return p_name_jump_particle; }
#endif
};

#endif //_MAINCHAR_OBJECT_H_