#ifndef _DISPLAY_VIEW_H_
#define _DISPLAY_VIEW_H_
#include "cocos2d.h"
using namespace cocos2d;
#ifdef _DEBUG
#define ENABLE_EDITOR 0
#else
#define ENABLE_EDITOR 0
#endif
#define USE_ANTIALIAS 0
//simulate dx cor on opengl, no-need on directX, fix for design from photoshop
#define USE_DX_COR 1
#define USE_ARK_ARCHIVE 1
//#define USE_STREAK_TEX 1
#define USE_CC_AUDIO 1
//#define USE_SPRITE_TEXTURE_CACHE 
//#define USE_FONT_TEXTURE_NUMBER
#ifdef OS_IOS
#include "Init/../../iOS/IOSCode/DisplayView_IOS.h"
typedef unsigned int uint32;
#endif
#define USE_HALF_LEVEL
#define MAXIMUM_STAR 3
#define NUMBER_LEVEL 48
#ifdef USE_HALF_LEVEL
#define NUMBER_LEVEL_PER_PAGE 6
#define NUMBER_LEVEL_ROW 3
#else
#define NUMBER_LEVEL_PER_PAGE 12
#define NUMBER_LEVEL_ROW 4
#endif
#define NUMBER_LEVEL_PER_SESSION 12
static std::string GamefontList[] =
{
	"gamefonts/JandaManateeSolid.ttf",
	"gamefonts/Big_Bimbo_NC_0.ttf",
	"gamefonts/Cookies.ttf",
	"gamefonts/grobold.ttf",
	"gamefonts/SAF.ttf",
	"gamefonts/arial.ttf",
	"gamefonts/comic.ttf"
};

int GetRandom(int num1, int num2);
Point ConvertPos(Point ps);
Point GravityVector();
#include "Utility/ObjectTexture.h"
Point ConvertPTSCorToCCCor(Point P);
Point GetViewPoint(Point TopLeft, Point CenterPos);
void InitValue();
void LoadData();
Size GetVisibleSize();
Point GetOriginPoint();
Point GetScreenCenter();
Point GetTextureCenterPos(MenuItemImage * m);
Size  GetWinSize();
//
float GetScaleFactor(bool Y = false);
float GetOffsetSCale();
//in milisecond
float getTimeTick(); 
//backkey
bool IsBackKeyPress();
void SetBackkey(bool b);
//use in the editor
#if ENABLE_EDITOR 
bool HaveObjectSelected();
void HaveObjectSelected(bool b);
#endif //ENABLE_EDITOR
//convert string
#ifdef WP8
std::string CCUnicodeToUtf8(const wchar_t* pwszStr);
std::wstring CCUtf8ToUnicode(const char * pszUtf8Str, unsigned len/* = -1*/);
#endif
//get platform
std::string GetPlatForm();
//getmempeak
bool IsDeviceLowMem();
uint32 GetCpuCoreCount();
//sync with fps
float SyncWithFPS(float d);
//convert to display time
std::string convertToDisplayTime(int timePlay);

#endif //_DISPLAY_VIEW_H_