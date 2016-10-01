#ifndef _OBJECT_TEXTURE_H_
#define _OBJECT_TEXTURE_H_
#pragma once
#include "CCGeometry.h"
#include "CCVector.h"
#include "cocos2d.h"
#include "CCTexture2D.h"
#include "LoadDataXML.h"
USING_NS_CC;
#define USE_SPRITE_STORE 1
class ObjectTexture
{
public:
	void Load(bool loadAA = false);
	void UnloadData();
	Texture2D * LoadTextureFromSprite(TYPE_TEXTURE type, std::string NameTexture, int idx = 0);
	Sprite *	   LoadSpriteFromTexture(TYPE_TEXTURE type, std::string NameTexture, int idx = 0 );

	Texture2D * GetTextureFromData(TYPE_TEXTURE type);
	Sprite	  * GetSpriteFromData(TYPE_TEXTURE type);

	Texture2D * GetTexureFromMemory(std::string NameTexture);
	Sprite	  * GetSpriteFromMemory(std::string NameTexture);
	Sprite    * GetFontNumber(int num, int idx = 0);

	Image * GetGlobalSprite(TYPE_TEXTURE type);

	void	ParseTextureToMemory();
	void	ParseSpriteToMemory();

	void    ParseSpriteFontToMemory();

	Sprite * GetSpriteFromTextureCache(std::string NameTexture);
	Sprite * GetFontNumberFromCache(std::string NameTexture);

	Label * GetFontData(int idx);
private:
	unsigned char *  GetDataSpecific(Rect bound, Image* im);
	//raw data;
	Image * p_AtlatSpriteObject;
	Image * p_AtlatMenuItem;
	Image * p_AtlatButtonItem;
	Image * p_AtlatMainChar;
	//
	Texture2D* p_AltastTextureSpriteObject;
	Texture2D* p_AtlatTextureMenuItem;
	Texture2D* p_AtlatTextureButtonItem;
	//
	Texture2D* P_Streak;// streak texture will be loaded in another texture
	//
	Vector<Label *> p_list_string_atlats;
	//
	std::map<std::string, Texture2D*> p_ListSpriteIG;
	Map<std::string, Sprite*>	  p_ListSpriteMemory;
	Map<std::string, Sprite*>	  p_ListCharacterFont;

	std::map<std::string, TYPE_TEXTURE> p_ListIdxNameSprite;
	std::map<std::string, TYPE_TEXTURE> p_ListIdxFontSprite;

	static ObjectTexture * p_Instance;
	bool	p_useMipMap;
public:
	static ObjectTexture * GetInstance()
	{
		if (!p_Instance)
		{
			p_Instance = new ObjectTexture();
		}
		return p_Instance;
	}
	ObjectTexture();
	~ObjectTexture();

	bool IsUseMipMap() { return p_useMipMap; }
};
#define TextureMgr ObjectTexture::GetInstance()
#endif