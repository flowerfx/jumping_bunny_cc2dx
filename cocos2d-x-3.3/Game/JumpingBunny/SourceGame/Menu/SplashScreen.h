#ifndef __SPLASH_SCREEN_H__
#define __SPLASH_SCREEN_H__

#include "Menu/MenuEntity.h"
#include "Menu/MenuManager.h"

class SplashScreen : public MenuEntity
{

public:
	SplashScreen();
	~SplashScreen();
	void Init();
	void Update(float dt);
	void Draw(SpriteBatchNode * spriteBatch = NULL);
	void Draw(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch = NULL);
};
#endif //__SPLASH_SCREEN_H__