#ifndef __CLOUD_OBJECT_H__
#define __CLOUD_OBJECT_H__

#include "BGObject.h"

class ObjectCloud : public BGObject
{
public:
	ObjectCloud();
	virtual ~ObjectCloud();

	void InitTheCloudValue();

	virtual void onDrawCloud(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch = NULL);
	virtual void onVisitCloud(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch = NULL);
	virtual void onUpdateCloud(float dt);

};

#endif //__CLOUD_OBJECT_H__

