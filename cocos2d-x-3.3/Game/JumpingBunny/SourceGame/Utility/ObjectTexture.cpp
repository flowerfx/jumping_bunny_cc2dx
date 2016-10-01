#include "ObjectTexture.h"
#include "Init/DisplayView.h"
ObjectTexture * ObjectTexture::p_Instance = NULL;
ObjectTexture::ObjectTexture()
{
	p_useMipMap = false;
    
    p_AltastTextureSpriteObject = NULL;
    p_AtlatTextureMenuItem = NULL;
    p_AtlatTextureButtonItem = NULL;

	p_AtlatSpriteObject = NULL;
	p_AtlatMenuItem = NULL;
	p_AtlatButtonItem = NULL;
	p_AtlatMainChar = NULL;

	P_Streak = NULL;
}

///////////////////////////////////////////////////////////////////////////////
ObjectTexture::~ObjectTexture()
{
	UnloadData();
}

///////////////////////////////////////////////////////////////////////////////
Image * ObjectTexture::GetGlobalSprite(TYPE_TEXTURE type)
{
	if (type == TYPE_TEXTURE::OBJECT_TEXTURE || type == TYPE_TEXTURE::OBJECT_CHAR || type == TYPE_TEXTURE::OBJECT_EFFECT || type == TYPE_TEXTURE::FONT_TEXTURE)
	{
		return p_AtlatSpriteObject;
	}
	else if (type == TYPE_TEXTURE::MENU_TEXRURE)
	{
		return p_AtlatButtonItem;
	}
	else if (type == TYPE_TEXTURE::MENU_LAYOUT)
	{
		return p_AtlatMenuItem;
	}
	else if (type == TYPE_TEXTURE::OBJECT_CHAR)
	{
		return p_AtlatMainChar;
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
Sprite * ObjectTexture::LoadSpriteFromTexture(TYPE_TEXTURE type, std::string NameTexture, int idx)
{
	DataObject * data = LoadDataXML::GetInstance()->GetDataObject(type);
	if (!data)
		return 0;
	DetailSize dSize = data->GetDataSize(NameTexture);

	//Rect bound = Rect(
	//	dSize.position.x,
	//	dSize.position.y,
	//	dSize.size.width / CC_CONTENT_SCALE_FACTOR(),
	//	dSize.size.height / CC_CONTENT_SCALE_FACTOR());
	Rect bound;
	if ((size_t)idx < dSize.Bounds.size())
		bound = dSize.Bounds.at(idx);
	else
		bound = dSize.Bounds.at(dSize.numberRect - 1);

	if (p_useMipMap)
	{
		dSize.size = Size((int)(dSize.size.width / 2.f), (int)(dSize.size.height / 2.f));
		dSize.Bounds.at(idx) = Rect((int)(bound.getMinX() / 2.f), (int)(bound.getMinY() / 2.f), (int)((bound.getMaxX() - bound.getMinX()) / 2.f), (int)((bound.getMaxY() - bound.getMinY()) / 2.f));
	}

	Texture2D * t = GetTextureFromData(type);
	data = NULL;
	//t->autorelease();
	auto sprite =  Sprite::createWithTexture(t, dSize.Bounds.at(idx));
	t = NULL;
	return sprite;
}

///////////////////////////////////////////////////////////////////////////////
Texture2D * ObjectTexture::LoadTextureFromSprite(TYPE_TEXTURE type, std::string NameTexture, int idx)
{
	Image * sprite_t = GetGlobalSprite(type);
	DataObject * data = LoadDataXML::GetInstance()->GetDataObject(type);
	if (!data)
		return 0;
	Texture2D * m_temp = new Texture2D();
	DetailSize dSize = data->GetDataSize(NameTexture);
	Rect bound = dSize.Bounds.at(idx);
	if (p_useMipMap && (sprite_t->getNumberOfMipmaps() > 0 || sprite_t->getFileType() == Image::Format::PNG))
	{
		dSize.size = Size((int)(dSize.size.width / 2.f), (int)(dSize.size.height / 2.f));
		dSize.Bounds.at(idx) = Rect((int)(bound.getMinX() / 2.f), (int)(bound.getMinY() / 2.f), (int)((bound.getMaxX() - bound.getMinX()) / 2.f), (int)((bound.getMaxY() - bound.getMinY()) / 2.f));
	}
	int ElementCount = (int)(dSize.size.width * dSize.size.height);
	if ((size_t)idx >= dSize.Bounds.size())
	{
		idx = (int)dSize.Bounds.size() - 1;
	}
	//Texture2D * tex = new Texture2D();
	//tex->initWithImage(sprite_t);
	//Sprite * s = Sprite::createWithTexture(tex, dSize.Bounds.at(idx));
	//auto t = s->setTe

	unsigned char* data_i = GetDataSpecific(dSize.Bounds.at(idx), sprite_t);
	m_temp->initWithData(data_i, ElementCount * 4, sprite_t->getRenderFormat(), dSize.size.width, dSize.size.height, dSize.size);	
	m_temp->autorelease();
	delete data_i;
	data_i = NULL;
	return m_temp;
}

///////////////////////////////////////////////////////////////////////////////
Texture2D * ObjectTexture::GetTextureFromData(TYPE_TEXTURE type)
{
#if USE_SPRITE_STORE
	if (type == TYPE_TEXTURE::OBJECT_TEXTURE || type == TYPE_TEXTURE::OBJECT_CHAR || type == TYPE_TEXTURE::OBJECT_EFFECT)
	{
		return p_AltastTextureSpriteObject;
	}
	else if (type == TYPE_TEXTURE::MENU_TEXRURE)
	{
		return p_AtlatTextureButtonItem;
	}
	else if (type == TYPE_TEXTURE::MENU_LAYOUT)
	{
		return p_AtlatTextureMenuItem;
	}
	return 0;
#else
	Image * sprite_t = GetGlobalSprite(type);
	Texture2D * m_temp = new Texture2D();
	unsigned char * dataImg = sprite_t->getData();
	m_temp->initWithData(dataImg, sprite_t->getDataLen(), sprite_t->getRenderFormat(), sprite_t->getWidth(), sprite_t->getHeight(), Size(sprite_t->getWidth(), sprite_t->getHeight()));
	m_temp->autorelease();

	return m_temp;
#endif
}

///////////////////////////////////////////////////////////////////////////////
Sprite	  * ObjectTexture::GetSpriteFromData(TYPE_TEXTURE type)
{
	Image * sprite_t = GetGlobalSprite(type);
	Texture2D * m_temp = new Texture2D();
	unsigned char * dataImg = sprite_t->getData();
	m_temp->initWithData(dataImg, sprite_t->getDataLen(), sprite_t->getRenderFormat(), sprite_t->getWidth(), sprite_t->getHeight(), Size(sprite_t->getWidth(), sprite_t->getHeight()));
	m_temp->autorelease();

	Sprite * sprite = Sprite::createWithTexture(m_temp);
	return sprite;
}

///////////////////////////////////////////////////////////////////////////////
Texture2D * ObjectTexture::GetTexureFromMemory(std::string NameTexture)
{
	return p_ListSpriteIG.find(NameTexture)->second;
}

///////////////////////////////////////////////////////////////////////////////
Sprite * ObjectTexture::GetSpriteFromMemory(std::string NameTexture)
{
#ifdef USE_SPRITE_TEXTURE_CACHE
	return GetSpriteFromTextureCache(NameTexture);
#else
	return p_ListSpriteMemory.find(NameTexture)->second;
#endif
}

///////////////////////////////////////////////////////////////////////////////
Sprite  * ObjectTexture::GetFontNumber(int num, int idx)
{
	std::string name = std::to_string(num) + std::string("_") + std::to_string(idx);
#ifdef USE_SPRITE_TEXTURE_CACHE
	return GetFontNumberFromCache(name);
#else
	return p_ListCharacterFont.find(name)->second;
#endif
}

///////////////////////////////////////////////////////////////////////////////
void ObjectTexture::UnloadData()
{
#if USE_SPRITE_STORE
	CC_SAFE_RELEASE_NULL(p_AltastTextureSpriteObject);
	CC_SAFE_RELEASE_NULL(p_AtlatTextureMenuItem);
	CC_SAFE_RELEASE_NULL(p_AtlatTextureButtonItem);
	CC_SAFE_RELEASE_NULL(P_Streak);
#else
	CC_SAFE_RELEASE_NULL((p_AtlatSpriteObject);
	CC_SAFE_RELEASE_NULL((p_AtlatButtonItem);
	CC_SAFE_RELEASE_NULL((p_AtlatMenuItem);
	CC_SAFE_RELEASE_NULL((p_AtlatMainChar);
#endif
	p_list_string_atlats.clear();
}

///////////////////////////////////////////////////////////////////////////////
void ObjectTexture::Load(bool loadAA)
{
	bool IsUseArk = false;
#ifdef USE_ARK_ARCHIVE
	IsUseArk = true;
#endif
	if (XMLMgr->GetHarwardProfile() == HARDWARD_PROFILE::PHIGHT || XMLMgr->GetHarwardProfile() == HARDWARD_PROFILE::PMED)
	{
		p_useMipMap = false;
	}
	else
	{
		p_useMipMap = true;
	}
	std::string ImagePath = "";
	auto dData = LoadDataXML::GetInstance()->GetDataObject(TYPE_TEXTURE::OBJECT_TEXTURE);
	if (dData)
	{
		std::string namepath = dData->NameImage;
		if (p_useMipMap && namepath.find(".png") != std::string::npos)
		{
			namepath = GenerateMipName(namepath);
		}
		ImagePath = ResolveNamePath(namepath);
#ifdef USE_SPRITE_STORE
		Image * p_AtlatSpriteObjectTemp = NULL;
		p_AtlatSpriteObjectTemp = new Image();
		p_AtlatSpriteObjectTemp->initWithImageFile(IsUseArk ? namepath : ImagePath, IsUseArk);
		//
		if (p_AltastTextureSpriteObject == NULL)
		{
			p_AltastTextureSpriteObject = new Texture2D();
		}
		if (p_useMipMap && p_AtlatSpriteObjectTemp->getNumberOfMipmaps() > 1)
		{
			p_AltastTextureSpriteObject->initWithData(p_AtlatSpriteObjectTemp->getMipmaps()[1].address, p_AtlatSpriteObjectTemp->getMipmaps()[1].len,
				p_AtlatSpriteObjectTemp->getRenderFormat(), p_AtlatSpriteObjectTemp->getWidth() / 2, p_AtlatSpriteObjectTemp->getHeight() / 2,
				Size(p_AtlatSpriteObjectTemp->getWidth() / 2, p_AtlatSpriteObjectTemp->getHeight() / 2)
				);
		}
		else
		{
			if (p_AtlatSpriteObjectTemp->getNumberOfMipmaps() <= 1)
			{
				p_AltastTextureSpriteObject->initWithImage(p_AtlatSpriteObjectTemp);
			}
			else
			{
				p_AltastTextureSpriteObject->initWithData(p_AtlatSpriteObjectTemp->getMipmaps()[0].address, p_AtlatSpriteObjectTemp->getMipmaps()[0].len,
					p_AtlatSpriteObjectTemp->getRenderFormat(), p_AtlatSpriteObjectTemp->getWidth(), p_AtlatSpriteObjectTemp->getHeight(),
					Size(p_AtlatSpriteObjectTemp->getWidth(), p_AtlatSpriteObjectTemp->getHeight())
					);
			}
		}
#ifdef USE_ANTIALIAS
		if(loadAA)
		{
			p_AltastTextureSpriteObject->generateMipmap();
			Texture2D::TexParams texParams = { GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE };
			p_AltastTextureSpriteObject->setTexParameters(&texParams);
		}
#endif
		CC_SAFE_RELEASE_NULL(p_AtlatSpriteObjectTemp);
		dData = NULL;
#else
		p_AtlatSpriteObject = new Image();
		p_AtlatSpriteObject->initWithImageFile(IsUseArk ? namepath : ImagePath, IsUseArk);
#endif
	}
	//dData = LoadDataXML::GetInstance()->GetDataObject(TYPE_TEXTURE::OBJECT_CHAR);
	//if (dData)
	//{
	//	ImagePath = ResolveNamePath(dData->NameImage);
	//	p_AtlatMainChar = new Image();
	//	p_AtlatMainChar->initWithImageFile(ImagePath);
	//}

	dData = LoadDataXML::GetInstance()->GetDataObject(TYPE_TEXTURE::MENU_TEXRURE);
	if (dData)
	{
		std::string namepath = dData->NameImage;
		if (p_useMipMap && namepath.find(".png") != std::string::npos)
		{
			namepath = GenerateMipName(namepath);
		}
		ImagePath = ResolveNamePath(namepath);
#ifdef USE_SPRITE_STORE
		auto p_AtlatButtonItemTemp = new Image();
		p_AtlatButtonItemTemp->initWithImageFile(IsUseArk ? namepath : ImagePath, IsUseArk);
		if (p_AtlatTextureButtonItem == NULL)
		{
			p_AtlatTextureButtonItem = new Texture2D();
		}
		if (p_useMipMap && p_AtlatButtonItemTemp->getNumberOfMipmaps() > 1)
		{
			p_AtlatTextureButtonItem->initWithData(p_AtlatButtonItemTemp->getMipmaps()[1].address, p_AtlatButtonItemTemp->getMipmaps()[1].len,
				p_AtlatButtonItemTemp->getRenderFormat(), p_AtlatButtonItemTemp->getWidth() / 2, p_AtlatButtonItemTemp->getHeight() / 2,
				Size(p_AtlatButtonItemTemp->getWidth() / 2, p_AtlatButtonItemTemp->getHeight() / 2)
				);
		}
		else
		{
			if (p_AtlatButtonItemTemp->getNumberOfMipmaps() <= 1)
			{
				p_AtlatTextureButtonItem->initWithImage(p_AtlatButtonItemTemp);
			}
			else
			{
				p_AtlatTextureButtonItem->initWithData(p_AtlatButtonItemTemp->getMipmaps()[0].address, p_AtlatButtonItemTemp->getMipmaps()[0].len,
					p_AtlatButtonItemTemp->getRenderFormat(), p_AtlatButtonItemTemp->getWidth(), p_AtlatButtonItemTemp->getHeight(),
					Size(p_AtlatButtonItemTemp->getWidth(), p_AtlatButtonItemTemp->getHeight())
					);
			}
		}
#ifdef USE_ANTIALIAS
		if(loadAA)
		{
			p_AtlatTextureButtonItem->generateMipmap();
			Texture2D::TexParams texParams = { GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE };
			p_AtlatTextureButtonItem->setTexParameters(&texParams);
        }
#endif
		CC_SAFE_RELEASE_NULL(p_AtlatButtonItemTemp);
		dData = NULL;
#else
		p_AtlatButtonItem = new Image();
		p_AtlatButtonItem->initWithImageFile(IsUseArk ? namepath : ImagePath, IsUseArk);
#endif
	}

	dData = LoadDataXML::GetInstance()->GetDataObject(TYPE_TEXTURE::MENU_LAYOUT);
	if (dData)
	{
		std::string namepath = dData->NameImage;
		if (p_useMipMap && namepath.find(".png") != std::string::npos)
		{
			namepath = GenerateMipName(namepath);
		}
		ImagePath = ResolveNamePath(namepath);
#ifdef USE_SPRITE_STORE
		auto p_AtlatMenuItemTemp = new Image();
		p_AtlatMenuItemTemp->initWithImageFile(IsUseArk ? namepath : ImagePath, IsUseArk);
		if (p_AtlatTextureMenuItem == NULL)
		{
			p_AtlatTextureMenuItem = new Texture2D();
		}
		if (p_useMipMap && p_AtlatMenuItemTemp->getNumberOfMipmaps() > 1)
		{
			p_AtlatTextureMenuItem->initWithData(p_AtlatMenuItemTemp->getMipmaps()[1].address, p_AtlatMenuItemTemp->getMipmaps()[1].len,
				p_AtlatMenuItemTemp->getRenderFormat(), p_AtlatMenuItemTemp->getWidth() / 2, p_AtlatMenuItemTemp->getHeight() / 2,
				Size(p_AtlatMenuItemTemp->getWidth() / 2, p_AtlatMenuItemTemp->getHeight() / 2)
				);
		}
		else
		{
			if (p_AtlatMenuItemTemp->getNumberOfMipmaps() <= 1)
			{
				p_AtlatTextureMenuItem->initWithImage(p_AtlatMenuItemTemp);
			}
			else
			{
				p_AtlatTextureMenuItem->initWithData(p_AtlatMenuItemTemp->getMipmaps()[0].address, p_AtlatMenuItemTemp->getMipmaps()[0].len,
					p_AtlatMenuItemTemp->getRenderFormat(), p_AtlatMenuItemTemp->getWidth(), p_AtlatMenuItemTemp->getHeight(),
					Size(p_AtlatMenuItemTemp->getWidth(), p_AtlatMenuItemTemp->getHeight())
					);
			}
		}
#ifdef USE_ANTIALIAS
		if(loadAA)
		{
			p_AtlatTextureMenuItem->generateMipmap();
			Texture2D::TexParams texParams = { GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE };
			p_AtlatTextureMenuItem->setTexParameters(&texParams);
		}
#endif
		CC_SAFE_RELEASE_NULL(p_AtlatMenuItemTemp);
		dData = NULL;
#else
		p_AtlatMenuItem = new Image();
		p_AtlatMenuItem->initWithImageFile(ImagePath, IsUseArk);
#endif
	}
	//
	//load streak
	//
#ifdef USE_STREAK_TEX
	if (!P_Streak)
		P_Streak = new Texture2D();
	Image * p_AtlatSpriteStreak = new Image();
	p_AtlatSpriteStreak->initWithImageFile(IsUseArk ? "Texture/GamePlay/streak.png" : GenerateMipName("Texture/GamePlay/streak.png"), IsUseArk);
	P_Streak->initWithImage(p_AtlatSpriteStreak);
	CC_SAFE_RELEASE(p_AtlatSpriteStreak);
#endif
	//
	//load font
	//
	p_list_string_atlats.clear();
	size_t numberfont = sizeof(GamefontList) / sizeof(std::string);
	for (size_t i = 0; i < numberfont; i++)
	{
		auto string = Label::createWithTTF("", ResolveNamePath(GamefontList[i]), 40);
		p_list_string_atlats.pushBack(string);
	}
}

///////////////////////////////////////////////////////////////////////////////
void ObjectTexture::ParseTextureToMemory()
{
	auto tx = LoadTextureFromSprite(TYPE_TEXTURE::OBJECT_TEXTURE, "orbit");tx->retain();
	p_ListSpriteIG.insert(std::pair<std::string, Texture2D*>("orbit", tx));
	tx = LoadTextureFromSprite(TYPE_TEXTURE::OBJECT_TEXTURE, "bubble"); tx->retain();
	p_ListSpriteIG.insert(std::pair<std::string, Texture2D*>("bubble", tx));
	tx = LoadTextureFromSprite(TYPE_TEXTURE::OBJECT_TEXTURE, "orbit_snow"); tx->retain();
	p_ListSpriteIG.insert(std::pair<std::string, Texture2D*>("orbit_snow", tx));
	tx = LoadTextureFromSprite(TYPE_TEXTURE::OBJECT_TEXTURE, "bomb"); tx->retain();
	p_ListSpriteIG.insert(std::pair<std::string, Texture2D*>("bomb", tx));
	tx = LoadTextureFromSprite(TYPE_TEXTURE::OBJECT_TEXTURE, "time_bomb"); tx->retain();
	p_ListSpriteIG.insert(std::pair<std::string, Texture2D*>("time_bomb", tx));
	//
	for (int i = 0; i < XMLMgr->GetOrbitData()->numberDeco; i++)
	{
		std::string str = "deco" + std::to_string(i);
		tx = LoadTextureFromSprite(TYPE_TEXTURE::OBJECT_TEXTURE, str);	tx->retain();

		p_ListSpriteIG.insert(std::pair<std::string, Texture2D*>(str, tx));
	}
	for (int i = 0; i < XMLMgr->GetOrbitData()->numberInter; i++)
	{
		std::string str = "inter" + std::to_string(i);
		tx = LoadTextureFromSprite(TYPE_TEXTURE::OBJECT_TEXTURE, str);	tx->retain();
		p_ListSpriteIG.insert(std::pair<std::string, Texture2D*>(str, tx));
	}
	//
	auto p_list = XMLMgr->GetDataObject(TYPE_TEXTURE::OBJECT_CHAR)->dDataSize;
	for (auto it = p_list.begin(); it != p_list.end(); ++it)
	{
		std::string str = it->first;
		tx = LoadTextureFromSprite(TYPE_TEXTURE::OBJECT_CHAR, str); tx->retain();
		p_ListSpriteIG.insert(std::pair<std::string, Texture2D*>(str, tx));	
	}
	//
	p_list = XMLMgr->GetDataObject(TYPE_TEXTURE::OBJECT_EFFECT)->dDataSize;
	for (auto it = p_list.begin(); it != p_list.end(); ++it)
	{
		std::string str = it->first;
		tx = LoadTextureFromSprite(TYPE_TEXTURE::OBJECT_EFFECT, str); tx->retain();
		p_ListSpriteIG.insert(std::pair<std::string, Texture2D*>(str, tx));
	}
	//
	CC_SAFE_RELEASE_NULL(p_AtlatSpriteObject);
	CC_SAFE_RELEASE_NULL(p_AtlatMainChar);
	//
	p_list = XMLMgr->GetDataObject(TYPE_TEXTURE::MENU_TEXRURE)->dDataSize;
	for (auto it = p_list.begin(); it != p_list.end(); ++it)
	{
		std::string str = it->first;
		tx = LoadTextureFromSprite(TYPE_TEXTURE::MENU_TEXRURE, str);	tx->retain();
		p_ListSpriteIG.insert(std::pair<std::string, Texture2D*>(str, tx));
	}
	CC_SAFE_RELEASE_NULL(p_AtlatButtonItem);
	//
	p_list = XMLMgr->GetDataObject(TYPE_TEXTURE::MENU_LAYOUT)->dDataSize;
	for (auto it = p_list.begin(); it != p_list.end(); ++it)
	{
		std::string str = it->first;
		tx = LoadTextureFromSprite(TYPE_TEXTURE::MENU_LAYOUT, str);	tx->retain();
		p_ListSpriteIG.insert(std::pair<std::string, Texture2D*>(str, tx));
	}
	//
	CC_SAFE_RELEASE_NULL(p_AtlatMenuItem);
	//
	p_ListSpriteIG.insert(std::pair<std::string, Texture2D*>("streak", P_Streak));
}

///////////////////////////////////////////////////////////////////////////////
void ObjectTexture::ParseSpriteToMemory()
{
#ifdef USE_SPRITE_TEXTURE_CACHE
	p_ListIdxNameSprite.insert(std::pair<std::string, TYPE_TEXTURE>("orbit", TYPE_TEXTURE::OBJECT_TEXTURE));
	p_ListIdxNameSprite.insert(std::pair<std::string, TYPE_TEXTURE>("bubble", TYPE_TEXTURE::OBJECT_TEXTURE));
	p_ListIdxNameSprite.insert(std::pair<std::string, TYPE_TEXTURE>("orbit_snow", TYPE_TEXTURE::OBJECT_TEXTURE));
	p_ListIdxNameSprite.insert(std::pair<std::string, TYPE_TEXTURE>("bomb", TYPE_TEXTURE::OBJECT_TEXTURE));
	p_ListIdxNameSprite.insert(std::pair<std::string, TYPE_TEXTURE>("time_bomb", TYPE_TEXTURE::OBJECT_TEXTURE));

	for (int i = 0; i < XMLMgr->GetOrbitData()->numberDeco; i++)
	{
		std::string str = "deco" + std::to_string(i);
		p_ListIdxNameSprite.insert(std::pair<std::string, TYPE_TEXTURE>(str, TYPE_TEXTURE::OBJECT_TEXTURE));
	}
	for (int i = 0; i < XMLMgr->GetOrbitData()->numberInter; i++)
	{
		std::string str = "inter" + std::to_string(i);
		p_ListIdxNameSprite.insert(std::pair<std::string, TYPE_TEXTURE>(str, TYPE_TEXTURE::OBJECT_TEXTURE));
	}

	auto p_list = XMLMgr->GetDataObject(TYPE_TEXTURE::OBJECT_CHAR)->dDataSize;
	for (auto it = p_list.begin(); it != p_list.end(); ++it)
	{
		std::string str = it->first;
		p_ListIdxNameSprite.insert(std::pair<std::string, TYPE_TEXTURE>(str, TYPE_TEXTURE::OBJECT_CHAR));
	}

	p_list = XMLMgr->GetDataObject(TYPE_TEXTURE::OBJECT_EFFECT)->dDataSize;
	for (auto it = p_list.begin(); it != p_list.end(); ++it)
	{
		std::string str = it->first;
		p_ListIdxNameSprite.insert(std::pair<std::string, TYPE_TEXTURE>(str, TYPE_TEXTURE::OBJECT_EFFECT));
	}

	p_list = XMLMgr->GetDataObject(TYPE_TEXTURE::MENU_TEXRURE)->dDataSize;
	for (auto it = p_list.begin(); it != p_list.end(); ++it)
	{
		std::string str = it->first;
		p_ListIdxNameSprite.insert(std::pair<std::string, TYPE_TEXTURE>(str, TYPE_TEXTURE::MENU_TEXRURE));
	}

	//
	p_list = XMLMgr->GetDataObject(TYPE_TEXTURE::MENU_LAYOUT)->dDataSize;
	for (auto it = p_list.begin(); it != p_list.end(); ++it)
	{
		std::string str = it->first;
		p_ListIdxNameSprite.insert(std::pair<std::string, TYPE_TEXTURE>(str, TYPE_TEXTURE::MENU_LAYOUT));
	}

#else
	auto tx0 = LoadSpriteFromTexture(TYPE_TEXTURE::OBJECT_TEXTURE, "orbit"); tx0->retain();
	p_ListSpriteMemory.insert("orbit", tx0); CC_SAFE_RELEASE_NULL(tx0);
	auto tx1 = LoadSpriteFromTexture(TYPE_TEXTURE::OBJECT_TEXTURE, "bubble"); tx1->retain();
	p_ListSpriteMemory.insert("bubble", tx1); CC_SAFE_RELEASE_NULL(tx1);
	auto tx2 = LoadSpriteFromTexture(TYPE_TEXTURE::OBJECT_TEXTURE, "orbit_snow"); tx2->retain();
	p_ListSpriteMemory.insert("orbit_snow", tx2); CC_SAFE_RELEASE_NULL(tx2);
	auto tx3 = LoadSpriteFromTexture(TYPE_TEXTURE::OBJECT_TEXTURE, "bomb"); tx3->retain();
	p_ListSpriteMemory.insert("bomb", tx3); CC_SAFE_RELEASE_NULL(tx3);
	auto tx4 = LoadSpriteFromTexture(TYPE_TEXTURE::OBJECT_TEXTURE, "time_bomb"); tx4->retain();
	p_ListSpriteMemory.insert("time_bomb", tx4); CC_SAFE_RELEASE_NULL(tx4);

	for (int i = 0; i < XMLMgr->GetOrbitData()->numberDeco; i++)
	{
		std::string str = "deco" + std::to_string(i);
		auto tx = LoadSpriteFromTexture(TYPE_TEXTURE::OBJECT_TEXTURE, str); tx->retain();
		p_ListSpriteMemory.insert(str, tx); CC_SAFE_RELEASE_NULL(tx);
	}
	for (int i = 0; i < XMLMgr->GetOrbitData()->numberInter; i++)
	{
		std::string str = "inter" + std::to_string(i);
		auto tx = LoadSpriteFromTexture(TYPE_TEXTURE::OBJECT_TEXTURE, str);	tx->retain();
		p_ListSpriteMemory.insert(str, tx); CC_SAFE_RELEASE_NULL(tx);
	}

	auto p_list = XMLMgr->GetDataObject(TYPE_TEXTURE::OBJECT_CHAR)->dDataSize;
	for (auto it = p_list.begin(); it != p_list.end(); ++it)
	{
		std::string str = it->first;
		auto tx = LoadSpriteFromTexture(TYPE_TEXTURE::OBJECT_CHAR, str);	tx->retain();
		p_ListSpriteMemory.insert(str, tx); CC_SAFE_RELEASE_NULL(tx);
	}

	p_list = XMLMgr->GetDataObject(TYPE_TEXTURE::OBJECT_EFFECT)->dDataSize;
	for (auto it = p_list.begin(); it != p_list.end(); ++it)
	{
		std::string str = it->first;
		auto tx = LoadSpriteFromTexture(TYPE_TEXTURE::OBJECT_EFFECT, str);	tx->retain();
		p_ListSpriteMemory.insert(str, tx);  CC_SAFE_RELEASE_NULL(tx);
	}

//	CC_SAFE_RELEASE_NULL(p_AltastTextureSpriteObject);
//	CC_SAFE_RELEASE_NULL(p_AtlatMainChar);
	//
	p_list = XMLMgr->GetDataObject(TYPE_TEXTURE::MENU_TEXRURE)->dDataSize;
	for (auto it = p_list.begin(); it != p_list.end(); ++it)
	{
		std::string str = it->first;
		auto tx = LoadSpriteFromTexture(TYPE_TEXTURE::MENU_TEXRURE, str);	tx->retain();
		p_ListSpriteMemory.insert(str, tx);  CC_SAFE_RELEASE_NULL(tx);
	}
//	CC_SAFE_RELEASE_NULL(p_AtlatTextureButtonItem);
	//
	p_list = XMLMgr->GetDataObject(TYPE_TEXTURE::MENU_LAYOUT)->dDataSize;
	for (auto it = p_list.begin(); it != p_list.end(); ++it)
	{
		std::string str = it->first;
		auto tx = LoadSpriteFromTexture(TYPE_TEXTURE::MENU_LAYOUT, str); tx->retain();
		p_ListSpriteMemory.insert(str, tx);   CC_SAFE_RELEASE_NULL(tx);
	}
	//
//	CC_SAFE_RELEASE_NULL(p_AtlatTextureMenuItem);
	//
#ifdef USE_STREAK_TEX
	tx = Sprite::createWithTexture(P_Streak); tx->retain();
	p_ListSpriteMemory.insert(std::pair<std::string, Sprite*>("streak", tx));
#endif // USE_STREAK_TEX
#endif // USE_SPRITE_TEXTURE_CACHE
	
	//parse font texture
	ParseSpriteFontToMemory();
	//
}

///////////////////////////////////////////////////////////////////////////////
void  ObjectTexture::ParseSpriteFontToMemory()
{
#ifdef USE_SPRITE_TEXTURE_CACHE
	p_ListIdxFontSprite
	for (int i = 0; i < 10; i++)
	{
			std::string name = std::to_string(i) + std::string("_6");
			p_ListIdxFontSprite.insert(std::pair<std::string, TYPE_TEXTURE>(name, TYPE_TEXTURE::FONT_TEXTURE));
	}
	for (int i = 0; i < 10; i++)
	{
		std::string name = std::to_string(i) + std::string("_3");
		p_ListIdxFontSprite.insert(std::pair<std::string, TYPE_TEXTURE>(name, TYPE_TEXTURE::FONT_TEXTURE));
	}
	for (int i = 0; i < 10; i++)
	{
		std::string name = std::to_string(i) + std::string("_0");
		p_ListIdxFontSprite.insert(std::pair<std::string, TYPE_TEXTURE>(name, TYPE_TEXTURE::FONT_TEXTURE));
	}

#else
	for (int i = 0; i < 10; i++)
	{
		std::string name = std::to_string(i) + std::string("_6");
		auto tx0 = LoadSpriteFromTexture(TYPE_TEXTURE::FONT_TEXTURE, name); tx0->retain();
		p_ListCharacterFont.insert(name, tx0);
	}
	for (int i = 0; i < 10; i++)
	{
		std::string name = std::to_string(i) + std::string("_3");
		auto tx0 = LoadSpriteFromTexture(TYPE_TEXTURE::FONT_TEXTURE, name); tx0->retain();
		p_ListCharacterFont.insert(name, tx0);
	}
	for (int i = 0; i < 10; i++)
	{
		std::string name = std::to_string(i) + std::string("_0");
		auto tx0 = LoadSpriteFromTexture(TYPE_TEXTURE::FONT_TEXTURE, name); tx0->retain();
		p_ListCharacterFont.insert(name, tx0);
	}
#endif
}
///////////////////////////////////////////////////////////////////////////////
Sprite * ObjectTexture::GetSpriteFromTextureCache(std::string NameTexture)
{
	TYPE_TEXTURE tp = p_ListIdxNameSprite.find(NameTexture)->second;
	auto tx = LoadSpriteFromTexture(tp, NameTexture);
	tx->retain();
	return tx;
}


Sprite * ObjectTexture::GetFontNumberFromCache(std::string NameTexture)
{
	auto tx = LoadSpriteFromTexture(TYPE_TEXTURE::FONT_TEXTURE, NameTexture);
	tx->retain();
	return tx;
}
///////////////////////////////////////////////////////////////////////////////
unsigned char *  ObjectTexture::GetDataSpecific(Rect bound, Image * im)
{
	int sizedata = int(bound.getMaxX() - bound.getMinX())*int(bound.getMaxY() - bound.getMinY()) * 4;
	unsigned char * data_return = new unsigned char[sizedata];
	unsigned char * dataImg = NULL;
	int texWidth = im->getWidth();
#ifdef OS_IOS
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-variable"
#endif
	auto formatIm = im->getFileType();
#ifdef OS_IOS
#pragma clang diagnostic pop
#endif
	//
	if (im->getBitPerPixel() < 32)
	{
		CCASSERT(false, "not support texture format");
	}
	//
	if (p_useMipMap && im->getNumberOfMipmaps() > 0)
	{
		dataImg = im->getMipmaps()[1].address;
		texWidth = im->getWidth() / 2;		
	}
	else
	{
		dataImg = im->getData();
	}
	//
	int idxBegin = ((int)bound.getMinX() + (texWidth * bound.getMinY())) * 4; //Red - Green - Blue - Alpha
	int idxEnd =   ((int)bound.getMaxX() + (texWidth * bound.getMaxY())) * 4; //Red - Green - Blue - Alpha
	int currentIdxHeight = bound.getMinY();
	int currentIdx = 0;
	for (int i = idxBegin; i< idxEnd;)
	{
		memcpy(data_return + currentIdx, dataImg + i, sizeof(char)); i += 1; currentIdx += 1;// red
		memcpy(data_return + currentIdx, dataImg + i, sizeof(char)); i += 1; currentIdx += 1; // green
		memcpy(data_return + currentIdx, dataImg + i, sizeof(char)); i += 1; currentIdx += 1; // blue
		memcpy(data_return + currentIdx, dataImg + i, sizeof(char)); i += 1; currentIdx += 1;// alpha
		if (i >= int(bound.getMaxX() + (texWidth * (currentIdxHeight))) * 4)
		{
			currentIdxHeight++;
			i = ((int)bound.getMinX() + (texWidth * (currentIdxHeight))) * 4;
		}
		if (currentIdx >= sizedata)
		{
			break;
		}
	}
	//
	return data_return;
}

///////////////////////////////////////////////////////////////////////////////
Label * ObjectTexture::GetFontData(int idx)
{
	CCASSERT(idx < p_list_string_atlats.size(), "index bigger than size of string list");
	return p_list_string_atlats.at(idx);
}