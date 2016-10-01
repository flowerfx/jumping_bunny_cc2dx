#ifndef _ORBIT_OBJECT_H_
#define _ORBIT_OBJECT_H_

#include "cocos2d.h"
#include "EntityObject.h"
USING_NS_CC;
#define INCREASE_BOOST  (3.0f / 2.0f)
enum TYPE_OBJECT
{
	DECORATE = 0,
	INTERACTIVE
};
enum TYPE_MODIFY
{
	M_ADD = 0,
	M_DELETE,
	M_REPLACE
};

enum TYPE_ORBIT
{
	ORBIT_DEFAULT = 0,
	ORBIT_BOOM,
	ORBIT_TIME_BOOM,
	ORBIT_BULLET
};

class OrbitObject : public EntityObject
{
private:
	Point p_OriginPoint;
	float p_Radius;
	EntityObject*		  p_Bubble;
	Vector<EntityObject*> p_ObjectDecorates;
	Vector<EntityObject*> p_ObjectInteractives;
	bool p_IsTheCharacterOnTheOrbit;

	int  p_Ratio;
	float p_PosXDead;

	bool		  p_IsMoveWithDirect;
	Point		  p_DirectToMoveWithDirect;
	float		  p_SpeedMoveWithDirect;
	float		  p_OriginSpeedMoveWithDirect;
	Vec2		  p_MaxDistanceMoveWithDirect;
	Vec2		  p_DeltaMoveDistance;


	bool		 p_IsRotateAroundAPoint;
	Vec2		 p_OriginPointRotateAround;
	float		 p_SpeedRotateAroundAPoint;

	int			 p_OrbitIdxStickTo;

	float p_OriginSpeedMove;

	//type orbit
	TYPE_ORBIT p_CurrentTypeOrbit;
	//
	bool p_isExplosive;
	//
	float p_OriginCDTime;
	float p_CurrentCDTime;
	bool  p_activeBomb;
public:
	OrbitObject();
	~OrbitObject();

	void FreeOrbit()
	{
		CC_SAFE_RELEASE_NULL(p_Bubble);
		for (int i = 0; i < p_ObjectDecorates.size(); i++)
		{
			p_ObjectDecorates.at(i)->FreeObject();
		}
		p_ObjectDecorates.clear();
		//
		for (int i = 0; i < p_ObjectInteractives.size(); i++)
		{
			p_ObjectInteractives.at(i)->FreeObject();
		}
		p_ObjectInteractives.clear();
		FreeObject();
	}
	//some public method//
	bool IsAlive()
	{
		if (GetPosObject().x <= p_PosXDead)
			return false;
		return true;
	}

	TYPE_ORBIT TypeOrbit() { return p_CurrentTypeOrbit; }
	bool IsExplosive() { return p_isExplosive; }
	void SetExplosive(bool b){ p_isExplosive = b; }

	bool IsMoveWithDirect() { return p_IsMoveWithDirect; }
	bool IsRotateAround() { return p_IsRotateAroundAPoint; }

	float GetSpeedRotateAroundAPoint() { return p_SpeedRotateAroundAPoint; }
	Vec2 GetPointRotateAround() { return p_OriginPointRotateAround; }
	bool IsOrbitTimeOnExplosion();

	int GetOrbitIdxStickTo() { return p_OrbitIdxStickTo; }

	void RotateOrbitAndComponentAroudAPoint(Vec2 point, float r);

	Vec2 GetDeltaDistanceMoveDirect() { return p_DeltaMoveDistance; }

	bool IsHaveStickedOrbit() { return p_OrbitIdxStickTo > -1; }

	float EffectRaius();

	bool IsTheCharacterOnTheOrbit()
	{
		return p_IsTheCharacterOnTheOrbit;
	}
	void IsTheCharacterOnTheOrbit(bool b)
	{
		p_IsTheCharacterOnTheOrbit = b;
	}

	void ActiveTheBoom(){ p_activeBomb = true; }

	float GetSpeedAroundRotate()
	{
		return p_SpeedAroundRotate;
	}
	Point GetOriginPoint()
	{
		p_OriginPoint = GetPosObject();
		return p_OriginPoint;
	}
	float GetRadius()
	{
		return p_Radius;
	}
	void SetSizeOrbit(Size s , int ratio = 1)
	{
		CCASSERT(s.width == s.height, "width and height must be equa !");
		s = s * GetScaleFactor();
		SetSizeObject(s);
		p_Radius = s.width / 2;
		if (TextureMgr->IsUseMipMap())
		{
			p_Radius = p_Radius * 2;
		}
		p_Ratio = ratio;
	}
	Size GetSizeOrbit()
	{
		return GetSizeObject();
	}
	void SetPosOrbit(Point p)
	{
		SetPosObject(p);
		p_OriginPoint = p;
	}
	Point GetPosOrbit()
	{
		return GetPosObject();
	}

	Vector<EntityObject*> GetListObject(TYPE_OBJECT type = DECORATE)
	{
		if (type == DECORATE)
			return p_ObjectDecorates;
		else if (type == INTERACTIVE)
			return p_ObjectInteractives;
        return p_ObjectInteractives;
	}

	EntityObject* GetObjectAtIndex(int idx, TYPE_OBJECT type = DECORATE)
	{
		if (type == DECORATE)
		{
			if (idx < p_ObjectDecorates.size())
				return p_ObjectDecorates.at(0);
		}
		else if (type == INTERACTIVE)
		{
			if (idx < p_ObjectInteractives.size())
				return p_ObjectInteractives.at(0);
		}
		return NULL;
	}
	void GenerateTypeObject(int * stateCreate = NULL);
	//
	void RemoveObjectAtIndex(int idx, TYPE_OBJECT type = DECORATE);
	void AddObject(EntityObject* obj, TYPE_OBJECT type = DECORATE);
	void ReplaceObjectAtIndex(int idx, EntityObject* obj, TYPE_OBJECT type = DECORATE);
	
	void LoadOrbitObject(int TypeOfLoad = 0, std::string name = "", Texture2D * t = NULL, Sprite * p = NULL, TYPE_ORBIT typeOrbit = TYPE_ORBIT::ORBIT_DEFAULT, ObjectGamePlayDetail * obj = NULL);
private:
	void LoadTexture(int TypeOfLoad = 0, std::string name = "", Texture2D * t = NULL, Sprite * p = NULL);
public:

	float GetRotation();
	void SetRotation(float r);

	void Update(float dt);

	void onDrawOrbit(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch = NULL);
	void onVisitOrbit(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch = NULL);

	void onDrawBubble(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch = NULL);
	void onVisitBubble(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch = NULL);

	void onDrawInteractiveObj(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch = NULL);
	void onVisitInteractiveObj(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch = NULL);

	void MoveOrbitToPoint(Point p);

	void UpdateMoveDirect(float dt);

	void SetPosOrbitWithDistance(float distance);
#if ENABLE_EDITOR
	void ToggleRotate(CONTROL_TYPE controlType);
#endif
	Point GenerateThePosObject();
	float CalculateAngle(Point p);

	float GetSpeedMoveOrbit(bool IsPreventNull = false);
};

#endif //_ORBIT_OBJECT_H_