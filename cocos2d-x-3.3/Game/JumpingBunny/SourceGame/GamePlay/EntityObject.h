#ifndef _ENTITY_OBJECT_H_
#define _ENTITY_OBJECT_H_

#include "cocos2d.h"
#include "Init/DisplayView.h"
#include "Control/InputManager.h"
#include "cocostudio/CocoStudio.h"
using namespace cocostudio;
USING_NS_CC;

static std::string fontList[] =
{
#if ((CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC))
	// custom ttf files are defined in Test-info.plist
	"American Typewriter",
	"Marker Felt",
	"A Damn Mess",
	"Abberancy",
	"Abduction",
	"Paint Boy",
	"Schwarzwald Regular",
	"Scissor Cuts",
#else
	"fonts/arial.ttf",
	"fonts/A Damn Mess.ttf",
	"fonts/Abberancy.ttf",
	"fonts/Abduction.ttf",
	"fonts/Paint Boy.ttf",
	"fonts/Schwarzwald Regular.ttf",
	"fonts/Scissor Cuts.ttf",
#endif
};
static int fontCount = sizeof(fontList) / sizeof(*fontList);
enum STATUS_ACTION
{
	IDLE = 0,
	MOVE = 1,
	ROTATE = 10,
	SELF_ROTATE = 100,
	MOVE_BEZIER = 1000
};

enum ACTION_TAG
{
	AC_NONE = 0,
	AC_ANIMATE,
	AC_COLOR_REPEAT
};

class EntityObject : public Ref
{
protected:
	Animation *	  p_Animate;
	Sprite *	  p_Object;
	CCArmature *  p_Armature;//animation action from editor;

	int		      p_numberSprite;
	float	      p_DeltaTimeAnimate;
	//
	std::vector<STATUS_ACTION> p_ListAction;
	Point		  p_PointToMoveTo;
	float		  p_DurationMove;
	Point		  p_VectorDirectMove;
	//
	float		  p_SpeedSeftRotate;
	float		  p_SpeedAroundRotate;
	float		  p_SpeedMove;
	//
	bool		  p_IsRotate;
	bool		  p_IsStopMove;
	//
	bool		  p_IsUseAnimatEditor;
	//
	Node	   *  GetObject();
	//
	BYTE	   *  p_currentTint;	
#if ENABLE_EDITOR
	Sprite*		  p_HL;
	Label*	  p_DisplayString;
	bool		  p_IsTargetSelected;
#endif

public:
	EntityObject();
	~EntityObject();
	void FreeObject()
	{

		CC_SAFE_RELEASE(p_Animate);
//#if ! USE_SPRITE_STORE
		if (p_Object)
		{
			p_Object->removeAllChildrenWithCleanup(true);
		}
		CC_SAFE_RELEASE_NULL(p_Object);
//#endif
		CC_SAFE_RELEASE_NULL(p_Armature);
#if ENABLE_EDITOR
		CC_SAFE_RELEASE_NULL(p_HL);
		CC_SAFE_RELEASE_NULL(p_DisplayString);
#endif
	}
#if ENABLE_EDITOR
	bool	IsTargetSelected();
	void    IsTargetSelected(bool b);
	void	InitParam();
	void	UpdateString(float dt);
	void	DrawString(Renderer *renderer, const Mat4& transform, uint32_t flags);
	bool	IsClickOnTarget(Point p);
	void    SeftRotate(CONTROL_TYPE controlType = CONTROL_TYPE::NONE);
#endif

//----------------get set method------------------//
	void    Flip(bool b)
	{
		if (p_IsUseAnimatEditor)
		{
			float cur_scale_x = p_Armature->getScaleX();
			p_Armature->setScaleX(b ? -abs(cur_scale_x) : abs(cur_scale_x));
			return;
		}
		CCASSERT(p_Object, "object sprite invaid");
		p_Object->setFlippedX(b);
	}

	BYTE * GetTint();

	void SetTint(BYTE * p);
	void SetTint(Color3B color);

	BYTE GetTintChannel(int channel);

	Vec2 GetContentSize() { return GetObject()->getContentSize(); }

	bool    IsRotate()
	{
		return p_IsRotate;
	}
	void	IsRotate(bool b)
	{
		p_IsRotate = b;
	}

	void		  SpeedSeftRotate(float s)
	{
		p_SpeedSeftRotate = s;
	}
	void		  SpeedAroundRotate(float s)
	{
		p_SpeedAroundRotate = s;
	}

	void		  SpeedMove(float s)
	{
		p_SpeedMove = s;
	}
	float		  SpeedMove()
	{
		return p_SpeedMove;
	}

	bool		 IsStopMove()
	{
		return p_IsStopMove;
	}

	void		DurationMove(float d)
	{
		p_DurationMove = d;
	}

	Point		PointToMoveTo()
	{
		return p_PointToMoveTo;
	}

	void	SetSizeObject(Size s, bool notMip = false);
	Size	GetSizeObject(bool notMip = false);

	void	SetPosObject(Point p);
	Point	GetPosObject();

	void	SetRectObject(Rect r);
	Rect	GetRectObject();

	void	SetRotateObject(float r);
	float	GetRotateObject();

	void	SetRotateAroundAPoint(Point p, float r, float distance = 0);

	int GetNumberObject();

	void MoveToPoint(Point p);
//
#if ENABLE_EDITOR
	void UpdateControlEdit();
#endif
	virtual void onDrawObject(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch = NULL);
	virtual void onVisitObject(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch = NULL);
	void updateObject(float dt, bool UpdatePosition = true);
//calculate seft rotate and rotate around an entity
	void CalculateRotateAroundObject(float dt,EntityObject * e, CONTROL_TYPE controlType = CONTROL_TYPE::NONE);
	void CalculateSelfRotateObject(float dt, EntityObject * e, CONTROL_TYPE controlType = CONTROL_TYPE::NONE);
	void CalculateSelfRotateAPoint(float dt, float r, CONTROL_TYPE controlType = CONTROL_TYPE::NONE);
//normalize vector
	Point NormalizeVector(Point p)
	{
		float leng = sqrt(p.x * p.x + p.y * p.y);
		return Point(p.x / leng, p.y / leng);
	}
	float AngleBy2Vector(Point a, Point b)
	{

		float cos_a = ((a.x*b.x) + (a.y*b.y)) /
			((sqrtf(a.x*a.x + a.y*a.y))*(sqrtf(b.x*b.x + b.y*b.y)));
		//	

		float valueReturn = acosf(cos_a);
		valueReturn = (valueReturn * 180 / M_PI);

		return valueReturn;
	}
public:
	void LoadObjectFromData(TYPE_TEXTURE t, std::string str);
	void LoadObjectFromSingleTexture(Texture2D * tx);
	void LoadObjectFromSingleImage(std::string name);
	void LoadObjectFromSprite(Sprite * p, bool UseAntiAlias = true);
	void LoadAnimateFromSpriteList(Vector<Sprite *> p_list);
	virtual void LoadAnimateFromEditor() {};
};
#endif
