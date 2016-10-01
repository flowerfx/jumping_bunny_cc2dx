#include "ObjectBGEntity.h"
#include "Control/InputManager.h"

BGEntity::BGEntity()
{
	p_dirMove = Point(0, 0);
	p_IsVisible = true;
	p_SpeedMove = 0;
	p_PointToMoveTo = Point(0, 0);
	p_PointAlive = 450;
//	p_Color = { 255, 255, 255, 255 };
	p_IsFree = false;
}
BGEntity::~BGEntity()
{
	CC_SAFE_RELEASE_NULL(p_Resource);
	p_Resource = NULL;
}

void BGEntity::SetScaleBG(Point s)
{
	CCASSERT(p_Resource, "Resource invaid");
	Point p_scale(1, 1);
	if (TextureMgr->IsUseMipMap())
	{
		p_scale.x = s.x * 2;
		p_scale.y = s.y * 2;
	}
	else
	{
		p_scale.x = s.x;
		p_scale.y = s.y;
	}
	p_Resource->setScaleX(p_scale.x);
	p_Resource->setScaleY(p_scale.y);
}

Point BGEntity::GetScaleBG()
{
	CCASSERT(p_Resource, "Resource invaid");
	Point p_Size(1, 1);
	if (TextureMgr->IsUseMipMap())
	{
		p_Size.x = p_Resource->getScaleX() / 2.f;
		p_Size.y = p_Resource->getScaleY() / 2.f;
	}
	else
	{
		p_Size.x = p_Resource->getScaleX();
		p_Size.y = p_Resource->getScaleY();
	}
	return p_Size;
}

bool BGEntity::VisibleBG()
{
	CCASSERT(p_Resource, "Resource invaid");
	return p_IsVisible;
}

void BGEntity::VisibleBG(bool b)
{
	CCASSERT(p_Resource, "Resource invaid");
	p_IsVisible = b;
}

Point BGEntity::GetSizeBG()
{
	CCASSERT(p_Resource, "Resource invaid");
	Point p_Size(1, 1);
	p_Size.x = p_Resource->getContentSize().width * p_Resource->getScaleX();
	p_Size.y = p_Resource->getContentSize().height * p_Resource->getScaleY();
	return p_Size;
}

void BGEntity::SetSizeBG(Point s)
{
	CCASSERT(p_Resource, "Resource invaid");
	Point p_scale(1, 1);
	p_scale.x = s.x / p_Resource->getContentSize().width;
	p_scale.y = s.y / p_Resource->getContentSize().height;
	p_Resource->setScaleX(p_scale.x);
	p_Resource->setScaleY(p_scale.y);
}

void BGEntity::SetPositionBG(Point p)
{
	CCASSERT(p_Resource, "Resource invaid");
	p_Resource->setPosition(p);
}

Point BGEntity::GetPositionBG()
{
	CCASSERT(p_Resource, "Resource invaid");
	return p_Resource->getPosition();
}

void BGEntity::onVisitBG(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch)
{
	CCASSERT(p_Resource, "Resource invaid");
	if (p_Resource == NULL)
		return;
	if (p_IsVisible)
	{
		if (spriteBatch == NULL)
		{
			p_Resource->visit(renderer, transform, flags);
		}
		else
		{
			spriteBatch->addChild(p_Resource);
		}
	}
}

void BGEntity::onDrawBG(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch)
{
	//CCASSERT(p_Resource, "Resource invaid");
	if (p_Resource == NULL)
		return;
	if (p_IsVisible)
	{
		if (spriteBatch == NULL)
		{
			p_Resource->draw(renderer, transform, flags);
		}
		else
		{
			spriteBatch->addChild(p_Resource);
		}
	}
}

void BGEntity::LoadBGFromSingleImage(std::string name)
{
		p_Resource = Sprite::create(name);
		p_Resource->retain();
}

void BGEntity::LoadBGFromSingleTexture(Texture2D * tx)
{
		p_Resource = Sprite::createWithTexture(tx);
		if (p_Resource)
		{
			p_Resource->retain();
		}
}
void BGEntity::parseSprite(Sprite * p)
{
#ifdef USE_SPRITE_TEXTURE_CACHE
	p_Resource = p;
#else
	p_Resource = p->createWithTexture(p->getTexture(),p->getTextureRect());
#endif
	p_Resource->retain();
}
void BGEntity::LoadBGFromData(TYPE_TEXTURE t, std::string str)
{
		p_Resource = ObjectTexture::GetInstance()->LoadSpriteFromTexture(t, str);
		if (p_Resource)
		{
			p_Resource->retain();
		}

}

void BGEntity::onUpdateBG(float dt)
{
	Point cur_pos = GetPositionBG();
	cur_pos = cur_pos + p_VectorMove * SyncWithFPS(dt * 100.f * p_SpeedMove);
	SetPositionBG(cur_pos);
}

void BGEntity::MoveToPoint(Point p)
{
	float distance = sqrt((float)((p.x - GetPositionBG().x) *(p.x - GetPositionBG().x) + (p.y - GetPositionBG().y)*(p.y - GetPositionBG().y)));
	p_PointToMoveTo = p;
	//get direct move
	p_VectorMove = p_PointToMoveTo - GetPositionBG();
	//normalize
	p_VectorMove = p_VectorMove / distance;
}
bool BGEntity::IsAlive()
{
	Point p = GetPositionBG();
	if (p_dirMove == Point(-1, 0))
	{
		if (p.x < -(p_PointAlive*GetScaleFactor() + (GetSizeBG().x / 2)))
		{
			return false;
		}
	}
	else if (p_dirMove == Point(1, 0))
	{
		if (p.x > (GetWinSize().width + p_PointAlive*GetScaleFactor() +( GetSizeBG().x / 2)))
		{
			return false;
		}
	}
	return true;
}