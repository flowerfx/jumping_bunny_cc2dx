#include "SplashScreen.h"
using namespace cocos2d;

extern int LoadingState;
SplashScreen::SplashScreen()
{
}


SplashScreen::~SplashScreen()
{
}

void SplashScreen::Init()
{
	BackgroundEntity * bg = new BackgroundEntity();

	auto pos = Point(GetWinSize().width * 0.5f, GetWinSize().height * 0.5f);
	auto scale = Point(0.4f,0.375f);
	bg->Init("logo", ResolveNamePath(std::string(TEXTURE_PATH) + std::string(TEXTURE_LOGO)), pos, scale, Point(0, 0), 0);
	p_ListBackground.pushBack(bg);
}


void SplashScreen::Update(float dt)
{
	
}
void SplashScreen::Draw(SpriteBatchNode * spriteBatch)
{
	onDraw(spriteBatch);
}
void SplashScreen::Draw(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch)
{
	onDraw(renderer, transform, flags, spriteBatch);
}