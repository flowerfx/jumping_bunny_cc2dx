#include "EntityObject.h"
#include "math.h"
#include "Utility/ObjectTexture.h"
#include "Utility/LoadDataXML.h"
EntityObject::EntityObject()
{
	p_numberSprite = 0;
	p_SpeedAroundRotate = 0;
	p_SpeedSeftRotate	= 1;
	p_SpeedMove = 1;
#if ENABLE_EDITOR
	InitParam();
#endif
	p_IsRotate = false;
	p_IsStopMove = true;
	p_IsUseAnimatEditor = false;
    p_Animate = NULL;
    p_Object = NULL;
    p_Armature = NULL;

	p_currentTint = new BYTE[3];

	p_VectorDirectMove = Point(0, 0);
}


EntityObject::~EntityObject()
{
	FreeObject();
	p_VectorDirectMove = Point(0, 0);
}

Node *  EntityObject::GetObject()
{
	if (p_IsUseAnimatEditor)
		return p_Armature;
	else
		return p_Object;
}
/////////////////////////////////////////////////////
//--------------------------------------------------
/////////////////////////////////////////////////////
void EntityObject::SetSizeObject(Size s, bool notMip)
{
		CCASSERT(GetObject(), "object sprite invaid");
		Size p_scale(1, 1);
		p_scale.width = s.width / GetObject()->getContentSize().width;
		p_scale.height = s.height / GetObject()->getContentSize().height;
		if (TextureMgr->IsUseMipMap() && notMip == false)
		{
			p_scale.width = p_scale.width * 2;
			p_scale.height = p_scale.height * 2;
		}
		GetObject()->setScaleX(p_scale.width);
		GetObject()->setScaleY(p_scale.height);
}
Size EntityObject::GetSizeObject(bool notMip)
{
	CCASSERT(GetObject(), "object sprite invaid");
	Size p_Size(1, 1);
	p_Size.width = GetObject()->getContentSize().width * abs(GetObject()->getScaleX());
	p_Size.height = GetObject()->getContentSize().height * abs(GetObject()->getScaleY());
	if (TextureMgr->IsUseMipMap() && notMip == false)
	{
		p_Size.width = p_Size.width  ;
		p_Size.height = p_Size.height ;
	}
	return p_Size;
}
/////////////////////////////////////////////////////
//--------------------------------------------------
/////////////////////////////////////////////////////
void EntityObject::SetPosObject(Point p)
{
	CCASSERT(GetObject(), "object sprite invaid");
	GetObject()->setPosition(p);
}
Point EntityObject::GetPosObject()
{
	CCASSERT(GetObject(), "object sprite invaid");
	return GetObject()->getPosition();
}
/////////////////////////////////////////////////////
//--------------------------------------------------
/////////////////////////////////////////////////////
void EntityObject::SetRectObject(Rect r)
{
	if (p_IsUseAnimatEditor)
		return;
	CCASSERT(p_Object, "object sprite invaid");
	p_Object->setTextureRect(r);
}
Rect EntityObject::GetRectObject()
{
	if (p_IsUseAnimatEditor)
		return Rect(0.f,0.f,0.f,0.f);

	CCASSERT(p_Object, "object sprite invaid");
	return p_Object->getTextureRect();
}
/////////////////////////////////////////////////////
//--------------------------------------------------
/////////////////////////////////////////////////////
void EntityObject::SetRotateObject(float r)
{
	CCASSERT(GetObject(), "object sprite invaid");
	GetObject()->setRotation(r);
}
float EntityObject::GetRotateObject()
{
	CCASSERT(GetObject(), "object sprite invaid");
	return GetObject()->getRotation();
}

void EntityObject::SetRotateAroundAPoint(Point point, float r, float distance)
{
	CCASSERT(GetObject(), "object sprite invaid");
	r = (r)* (M_PI / 180); // Convert to radians
	float X = (float)(cos(r) * (GetPosObject().x - point.x) - sin(r) * (GetPosObject().y - point.y) + point.x);
	float Y = (float)(sin(r) * (GetPosObject().x - point.x) + cos(r) * (GetPosObject().y - point.y) + point.y);

	SetPosObject(Point(X, Y));
}
/////////////////////////////////////////////////////
//--------------------------------------------------
/////////////////////////////////////////////////////
int EntityObject::GetNumberObject()
{
	return p_numberSprite;
}
/////////////////////////////////////////////////////
//--------------------------------------------------
/////////////////////////////////////////////////////
void EntityObject::LoadObjectFromSingleImage(std::string name)
{
	p_Object = Sprite::create(name);
	p_numberSprite = 1;
	p_Object->retain();
}
/////////////////////////////////////////////////////
//--------------------------------------------------
/////////////////////////////////////////////////////
void EntityObject::LoadObjectFromSingleTexture(Texture2D * tx)
{
	p_Object = Sprite::createWithTexture(tx);
	p_numberSprite = 1;
	if (p_Object)
	{
		p_Object->retain();
	}
}
/////////////////////////////////////////////////////
//--------------------------------------------------
/////////////////////////////////////////////////////
void EntityObject::LoadObjectFromData(TYPE_TEXTURE t, std::string str)
{
	p_Object = ObjectTexture::GetInstance()->LoadSpriteFromTexture(t , str );
	if (p_Object)
	{
		p_Object->retain();
	}

}
/////////////////////////////////////////////////////
//--------------------------------------------------
/////////////////////////////////////////////////////
void EntityObject::LoadAnimateFromSpriteList(Vector<Sprite *> p_list)
{
	CCASSERT(p_Object != NULL, "object not init");

	Vector<SpriteFrame*> animFrames;

	for (size_t i = 0; i < p_list.size(); i++)
	{
		auto frame = SpriteFrame::createWithTexture(p_list.at(i)->getTexture(), p_list.at(i)->getTextureRect());
		animFrames.pushBack(frame);
	}

	p_Animate = CCAnimation::createWithSpriteFrames(animFrames, 0.3f, -1);
	p_Animate->retain();
	auto t = CCAnimate::create(p_Animate);
	t->setTag(ACTION_TAG::AC_ANIMATE);
	p_Object->runAction(t);
}
/////////////////////////////////////////////////////
//--------------------------------------------------
/////////////////////////////////////////////////////
void EntityObject::LoadObjectFromSprite(Sprite * p, bool UseAntiAlias)
{
#ifdef USE_SPRITE_TEXTURE_CACHE
	p_Object = p;
#else
	p_Object = p->createWithTexture(p->getTexture(), p->getTextureRect());
#endif
#if USE_ANTIALIAS
	if(UseAntiAlias)
	{
		p_Object->getTexture()->generateMipmap();
		Texture2D::TexParams texParams = { GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE };
		p_Object->getTexture()->setTexParameters(&texParams);
	}
#endif
	p_Object->retain();
}
/////////////////////////////////////////////////////
//--------------------------------------------------
/////////////////////////////////////////////////////
void EntityObject::onDrawObject(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch)
{
	if (spriteBatch == NULL)
	{
		GetObject()->draw(renderer, transform, flags);
	}
	else
	{
		spriteBatch->addChild(GetObject());	
	}
#if ENABLE_EDITOR
	if (p_IsTargetSelected)
	{
		DrawString(renderer, transform, flags);
	}
#endif
}
void EntityObject::onVisitObject(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch)
{
	if (spriteBatch == NULL)
	{
		GetObject()->visit(renderer, transform, flags);
	}
	else
	{
		spriteBatch->addChild(GetObject());
	}
}
/////////////////////////////////////////////////////
//--------------------------------------------------
/////////////////////////////////////////////////////
void EntityObject::MoveToPoint(Point p)
{
	float distance = sqrt((float)((p.x - GetPosObject().x) *(p.x - GetPosObject().x) + (p.y - GetPosObject().y)*(p.y - GetPosObject().y)));
	p_PointToMoveTo = p;
	//
	p_VectorDirectMove = p_PointToMoveTo - GetPosObject();
	p_VectorDirectMove = p_VectorDirectMove / distance;
	//
	p_DurationMove = distance / p_SpeedMove;
}
/////////////////////////////////////////////////////
//--------------------------------------------------
/////////////////////////////////////////////////////
void EntityObject::updateObject(float dt, bool UpdatePosition)
{
	if (UpdatePosition)
	{
		if (p_DurationMove > 0)
		{
			if (p_DurationMove < 1)
			{
				p_DurationMove = 0;
				p_IsStopMove = true;
				return;
			}
			Point cur_pos = GetPosObject();
			cur_pos = cur_pos + p_VectorDirectMove * SyncWithFPS(dt * 100.f * p_SpeedMove);
			//
			Point p = Point(p_PointToMoveTo.x - cur_pos.x, p_PointToMoveTo.y - cur_pos.y);
			float lengt = sqrt(p.x*p.x + p.y*p.y);
			p_DurationMove = lengt / p_SpeedMove;
			//
			SetPosObject(cur_pos);
			p_IsStopMove = false;
		}
		else
		{
			p_DurationMove = 0;
			p_IsStopMove = true;
		}
	}
#if ENABLE_EDITOR
	if (p_IsTargetSelected)
	{
		UpdateString(dt);
	}
	if (InputMgr->GetCurrentControlType() == CONTROL_TYPE::CANCEL_SELECT)
	{
		if (HaveObjectSelected() && p_IsTargetSelected)
		{
			p_IsTargetSelected = false;
			HaveObjectSelected(false);
		}
	}
#endif
}
/////////////////////////////////////////////////////
//--------------------------------------------------
/////////////////////////////////////////////////////
#if ENABLE_EDITOR
bool	EntityObject::IsTargetSelected()
{
	return p_IsTargetSelected;
}
void    EntityObject::IsTargetSelected(bool b)
{
	p_IsTargetSelected = b;
}
void	EntityObject::InitParam()
{
	p_IsTargetSelected = false;
	p_DisplayString = Label::createWithTTF("", fontList[0], 12);
	p_DisplayString->retain();
}
void	EntityObject::UpdateString(float dt)
{
	if (p_DisplayString)
	{
		std::string str = std::string("Position:") + std::to_string(GetPosObject().x) + std::string("-") + std::to_string(GetPosObject().y) + std::string("\n") +
			std::string("OriginSize:") + std::to_string(GetObject()->getContentSize().width) + std::string("-") + std::to_string(GetObject()->getContentSize().height) + std::string("\n") +
			std::string("DisplaySize:") + std::to_string(GetSizeObject().width) + std::string("-") + std::to_string(GetSizeObject().height) + std::string("\n") +
			std::string("CurrentRotate:") + std::to_string(GetRotateObject()) + std::string(" c ") + std::string("\n");
		p_DisplayString->setString(str);
		p_DisplayString->setPosition(GetPosObject());
	}
}
void	EntityObject::DrawString(Renderer *renderer, const Mat4& transform, uint32_t flags)
{
	if (p_DisplayString)
	{
		p_DisplayString->visit(renderer, transform, flags);
	}
}
bool	EntityObject::IsClickOnTarget(Point p)
{
	Rect rec(GetPosObject().x - 10.f, GetPosObject().y - 10.f, 20, 20);
	if (rec.containsPoint(p))
		return true;
	return false;
}
void	EntityObject::SeftRotate(CONTROL_TYPE controlType)
{
	if ((controlType == CONTROL_TYPE::ROTATE_FORWARD ||
		controlType == CONTROL_TYPE::ROTATE_BACKWARD)
		&& IsTargetSelected()
		)
	{
		float s_angleRotate = GetRotateObject();
		if (controlType == CONTROL_TYPE::ROTATE_FORWARD) //forward
		{
			s_angleRotate -= p_SpeedSeftRotate;
		}
		else if (controlType == CONTROL_TYPE::ROTATE_BACKWARD) //backward
		{
			s_angleRotate += p_SpeedSeftRotate;
		}
		//
		if (s_angleRotate >= 360)
		{
			float delta = s_angleRotate - 360;
			s_angleRotate = delta;
		}
		else if (s_angleRotate < 0)
		{
			float delta = 360 + s_angleRotate;
			s_angleRotate = delta;
		}
		//
		SetRotateObject(s_angleRotate);
	}
}
void	EntityObject::UpdateControlEdit()
{
	if (InputMgr->GetNumberTouch() > 0 && IsTargetSelected())
	{
		MoveToPoint(InputMgr->GetPosAtId(0));
	}
	else if (InputMgr->GetNumberTouch() > 0)
	{
		if (IsClickOnTarget(InputMgr->GetPosAtId(0)) && !IsTargetSelected() && !HaveObjectSelected())
		{
			IsTargetSelected(true);
			HaveObjectSelected(true);
		}
	}
}
#endif
/////////////////////////////////////////////////////
//--------------------------------------------------
/////////////////////////////////////////////////////
//calculate seft rotate and rotate around an entity
void EntityObject::CalculateRotateAroundObject(float dt, EntityObject * e, CONTROL_TYPE controlType)
{
	if (e)
	{
		float speedRotate = 0;
		if (e->IsRotate())
		{
			speedRotate = -(SyncWithFPS(e->p_SpeedAroundRotate* dt * 100.f));
		}
		//
		if (controlType == CONTROL_TYPE::MOVE_FORWARD) //forward
		{
			speedRotate += SyncWithFPS(p_SpeedAroundRotate * dt * 100.f);
		}
		else if (controlType == CONTROL_TYPE::MOVE_BACKWARD) //backward
		{
			speedRotate -= SyncWithFPS(p_SpeedAroundRotate * dt * 100.f);
		}
		SetRotateAroundAPoint(e->GetPosObject(), speedRotate);
		if (!e->IsStopMove())
		{
			float rot = speedRotate;
			rot = (rot)* (M_PI / 180); // Convert to radians
			float X = (float)(cos(rot) * (p_PointToMoveTo.x - e->PointToMoveTo().x) - sin(rot) * (p_PointToMoveTo.y - e->PointToMoveTo().y) + e->PointToMoveTo().x);
			float Y = (float)(sin(rot) * (p_PointToMoveTo.x - e->PointToMoveTo().x) + cos(rot) * (p_PointToMoveTo.y - e->PointToMoveTo().y) + e->PointToMoveTo().y);


			p_PointToMoveTo.x = X;
			p_PointToMoveTo.y = Y;
		}
	}
}
/////////////////////////////////////////////////////
//--------------------------------------------------
/////////////////////////////////////////////////////
void EntityObject::CalculateSelfRotateObject(float dt, EntityObject * e, CONTROL_TYPE controlType)
{
	if (e)
	{
		float s_angleRotate = GetRotateObject();
		if (e->IsRotate())
		{
			s_angleRotate += SyncWithFPS(e->p_SpeedAroundRotate * dt * 100.f);
		}
		if (controlType == CONTROL_TYPE::MOVE_FORWARD) //forward
		{
			s_angleRotate -= SyncWithFPS(p_SpeedAroundRotate * dt * 100.f);
			Flip(true);
		}
		else if (controlType == CONTROL_TYPE::MOVE_BACKWARD) //backward
		{
			s_angleRotate += SyncWithFPS(p_SpeedAroundRotate* dt * 100.f);
			Flip(false);
		}
		//
		if (s_angleRotate >= 360)
		{
			float delta = s_angleRotate - 360;
			s_angleRotate = delta;
		}
		else if (s_angleRotate < 0)
		{
			float delta = 360 + s_angleRotate;
			s_angleRotate = delta;
		}
		//
		SetRotateObject(s_angleRotate);
	}
}
/////////////////////////////////////////////////////
//--------------------------------------------------
/////////////////////////////////////////////////////
void EntityObject::CalculateSelfRotateAPoint(float dt, float r, CONTROL_TYPE controlType)
{
	float s_angleRotate = GetRotateObject();

	if (controlType == CONTROL_TYPE::MOVE_FORWARD) //forward
	{
		s_angleRotate -= SyncWithFPS(r * dt * 100.f);
	}
	else if (controlType == CONTROL_TYPE::MOVE_BACKWARD) //backward
	{
		s_angleRotate += SyncWithFPS(r * dt * 100.f);
	}
	//
	if (s_angleRotate >= 360)
	{
		float delta = s_angleRotate - 360;
		s_angleRotate = delta;
	}
	else if (s_angleRotate < 0)
	{
		float delta = 360 + s_angleRotate;
		s_angleRotate = delta;
	}
	//
	SetRotateObject(s_angleRotate);

}

/////////////////////////////////////////////////////
//--------------------------------------------------
/////////////////////////////////////////////////////
BYTE * EntityObject::GetTint()
{
	return p_currentTint;
}
/////////////////////////////////////////////////////
//--------------------------------------------------
/////////////////////////////////////////////////////
void EntityObject::SetTint(BYTE * p)
{
	p_currentTint = p;
	Color3B color = Color3B(p[0], p[1], p[2]);
	GetObject()->setColor(color);
}
/////////////////////////////////////////////////////
//--------------------------------------------------
/////////////////////////////////////////////////////
void EntityObject::SetTint(Color3B color)
{
	p_currentTint[0] = color.r;
	p_currentTint[1] = color.g;
	p_currentTint[2] = color.b;
	GetObject()->setColor(color);
}
/////////////////////////////////////////////////////
//--------------------------------------------------
/////////////////////////////////////////////////////
BYTE EntityObject::GetTintChannel(int channel)
{
	if (channel >= 3 || channel < 0)
		return 0;
	return p_currentTint[channel];
}
