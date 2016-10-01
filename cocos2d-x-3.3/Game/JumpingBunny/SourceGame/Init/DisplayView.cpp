#include "init/DisplayView.h"
#include "Utility/ObjectTexture.h"
cocos2d::Size visibleSize = cocos2d::Size(0, 0);
cocos2d::Size WinSize = cocos2d::Size(0, 0);
cocos2d::Point originPoint = cocos2d::Point(0, 0);
float ScaleFactorX = 1.f;
float ScaleFactorY = 1.f;
bool IsPressBackkey = false;
size_t p_cpuCount;
using namespace cocos2d;
void InitValue()
{
	 visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	 originPoint = cocos2d::Director::getInstance()->getVisibleOrigin();
	 WinSize = cocos2d::Director::getInstance()->getWinSize();
	 //
	 ScaleFactorX = WinSize.width / 800.f;
	 ScaleFactorY = WinSize.height / 480.f;
	 //
	 Initialize();
	 //
#ifdef WP8
	 SYSTEM_INFO sysinfo;
	 GetNativeSystemInfo(&sysinfo);
	 p_cpuCount=  sysinfo.dwNumberOfProcessors;
#elif defined OS_IOS
	 p_cpuCount = 2;
#else
    p_cpuCount = 4;
#endif
}
void LoadData()
{
	//
	LoadDataXML::GetInstance()->OnLoadXMLData();
	//
	ObjectTexture::GetInstance()->Load();
	//
	ObjectTexture::GetInstance()->ParseTextureToMemory();
}
float GetScaleFactor(bool Y)
{
	if (!Y)
		return ScaleFactorX;
	return ScaleFactorY;
}

float GetOffsetSCale()
{
	float ratioDesgin = 800.f / 480.f;
	float curRatio = GetWinSize().width / GetWinSize().height;
	float scaleoffset = 0.f;
	if (curRatio < ratioDesgin)
	{
		scaleoffset = (ratioDesgin - curRatio) / 2;
	}
	return scaleoffset;
}

cocos2d::Size GetVisibleSize()
{
	return visibleSize;
}
cocos2d::Point GetOriginPoint()
{
	return originPoint;
}
cocos2d::Point GetScreenCenter()
{
	return (cocos2d::Point(visibleSize / 2) + originPoint);
}
cocos2d::Point GetTextureCenterPos(MenuItemImage * m)
{
	return cocos2d::Point(m->getContentSize() / 2);
}
cocos2d::Size GetWinSize()
{
	return WinSize;
}
cocos2d::Point ConvertPTSCorToCCCor(cocos2d::Point P)
{
	return cocos2d::Point(P.x, WinSize.height - P.y);
}
cocos2d::Point GetViewPoint(cocos2d::Point TopLeft, cocos2d::Point CenterPos)
{
	cocos2d::Point p = ConvertPTSCorToCCCor(TopLeft);
	return cocos2d::Point(p.x + CenterPos.x, p.y - CenterPos.y);
}
float getTimeTick()
{
	timeval time;
	gettimeofday(&time, NULL);
	unsigned long millisecs = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	return (float)millisecs;
}
cocos2d::Point ConvertPos(cocos2d::Point ps)
{
	cocos2d::Point p(0, 0);
#ifdef USE_DX_COR
	p.y = 1 - ps.y;
	p.x = ps.x;
#endif
	return p;
}

cocos2d::Point GravityVector()
{
	return cocos2d::Point(0, 1);
}

int GetRandom(int num1, int num2)
{
	int max = MAX(num1, num2);
	int min = MIN(num1, num2);
	int returnvalue = min;
	if (max == min)
	{
		min = 0;
		return max;
	}
	returnvalue = ((rand() % (max - min)) + min);
	return returnvalue;
}
#ifdef WP8
std::string CCUnicodeToUtf8(const wchar_t* pwszStr)
{
	std::string ret = "";
	do
	{
		if (!pwszStr) break;
		size_t len = wcslen(pwszStr);
		if (len <= 0) break;
		size_t convertedChars = 0;
		char * pszUtf8Str = new char[len * 3 + 1];
		WideCharToMultiByte(CP_UTF8, 0, pwszStr, len + 1, pszUtf8Str, len * 3 + 1, 0, 0);
		ret = pszUtf8Str;
		CC_SAFE_DELETE_ARRAY(pszUtf8Str);
	} while (0);

	return ret;
}

std::wstring CCUtf8ToUnicode(const char * pszUtf8Str, unsigned len/* = -1*/)
{
	std::wstring ret;
	do
	{
		if (!pszUtf8Str) break;
		// get UTF8 string length
		if (-1 == len)
		{
			len = (int)strlen(pszUtf8Str);
		}
		if (len <= 0) break;
		// get UTF16 string length
		int wLen = MultiByteToWideChar(CP_UTF8, 0, pszUtf8Str, len, 0, 0);
		if (0 == wLen || 0xFFFD == wLen) break;
		// convert string
		wchar_t * pwszStr = new wchar_t[wLen + 1];
		if (!pwszStr) break;
		pwszStr[wLen] = 0;
		MultiByteToWideChar(CP_UTF8, 0, pszUtf8Str, len, pwszStr, wLen + 1);
		ret = pwszStr;
		CC_SAFE_DELETE_ARRAY(pwszStr);
	} while (0);

	return ret;
}
#endif

bool IsBackKeyPress()
{
	return IsPressBackkey;
}
void SetBackkey(bool b)
{
	IsPressBackkey = b;
}

#if ENABLE_EDITOR
bool p_HaveObjectSelected = false;

bool HaveObjectSelected()
{
	return p_HaveObjectSelected;
}
void HaveObjectSelected(bool b)
{
	p_HaveObjectSelected = b;
}
#endif

std::string GetPlatForm()
{
	std::string type = "";
#if defined W8 || defined WP8
	type = "WP8";
#elif defined WIN32
	type = "WIN32";
#elif defined IOS
	type = "IOS";
#elif defined ANDROID
	type = "ANDROID";
#endif
	return type;
}
bool IsDeviceLowMem()
{
#ifdef WP8
	long mem = Windows::Phone::System::Memory::MemoryManager::ProcessCommittedLimit / 1000000;
	if (mem < 190)
	{
		return true;
	}
#endif
	return false;
}

uint32 GetCpuCoreCount()
{
	return (uint32)p_cpuCount;
}
#ifdef WP8
extern "C"
{
	__declspec(dllimport) float g_framerate;
}
#elif defined OS_IOS
extern "C" float g_framerate;
#endif

float SyncWithFPS(float d)
{
	return d / 2.f;
}

std::string convertToDisplayTime(int timePlay)
{
	std::string minute = "";
	std::string sec = "";
	int numberMinute = timePlay / 60;
	if (numberMinute < 10)
	{
		minute = "0" + std::to_string(numberMinute);
	}
	else
	{
		minute = std::to_string(numberMinute);
	}

	int numberSecond = timePlay % 60;
	if (numberSecond < 10)
	{
		sec = "0" + std::to_string(numberSecond);
	}
	else
	{
		sec = std::to_string(numberSecond);
	}

	return (minute + std::string(":") + sec);
}