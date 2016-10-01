#include "CloudObject.h"
ObjectCloud::ObjectCloud()
{
}
ObjectCloud::~ObjectCloud()
{
}

void ObjectCloud::InitTheCloudValue()
{
	InitTheValue("cloud");
}

void ObjectCloud::onDrawCloud(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch)
{
	onDraw(renderer, transform, flags,spriteBatch);
}
void ObjectCloud::onVisitCloud(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch)
{
	onVisit(renderer, transform, flags, spriteBatch);
}
void ObjectCloud::onUpdateCloud(float dt)
{
	onUpdate(dt);
}