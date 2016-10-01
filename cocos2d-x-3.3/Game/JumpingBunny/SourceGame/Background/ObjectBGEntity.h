#ifndef __OBJECT_BG_ENTITY_H__
#define __OBJECT_BG_ENTITY_H__

#include "Init/DisplayView.h"
#include "Init/ParamTexture.h"
#include "cocos2d.h"
USING_NS_CC;

class BGEntity : public Ref
{
protected:
	Sprite*		p_Resource;
	Point		p_dirMove;
	bool		p_IsVisible;
	float		p_Color[4];
	float		p_SpeedMove;
	Point		p_PointToMoveTo;
	Point		p_VectorMove;

	float       p_PointAlive;

	bool		p_IsFree;
public:
	int IDTag;
	Sprite* GetSprite() { return p_Resource; }

	BGEntity();
	~BGEntity();

	void SetScaleBG(Point s);
	Point GetScaleBG();

	bool VisibleBG();
	void VisibleBG(bool b);

	Point GetSizeBG();
	void SetSizeBG(Point s);

	void SetPositionBG(Point p);
	Point GetPositionBG();

	void onVisitBG(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch = NULL);
	void onDrawBG(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch = NULL);
	void onUpdateBG(float dt);

	void MoveToPoint(Point p);

	void SpeedMove(float s) { p_SpeedMove = s; }
	void DirMove(Point p) { p_dirMove = p; }
	Point DirMove() { return p_dirMove; }
	void PointAlive(float p) { p_PointAlive = p; }
	float PointAlive() { return p_PointAlive; }
	bool IsAlive();
	bool IsFree() { return p_IsFree; }
	void IsFree(bool b) { p_IsFree = b; }
public:
	void LoadBGFromSingleImage(std::string name);
	void LoadBGFromSingleTexture(Texture2D * tx);
	void LoadBGFromData(TYPE_TEXTURE t, std::string str);
	void parseSprite(Sprite * p);
};

#endif //__OBJECT_BG_ENTITY_H__

