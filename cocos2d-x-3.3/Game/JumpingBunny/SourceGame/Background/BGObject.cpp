#include "BGObject.h"
//////////////////////////////////////////////////////////////////
//
// background layer
//
//////////////////////////////////////////////////////////////////
BGLayer::BGLayer()
{
}
BGLayer::~BGLayer()
{
	p_objectLayer.clear();
}

void BGLayer::InitTheValue(std::string nameLayer, std::string nameType)
{
	auto val = XMLMgr->GetGameBG()->GetLayerbackground(nameLayer)->GetLayerData(nameType);
	int min_number = val->p_numberObject[0];
	int max_number = val->p_numberObject[1];
	int number = GetRandom(max_number, min_number);
	for (int i = 0; i < number; i++)
	{
		p_objectLayer.pushBack(CreateObject(nameLayer, nameType));
	}
	p_nameBG = nameLayer;
	p_nametype = nameType;
}

void BGLayer::onDraw(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch)
{
	for (int i = 0; i < p_objectLayer.size(); i++)
	{
		//if (p_objectLayer.at(i)->IDTag)
		{
			p_objectLayer.at(i)->onDrawBG(renderer, transform, flags, spriteBatch);
		}
	}
}
void BGLayer::onVisit(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch)
{
	for (int i = 0; i < p_objectLayer.size(); i++)
	{
		//if (p_objectLayer.at(i)->IDTag)
		{
			p_objectLayer.at(i)->onVisitBG(renderer, transform, flags, spriteBatch);
		}
	}
}

void BGLayer::onUpdate(float dt)
{
	for (int i = 0; i < p_objectLayer.size(); i++)
	{
		if (p_objectLayer.at(i)->IsFree())
			continue;
		p_objectLayer.at(i)->onUpdateBG(dt);
		if (!p_objectLayer.at(0)->IsAlive())
		{
			p_objectLayer.at(0)->IsFree(true);

			auto val = XMLMgr->GetGameBG()->GetLayerbackground(p_nameBG)->GetLayerData(p_nametype);
			int min = (int)val->p_chanceAppear.x;
			int max = (int)val->p_chanceAppear.y;
			bool isAppear = (GetRandom(min, max) == min);
			if (isAppear)
			{
				p_objectLayer.pushBack(CreateObject(p_nameBG, p_nametype));
			}
			break;
		}
	}
}

void BGLayer::onRemoveBG()
{
	if (p_objectLayer.size() <= 0)
		return;
	if (p_objectLayer.at(0)->IsFree())
	{
		CC_SAFE_RELEASE(p_objectLayer.at(0));
		p_objectLayer.erase(0);
	}
}

BGEntity* BGLayer::CreateObject(std::string nameLayer, std::string nameType, BGEntity* previousC)
{
	auto entity = new BGEntity();
	//
	Vector<BGEntity*> listTemp = p_objectLayer;
	auto pdData = XMLMgr->GetGameBG()->GetLayerbackground(nameLayer)->GetLayerData(nameType);

	int numberIdxSource = pdData->p_numberIdxSource;
	int idx = GetRandom(numberIdxSource, 0);
	int typeObject = GetRandom((int)pdData->p_listSource.size(), 0);
	CCASSERT((size_t)typeObject < pdData->p_listSource.size(), "the value must be smaller than size of listsource");
	std::string sourceName = pdData->p_listSource.at(typeObject);
	//
	if (numberIdxSource > 1)
	{
		sourceName = sourceName + std::to_string(idx);
	}
	//
#if USE_SPRITE_STORE
	auto te = TextureMgr->GetSpriteFromMemory(sourceName);
	entity->parseSprite(te);
#else
	auto te = TextureMgr->GetTexureFromMemory(sourceName);
	entity->LoadBGFromSingleTexture(te);
#endif
	//
	Point ScaleRatio = pdData->p_ListScale.at(typeObject);
	float p_scaleRatio = (float)GetRandom((int)(ScaleRatio.x * 100), (int)(ScaleRatio.y * 100)) / 100.f;
	p_scaleRatio = p_scaleRatio * GetScaleFactor();
	entity->SetScaleBG(Point(p_scaleRatio, p_scaleRatio));
	//
	bool IsFullScreen = (pdData->p_IsFullScreen == 1);
	if (IsFullScreen)
	{
		entity->SetSizeBG(Point(GetWinSize().width, GetWinSize().height));
	}
	//
	int speedMove = (GetRandom((int)(pdData->p_SpeedMove[1] * 100), (int)(pdData->p_SpeedMove[0] * 100)));
	float fspeedMove = (float)speedMove / 100.f;
	entity->SpeedMove(fspeedMove);
	entity->DirMove(pdData->p_DirMove);
	entity->PointAlive(pdData->p_DistanceAlive);

	Point pointToMoveto(0, 0);
	//
	Point ratioPosY = pdData->p_ListRatioPosY.at(typeObject);
	Point ratioPosX = pdData->p_ListRatioPosX.at(typeObject);
	//
	if (listTemp.size() > 1)
	{
		if (previousC == NULL)
		{
			CalculateThePosObject(listTemp.at(listTemp.size() - 1), entity, ratioPosY, ratioPosX, nameLayer, nameType);
		}
		else
		{
			CalculateThePosObject(previousC, entity, ratioPosY, ratioPosX, nameLayer, nameType);
		}
	}
	else
	{
		CalculateThePosObject(NULL, entity, ratioPosY, ratioPosX, nameLayer, nameType);
	}

	if (pdData->p_DirMove == Point(-1, 0))
	{
		pointToMoveto = Point(-((entity->PointAlive() * 2 * GetScaleFactor()) + entity->GetSizeBG().x), entity->GetPositionBG().y);
	}
	else
	{
		pointToMoveto = Point(((entity->PointAlive() * 2 * GetScaleFactor()) + entity->GetSizeBG().x + GetWinSize().width), entity->GetPositionBG().y);
	}
	entity->MoveToPoint(pointToMoveto);

	return entity;
}

void BGLayer::CalculateThePosObject(BGEntity * previousC, BGEntity* t, Point ratioPosY, Point ratioPosX, std::string name, std::string type)
{
	Size winSize = GetWinSize();

	int deltaY = GetRandom((int)(ratioPosY.y * 100), (int)(ratioPosY.x * 100));
#if USE_DX_COR
	int rY = GetWinSize().height * (100.f - (float)deltaY) / 100.f;
#else
	int rY = GetWinSize().height * (float)deltaY / 100.f;
#endif
	int rX = winSize.width / 2.f;//(rand() % maxWReach) + minWReach;

	if (previousC)
	{
		float dis = XMLMgr->GetGameBG()->GetLayerbackground(name)->GetLayerData(type)->p_Distance;
		if (t->DirMove() == Point(-1, 0))
		{
			rX = previousC->GetPositionBG().x + (previousC->GetSizeBG().x / 2) + dis + (t->GetSizeBG().x / 2);
			if (rX < GetWinSize().width && rX > 0)
			{
				rX = GetWinSize().width + (dis + (t->GetSizeBG().x));
			}
		}
		else if (t->DirMove() == Point(1, 0))
		{
			rX = previousC->GetPositionBG().x - ((previousC->GetSizeBG().x / 2) + dis + (t->GetSizeBG().x / 2));
			if (rX < GetWinSize().width && rX > 0)
			{
				rX = -((previousC->GetSizeBG().x / 2) + dis + (t->GetSizeBG().x));
			}
		}
	}
	else
	{
		rX = GetRandom((int)(ratioPosX.x * winSize.width), (int)(ratioPosX.y * winSize.width));
	}

	t->SetPositionBG(Point(rX, rY));
}

//////////////////////////////////////////////////////////////////
//
// background object
//
//////////////////////////////////////////////////////////////////

BGObject::BGObject()
{
	p_nameBG = "";
}
BGObject::~BGObject()
{
	for (int i = 0; i < p_ListBG.size(); i++)
	{
		p_ListBG.at(i)->release();
	}
	p_ListBG.clear();
}

void BGObject::InitTheValue(std::string name)
{
	//init the p_nameBG
	p_nameBG = name;
	auto val = XMLMgr->GetGameBG()->GetLayerbackground(p_nameBG);
	int numberLayer = (int)val->p_listNamelayerRender.size();
	for (int i = 0; i < numberLayer; i++)
	{
		auto layer = new BGLayer();
		layer->InitTheValue(p_nameBG, val->p_listNamelayerRender[i]);
		//layer->retain();
		p_ListBG.pushBack(layer);
		//layer->autorelease();
		CC_SAFE_RELEASE_NULL(layer);
	}
}

void BGObject::onDraw(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch)
{
	for (int i = 0; i < p_ListBG.size(); i++)
	{
		p_ListBG.at(i)->onDraw(renderer, transform, flags, spriteBatch);
	}	
}
void BGObject::onVisit(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch)
{
	for (int i = 0; i < p_ListBG.size(); i++)
	{
		p_ListBG.at(i)->onVisit(renderer, transform, flags, spriteBatch);
	}
}
void BGObject::onUpdate(float dt)
{
	for (int i = 0; i < p_ListBG.size(); i++)
	{
		p_ListBG.at(i)->onUpdate(dt);
	}
}

void BGObject::onRemoveBG()
{
	for (int i = 0; i < p_ListBG.size(); i++)
	{
		p_ListBG.at(i)->onRemoveBG();
	}
}