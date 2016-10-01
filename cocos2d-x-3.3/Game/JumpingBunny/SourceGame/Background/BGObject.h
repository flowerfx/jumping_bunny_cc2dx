#ifndef __BG_OBJECT_H__
#define __BG_OBJECT_H__

#include "ObjectBGEntity.h"
class BGLayer : public Ref
{
protected:
	Vector<BGEntity*>				p_objectLayer;
	std::string p_nameBG;
	std::string p_nametype;
public:
	BGLayer();
	~BGLayer();

	void InitTheValue(std::string nameLayer, std::string nameType);

	void onDraw(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch = NULL);
	void onVisit(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch = NULL);
	void onUpdate(float dt);

	void onRemoveBG();

	BGEntity* CreateObject(std::string nameLayer, std::string nameType, BGEntity* previousC = NULL);
	void CalculateThePosObject(BGEntity * previousC, BGEntity* t, Point ratioPosY,Point RatioPosX, std::string name, std::string type);

	std::string GetNameType() { return p_nametype; }
};

class BGObject : public Ref
{
protected:
	Vector<BGLayer*>				p_ListBG;
	std::string p_nameBG;
public:
	BGObject();
	virtual ~BGObject();

	void InitTheValue(std::string name);

	virtual void onDraw(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch = NULL);
	virtual void onVisit(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch = NULL);
	virtual void onUpdate(float dt);

	void onRemoveBG();
};

#endif //__BG_OBJECT_H__

