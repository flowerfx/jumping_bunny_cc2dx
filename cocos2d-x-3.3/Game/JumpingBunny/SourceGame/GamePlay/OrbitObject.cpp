#include "OrbitObject.h"
#include "Utility/ObjectTexture.h"
#include "GamePlay/MainCharacterObject.h"
#include "GamePlay/APManager.h"
OrbitObject::OrbitObject()
{
	p_Radius = 0.f;
	p_OriginPoint = Point(0, 0);
	p_SpeedAroundRotate = 1.f;
	p_IsRotate = true;
	p_IsTheCharacterOnTheOrbit = false;
	p_Ratio = 1;
	p_PosXDead = -(XMLMgr->GetOrbitData()->pointAlive);
	p_IsMoveWithDirect = false;
	p_DirectToMoveWithDirect = Point(0, 0);
	p_DeltaMoveDistance = Vec2(0, 0);
	p_SpeedMoveWithDirect = 0.f;
	p_CurrentTypeOrbit = TYPE_ORBIT::ORBIT_DEFAULT;
	p_isExplosive = false;
	p_MaxDistanceMoveWithDirect = Vec2(0, 0);
	p_Bubble = NULL;
	p_ObjectDecorates.clear();
	p_ObjectInteractives.clear();

	p_OriginPointRotateAround = Vec2(0,0);
	p_SpeedRotateAroundAPoint = 0;
	p_IsRotateAroundAPoint = false;

	p_OriginCDTime = p_CurrentCDTime = 0;
	//
	p_activeBomb = false;
}


OrbitObject::~OrbitObject()
{
	FreeOrbit();
}
/////////////////////////////////////////////////////
//--------------------------------------------------
/////////////////////////////////////////////////////
void	OrbitObject::RemoveObjectAtIndex(int idx, TYPE_OBJECT type )
{
	if (type == TYPE_OBJECT::DECORATE)
	{
		if (idx < p_ObjectDecorates.size())
		{
			p_ObjectDecorates.erase(p_ObjectDecorates.begin() + idx );
		}
	}
	else if (type == TYPE_OBJECT::INTERACTIVE)
	{
		if (idx < p_ObjectInteractives.size())
		{
			p_ObjectInteractives.erase(p_ObjectInteractives.begin() + idx);
		}
	}
}
/////////////////////////////////////////////////////
//--------------------------------------------------
/////////////////////////////////////////////////////
void	OrbitObject::AddObject(EntityObject* obj, TYPE_OBJECT type)
{
	if (type == TYPE_OBJECT::DECORATE)
	{
		p_ObjectDecorates.insert(p_ObjectDecorates.size() - 1, obj);
	}
	else if (type == TYPE_OBJECT::INTERACTIVE)
	{
		p_ObjectInteractives.insert(p_ObjectInteractives.size() - 1, obj);
	}
}
/////////////////////////////////////////////////////
//--------------------------------------------------
/////////////////////////////////////////////////////
void	OrbitObject::ReplaceObjectAtIndex(int idx, EntityObject* obj, TYPE_OBJECT type )
{
	if (type == TYPE_OBJECT::DECORATE)
	{
		if (idx < p_ObjectDecorates.size())
		{
			p_ObjectDecorates.replace(idx, obj);
		}
	}
	else if (type == TYPE_OBJECT::INTERACTIVE)
	{
		if (idx < p_ObjectInteractives.size())
		{
			p_ObjectInteractives.replace(idx, obj);
		}
	}
}
/////////////////////////////////////////////////////
//--------------------------------------------------
/////////////////////////////////////////////////////
void	OrbitObject::LoadOrbitObject(int TypeOfLoad, std::string name, Texture2D * t, Sprite * p, TYPE_ORBIT typeOrbit, ObjectGamePlayDetail * obj)
{
	LoadTexture(TypeOfLoad, name, t, p);
	//
	p_CurrentTypeOrbit = typeOrbit;

	p_Radius = GetSizeObject().width / 2;

	if (obj)
	{
		p_SpeedMove = obj->speed_move;
		p_OriginSpeedMove = p_SpeedMove;
		//
		p_SpeedAroundRotate = obj->speed_rotate;
		//
		p_SpeedMoveWithDirect = obj->speed_move;
		p_OriginSpeedMoveWithDirect = p_SpeedMoveWithDirect;
		//
		p_IsMoveWithDirect = obj->direct_move != Vec2(0, 0);
		if (p_IsMoveWithDirect)
		{
			p_DirectToMoveWithDirect = obj->direct_move;
			//
			float dis = obj->distance_move * ((obj->direct_move.x != 0) ? GetWinSize().width : GetWinSize().height);
			Vec2 Pos = Vec2(obj->pos.x * GetWinSize().width, obj->pos.y * GetWinSize().height);
			//
			float max = (abs(obj->direct_move.x) * Pos.x + dis / 2) + (abs(obj->direct_move.y) * Pos.y + dis / 2);
			float min = (abs(obj->direct_move.x) * Pos.x - dis / 2) + (abs(obj->direct_move.y) * Pos.y - dis / 2);
			//
			p_MaxDistanceMoveWithDirect = Vec2(max ,min);
		}
		p_IsRotateAroundAPoint = (obj->point_rotate != Vec2(0, 0));
		if (p_IsRotateAroundAPoint)
		{
			p_OriginPointRotateAround = Vec2(obj->point_rotate.x * GetWinSize().width, obj->point_rotate.y * GetWinSize().height);
			p_SpeedRotateAroundAPoint = obj->speed_rotate_point;
		}
		if (p_CurrentTypeOrbit == TYPE_ORBIT::ORBIT_TIME_BOOM)
		{
			p_OriginCDTime = p_CurrentCDTime = 5.f;
		}

		p_OrbitIdxStickTo = obj->stick_idx_orbit;
	}
	else
	{
		p_SpeedMove = XMLMgr->GetOrbitData()->speedMove;
		p_OriginSpeedMove = p_SpeedMove;
		//
		p_SpeedAroundRotate = XMLMgr->GetOrbitData()->speedRotate;
		//
		p_SpeedMoveWithDirect = XMLMgr->GetOrbitData()->speedMoveUpDown;
		p_OriginSpeedMoveWithDirect = p_SpeedMoveWithDirect;
		//
		if (p_CurrentTypeOrbit == TYPE_ORBIT::ORBIT_TIME_BOOM)
		{
			p_OriginCDTime = p_CurrentCDTime = 5.f;
		}
		//
		if (ManageObjectAP::GetInstance()->GetScore() >= 10)
		{
			int curScore = ManageObjectAP::GetInstance()->GetScore();
			if (curScore <= 30)
			{
				p_IsMoveWithDirect = (GetRandom(1, 5) == 1);
			}
			else if (curScore <= 70)
			{
				p_IsMoveWithDirect = (GetRandom(1, 4) == 1);
			}
			else if (curScore <= 110)
			{
				p_IsMoveWithDirect = (GetRandom(1, 3) == 1);
			}
			else if (curScore > 110)
			{
				p_IsMoveWithDirect = (GetRandom(1, 2) == 1);
			}
			if (p_IsMoveWithDirect)
			{
				p_SpeedMoveWithDirect = p_SpeedMoveWithDirect * ManageObjectAP::GetInstance()->GetScore() / 5.f;
				if (p_SpeedMoveWithDirect > 10.f)
				{
					p_SpeedMoveWithDirect = 10.f;
				}
				p_OriginSpeedMoveWithDirect = p_SpeedMoveWithDirect;
				p_DirectToMoveWithDirect = Point(0, 1);
				//
				//
				p_MaxDistanceMoveWithDirect = Vec2(GetWinSize().height * 4.f / 5.f, GetWinSize().height / 5.f);
			}
		}
	}

	if (p_DirectToMoveWithDirect.x != 0 && p_DirectToMoveWithDirect.y != 0)
	{
		CC_ASSERT(false, "vari p_DirectToMoveWithDirect not have same x and y != 0 \n it's not implement");
	}

	SetTint(Color3B::WHITE);

}
/////////////////////////////////////////////////////
//--------------------------------------------------
/////////////////////////////////////////////////////
void	OrbitObject::LoadTexture(int TypeOfLoad, std::string name, Texture2D * t, Sprite * p)
{
	bool loaded = false;
	if (TypeOfLoad == 0)
	{
		LoadObjectFromSingleImage(name);
		loaded = true;
	}
	else if (TypeOfLoad == 1)
	{
		LoadObjectFromData(TYPE_TEXTURE::OBJECT_TEXTURE, name);
		loaded = true;
	}
	else if (TypeOfLoad == 2)
	{
		LoadObjectFromSingleTexture(t);
		loaded = true;
	}
	else if (TypeOfLoad == 3)
	{
		LoadObjectFromSprite(p, false);
		loaded = true;
	}
	//no load image available
	CCASSERT(loaded, "wrong method load image or error in loading image !");
}
/////////////////////////////////////////////////////
//--------------------------------------------------
/////////////////////////////////////////////////////
float	OrbitObject::GetRotation()
{
	return GetRotateObject();
}
void	OrbitObject::SetRotation(float r)
{
	SetRotateObject(r);
}
/////////////////////////////////////////////////////
//--------------------------------------------------
/////////////////////////////////////////////////////
void OrbitObject::UpdateMoveDirect(float dt)
{
	if (p_IsMoveWithDirect)
	{
		//move with direct
		if (!APMgr->IsOnStoryMode())
		{
			if (APMgr->IsUseTheBoost(TYPE_BOOST::B_JUMP_BOOST))
			{
				p_SpeedMoveWithDirect = p_OriginSpeedMoveWithDirect;
			}
			else
			{
				p_SpeedMoveWithDirect += (dt / 20.f);
			}
			//
			if (p_SpeedMoveWithDirect > p_OriginSpeedMoveWithDirect * 2.f)
			{
				p_SpeedMoveWithDirect = p_OriginSpeedMoveWithDirect * 2.f;
			}

			if (APMgr->GetCurrentGamePlay() == TYPE_GAMEPLAY::GAMEPLAY_PLAY_STORY)
			{
				p_SpeedMoveWithDirect = p_OriginSpeedMoveWithDirect;
			}
		}

		Point pointPosOr = GetPosOrbit();
		if (p_OrbitIdxStickTo <= -1)
		{
			p_DeltaMoveDistance = p_DirectToMoveWithDirect * SyncWithFPS(dt * 100.f * p_SpeedMoveWithDirect);
		}
		pointPosOr += (p_DirectToMoveWithDirect * SyncWithFPS(dt * 100.f *p_SpeedMoveWithDirect));
		SetPosOrbit(pointPosOr);

		if (!(p_CurrentTypeOrbit == TYPE_ORBIT::ORBIT_BOOM
			|| p_CurrentTypeOrbit == TYPE_ORBIT::ORBIT_BULLET))
		{
			p_Bubble->SetPosObject(pointPosOr);
			for (int i = 0; i < p_ObjectDecorates.size(); i++)
			{
				Point pos = p_ObjectDecorates.at(i)->GetPosObject();
				pos += (p_DirectToMoveWithDirect * SyncWithFPS(dt * 100.f * p_SpeedMoveWithDirect));
				p_ObjectDecorates.at(i)->SetPosObject(pos);
			}
			for (int i = 0; i < p_ObjectInteractives.size(); i++)
			{
				Point pos = p_ObjectInteractives.at(i)->GetPosObject();
				pos += (p_DirectToMoveWithDirect * SyncWithFPS(dt * 100.f * p_SpeedMoveWithDirect));
				p_ObjectInteractives.at(i)->SetPosObject(pos);
			}
			//
			float distance = sqrt((GetChar->GetPosObject().x - GetPosOrbit().x) * (GetChar->GetPosObject().x - GetPosOrbit().x) +
				(GetChar->GetPosObject().y - GetPosOrbit().y) * (GetChar->GetPosObject().y - GetPosOrbit().y));
			if (IsTheCharacterOnTheOrbit() && distance <= GetRadius() + (GetChar->GetSizeMainChar().height * GetChar->GetRatioSizeDistance()))
			{
				Point pos = GetChar->GetPosObject();
				pos += (p_DirectToMoveWithDirect * SyncWithFPS(dt * 100.f * p_SpeedMoveWithDirect));
				GetChar->SetPosMainChar(pos);
				pos = (GetChar->GetArrowPoint()->GetPosObject());
				pos += (p_DirectToMoveWithDirect * SyncWithFPS(dt * 100.f * p_SpeedMoveWithDirect));
				GetChar->GetArrowPoint()->SetPosObject(pos);
			}
		}

		if (p_DirectToMoveWithDirect.y != 0)
		{
			if (pointPosOr.y >= p_MaxDistanceMoveWithDirect.x || pointPosOr.y <= p_MaxDistanceMoveWithDirect.y)
			{
				p_DirectToMoveWithDirect = -p_DirectToMoveWithDirect;
			}
		}
		else if (p_DirectToMoveWithDirect.x != 0)
		{
			if (pointPosOr.x >= p_MaxDistanceMoveWithDirect.x || pointPosOr.x <= p_MaxDistanceMoveWithDirect.y)
			{
				p_DirectToMoveWithDirect = -p_DirectToMoveWithDirect;
			}
		}
	}
}


void	OrbitObject::Update(float dt)
{
#if ENABLE_EDITOR
	if (InputMgr->GetNumberTouch() > 0 && IsTargetSelected())
	{
		MoveOrbitToPoint(InputMgr->GetPosAtId(0));
	}
	else if (InputMgr->GetNumberTouch() > 0)
	{
		if (IsClickOnTarget(InputMgr->GetPosAtId(0)) && !IsTargetSelected() && !HaveObjectSelected())
		{
			IsTargetSelected(true);
			HaveObjectSelected(true);
		}
	}
	ToggleRotate(InputMgr->GetCurrentControlType());
#endif

	auto orbit_stick = OBJAPMgr->GetOrbitAtIdx(p_OrbitIdxStickTo);

	if (!GetChar->IsMoveToOrbitInit())
	{
		MoveOrbitToPoint(Point(p_PosXDead * 2, GetPosOrbit().y));
	}
	else
	{
		p_DurationMove = 0;
	}

	//move
	if (APMgr->IsUseTheBoost(TYPE_BOOST::B_JUMP_BOOST))
	{
		p_SpeedMove = p_OriginSpeedMove;
	}
	else
	{
		p_SpeedMove += (dt / 50.f);
	}
	if (p_SpeedMove > p_OriginSpeedMove * 3.f)
	{
		p_SpeedMove = p_OriginSpeedMove * 3.f;
	}


	//rotate the orbit
	float angleRotate = GetRotation();
	if (p_IsRotate)
	{
		angleRotate += SyncWithFPS(dt * 100.f * p_SpeedAroundRotate);
	}
	if (angleRotate >= 360)
	{
		float delta = angleRotate - 360;
		angleRotate = delta;
	}
	SetRotation(angleRotate);

	//rotate the orbit around a point
	if (p_IsRotateAroundAPoint && !orbit_stick)
	{
		RotateOrbitAndComponentAroudAPoint(p_OriginPointRotateAround, p_SpeedRotateAroundAPoint);
	}
	else if (orbit_stick)
	{
		//map the position of orbit stick when it move or rotate around another point
		if (orbit_stick->IsMoveWithDirect())
		{
			Point cur_point = GetPosOrbit();
			cur_point += orbit_stick->GetDeltaDistanceMoveDirect();
			if (p_OrbitIdxStickTo > -1)
			{
				p_DeltaMoveDistance = orbit_stick->GetDeltaDistanceMoveDirect();
			}
			SetPosObject(cur_point);
			if (!(p_CurrentTypeOrbit == TYPE_ORBIT::ORBIT_BOOM
				|| p_CurrentTypeOrbit == TYPE_ORBIT::ORBIT_BULLET))
			{
				p_Bubble->SetPosObject(cur_point);
				for (int i = 0; i < p_ObjectDecorates.size(); i++)
				{
					Point pos = p_ObjectDecorates.at(i)->GetPosObject();
					pos += orbit_stick->GetDeltaDistanceMoveDirect();
					p_ObjectDecorates.at(i)->SetPosObject(pos);
				}
				for (int i = 0; i < p_ObjectInteractives.size(); i++)
				{
					Point pos = p_ObjectInteractives.at(i)->GetPosObject();
					pos += orbit_stick->GetDeltaDistanceMoveDirect();
					p_ObjectInteractives.at(i)->SetPosObject(pos);
				}
				//
				float distance = sqrt((GetChar->GetPosObject().x - GetPosOrbit().x) * (GetChar->GetPosObject().x - GetPosOrbit().x) +
					(GetChar->GetPosObject().y - GetPosOrbit().y) * (GetChar->GetPosObject().y - GetPosOrbit().y));
				if (IsTheCharacterOnTheOrbit() && distance <= GetRadius() + (GetChar->GetSizeMainChar().height * GetChar->GetRatioSizeDistance()))
				{
					Point pos = GetChar->GetPosObject();
					pos += orbit_stick->GetDeltaDistanceMoveDirect();
					GetChar->SetPosMainChar(pos);
					pos = (GetChar->GetArrowPoint()->GetPosObject());
					pos += orbit_stick->GetDeltaDistanceMoveDirect();
					GetChar->GetArrowPoint()->SetPosObject(pos);
				}
			}
		}
		//
		if (orbit_stick->IsRotateAround())
		{
			float speed_r_orbit = orbit_stick->GetSpeedRotateAroundAPoint();
			CalculateSelfRotateAPoint(speed_r_orbit, speed_r_orbit > 0 ? CONTROL_TYPE::MOVE_FORWARD : CONTROL_TYPE::MOVE_BACKWARD);
			SetRotateAroundAPoint(orbit_stick->GetPointRotateAround(), speed_r_orbit);
			if (!(p_CurrentTypeOrbit == TYPE_ORBIT::ORBIT_BOOM
				|| p_CurrentTypeOrbit == TYPE_ORBIT::ORBIT_BULLET))
			{
				p_Bubble->CalculateSelfRotateAPoint(speed_r_orbit, speed_r_orbit > 0 ? CONTROL_TYPE::MOVE_FORWARD : CONTROL_TYPE::MOVE_BACKWARD);
				p_Bubble->SetRotateAroundAPoint(orbit_stick->GetPointRotateAround(), speed_r_orbit);
				for (int i = 0; i < p_ObjectDecorates.size(); i++)
				{
					p_ObjectDecorates.at(i)->CalculateSelfRotateAPoint(speed_r_orbit, speed_r_orbit > 0 ? CONTROL_TYPE::MOVE_FORWARD : CONTROL_TYPE::MOVE_BACKWARD);
					p_ObjectDecorates.at(i)->SetRotateAroundAPoint(orbit_stick->GetPointRotateAround(), speed_r_orbit);
				}
				for (int i = 0; i < p_ObjectInteractives.size(); i++)
				{
					p_ObjectInteractives.at(i)->CalculateSelfRotateAPoint(speed_r_orbit, speed_r_orbit > 0 ? CONTROL_TYPE::MOVE_FORWARD : CONTROL_TYPE::MOVE_BACKWARD);
					p_ObjectInteractives.at(i)->SetRotateAroundAPoint(orbit_stick->GetPointRotateAround(), speed_r_orbit);
				}
				//
				float distance = sqrt((GetChar->GetPosObject().x - GetPosOrbit().x) * (GetChar->GetPosObject().x - GetPosOrbit().x) +
					(GetChar->GetPosObject().y - GetPosOrbit().y) * (GetChar->GetPosObject().y - GetPosOrbit().y));
				if (IsTheCharacterOnTheOrbit() && distance <= GetRadius() + (GetChar->GetSizeMainChar().height * GetChar->GetRatioSizeDistance()))
				{

					GetChar->CalculateSelfRotateAPoint(speed_r_orbit, speed_r_orbit > 0 ? CONTROL_TYPE::MOVE_FORWARD : CONTROL_TYPE::MOVE_BACKWARD);
					GetChar->SetRotateAroundAPoint(orbit_stick->GetPointRotateAround(), speed_r_orbit);
					GetChar->GetArrowPoint()->CalculateSelfRotateAPoint(speed_r_orbit, speed_r_orbit > 0 ? CONTROL_TYPE::MOVE_FORWARD : CONTROL_TYPE::MOVE_BACKWARD);
					GetChar->GetArrowPoint()->SetRotateAroundAPoint(orbit_stick->GetPointRotateAround(), speed_r_orbit);
				}
			}
		}
		//
		RotateOrbitAndComponentAroudAPoint(orbit_stick->GetPosOrbit(), p_SpeedRotateAroundAPoint);
	}

	//update as direct
	UpdateMoveDirect(dt);

	//update parent
	updateObject(dt);
	//
	if (p_isExplosive)
		return;
	//
	if (p_CurrentTypeOrbit == TYPE_ORBIT::ORBIT_BOOM 
		|| p_CurrentTypeOrbit == TYPE_ORBIT::ORBIT_BULLET)
		return;

	if (p_CurrentTypeOrbit == TYPE_ORBIT::ORBIT_TIME_BOOM)
	{
		if (p_activeBomb)
		{
			//
			auto cur_tin = GetTint();
			cur_tin[0] -= dt * (128.f / p_OriginCDTime);
			cur_tin[1] -= dt * (128.f / p_OriginCDTime);
			cur_tin[2] -= dt * (128.f / p_OriginCDTime);
			SetTint(cur_tin);
			//
			p_CurrentCDTime -= dt;
			if (p_CurrentCDTime <= 0)
			{
				p_CurrentCDTime = 0;
				return;
			}
		}
	}

	//bubble update	
	p_Bubble->CalculateSelfRotateObject(dt,this);
	if (this->IsStopMove())
	{
		p_Bubble->DurationMove(0);
	}
	p_Bubble->SpeedMove(p_SpeedMove);
	p_Bubble->updateObject(dt);
	
	//deco and interaction update
	for (int i = 0; i < p_ObjectDecorates.size(); i++)
	{
#if ENABLE_EDITOR
		p_ObjectDecorates.at(i)->UpdateControlEdit();
#endif
		p_ObjectDecorates.at(i)->SpeedMove(p_SpeedMove);
		p_ObjectDecorates.at(i)->CalculateRotateAroundObject(dt,this);
		p_ObjectDecorates.at(i)->CalculateSelfRotateObject(dt,this);
		if (this->IsStopMove())
		{
			p_ObjectDecorates.at(i)->DurationMove(0);
		}
		p_ObjectDecorates.at(i)->updateObject(dt);
#if ENABLE_EDITOR
		p_ObjectDecorates.at(i)->SeftRotate(InputMgr->GetCurrentControlType());
#endif
	}
	for (int i = 0; i < p_ObjectInteractives.size(); i++)
	{
#if ENABLE_EDITOR
		p_ObjectInteractives.at(i)->UpdateControlEdit();
#endif
		p_ObjectInteractives.at(i)->SpeedMove(p_SpeedMove);
		if (p_CurrentTypeOrbit != TYPE_ORBIT::ORBIT_TIME_BOOM)
		{
			p_ObjectInteractives.at(i)->CalculateRotateAroundObject(dt,this);
			p_ObjectInteractives.at(i)->CalculateSelfRotateObject(dt,this);
		}
		if (this->IsStopMove())
		{
			p_ObjectInteractives.at(i)->DurationMove(0);
		}
		p_ObjectInteractives.at(i)->updateObject(dt);
#if ENABLE_EDITOR
		p_ObjectInteractives.at(i)->SeftRotate(InputMgr->GetCurrentControlType());
#endif
	}
}
/////////////////////////////////////////////////////
//--------------------------------------------------
/////////////////////////////////////////////////////
void OrbitObject::RotateOrbitAndComponentAroudAPoint(Vec2 point, float r)
{
	float speed_r_orbit = GetSpeedRotateAroundAPoint();
	CalculateSelfRotateAPoint(speed_r_orbit, speed_r_orbit > 0 ? CONTROL_TYPE::MOVE_FORWARD : CONTROL_TYPE::MOVE_BACKWARD);
	SetRotateAroundAPoint(point, r);
	if (p_Bubble)
	{
		p_Bubble->SetRotateAroundAPoint(point, r);
		p_Bubble->CalculateSelfRotateAPoint(speed_r_orbit, speed_r_orbit > 0 ? CONTROL_TYPE::MOVE_FORWARD : CONTROL_TYPE::MOVE_BACKWARD);
	}
	for (int i = 0; i < p_ObjectDecorates.size(); i++)
	{
		p_ObjectDecorates.at(i)->SetRotateAroundAPoint(point, r);
		p_ObjectDecorates.at(i)->CalculateSelfRotateAPoint(speed_r_orbit, speed_r_orbit > 0 ? CONTROL_TYPE::MOVE_FORWARD : CONTROL_TYPE::MOVE_BACKWARD);
	}
	for (int i = 0; i < p_ObjectInteractives.size(); i++)
	{
		p_ObjectInteractives.at(i)->SetRotateAroundAPoint(point, r);
		p_ObjectInteractives.at(i)->CalculateSelfRotateAPoint(speed_r_orbit, speed_r_orbit > 0 ? CONTROL_TYPE::MOVE_FORWARD : CONTROL_TYPE::MOVE_BACKWARD);
	}
}
/////////////////////////////////////////////////////
//--------------------------------------------------
/////////////////////////////////////////////////////
//use the normal draw only
void	OrbitObject::onDrawOrbit(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch)
{
	if (p_isExplosive)
		return;
	for (int i = 0; i < p_ObjectDecorates.size(); i++)
	{
		p_ObjectDecorates.at(i)->onDrawObject(renderer, transform, flags, spriteBatch);
	}
	onDrawObject(renderer, transform, flags, spriteBatch);
	//interractive object will be drawn at the parent draw 
}
/////////////////////////////////////////////////////
//--------------------------------------------------
/////////////////////////////////////////////////////
void	OrbitObject::onVisitOrbit(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch)
{
	if (p_isExplosive)
		return;
	for (int i = 0; i < p_ObjectDecorates.size(); i++)
	{
		p_ObjectDecorates.at(i)->onVisitObject(renderer, transform, flags, spriteBatch);
	}
	onVisitObject(renderer, transform, flags, spriteBatch);
	//interractive object will be drawn at the parent draw 
}
/////////////////////////////////////////////////////
//draw bubble
/////////////////////////////////////////////////////
void OrbitObject::onDrawBubble(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch)
{
	if (p_CurrentTypeOrbit == TYPE_ORBIT::ORBIT_BOOM
		|| p_CurrentTypeOrbit == TYPE_ORBIT::ORBIT_BULLET)
		return;
	p_Bubble->onDrawObject(renderer, transform, flags, spriteBatch);
}
//
void OrbitObject::onVisitBubble(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch)
{
	if (p_CurrentTypeOrbit == TYPE_ORBIT::ORBIT_BOOM
		|| p_CurrentTypeOrbit == TYPE_ORBIT::ORBIT_BULLET)
		return;
	p_Bubble->onVisitObject(renderer, transform, flags, spriteBatch);
}
/////////////////////////////////////////////////////
//draw interractive object
/////////////////////////////////////////////////////
void OrbitObject::onDrawInteractiveObj(Renderer *renderer, const Mat4& transform, uint32_t flags,SpriteBatchNode * spriteBatch)
{
	if (p_isExplosive)
		return;
	for (int i = 0; i < p_ObjectInteractives.size(); i++)
	{
		p_ObjectInteractives.at(i)->onDrawObject(renderer, transform, flags, spriteBatch);
	}
}
void OrbitObject::onVisitInteractiveObj(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch)
{
	if (p_isExplosive)
		return;
	for (int i = 0; i < p_ObjectInteractives.size(); i++)
	{
		p_ObjectInteractives.at(i)->onVisitObject(renderer, transform, flags, spriteBatch);
	}
}
/////////////////////////////////////////////////////
//--------------------------------------------------
/////////////////////////////////////////////////////
void	OrbitObject::MoveOrbitToPoint(Point p)
{
	if (APMgr->GetCurrentGamePlay() == TYPE_GAMEPLAY::GAMEPLAY_PLAY_STORY)
		return;
	MoveToPoint(p);
	if (p_CurrentTypeOrbit == TYPE_ORBIT::ORBIT_BOOM
		|| p_CurrentTypeOrbit == TYPE_ORBIT::ORBIT_BULLET)
		return;
	p_Bubble->MoveToPoint(p);
	for (int i = 0; i < p_ObjectDecorates.size(); i++)
	{
		Point p_toMove = p + (p_ObjectDecorates.at(i)->GetPosObject() - GetPosObject());
		p_ObjectDecorates.at(i)->MoveToPoint(p_toMove);
	}
	for (int i = 0; i < p_ObjectInteractives.size(); i++)
	{
		Point p_toMove = p + (p_ObjectInteractives.at(i)->GetPosObject() - GetPosObject());
		p_ObjectInteractives.at(i)->MoveToPoint(p_toMove);
	}
	//
	float distance = sqrt((GetChar->GetPosObject().x - GetPosOrbit().x) * (GetChar->GetPosObject().x - GetPosOrbit().x) +
		(GetChar->GetPosObject().y - GetPosOrbit().y) * (GetChar->GetPosObject().y - GetPosOrbit().y));
	if (IsTheCharacterOnTheOrbit() && distance <= GetRadius() + (GetChar->GetSizeMainChar().height * GetChar->GetRatioSizeDistance()))
	{
		Point p_toMove = p + (GetChar->GetPosObject() - GetPosObject());
		GetChar->MoveToPoint(p_toMove);
		p_toMove = p + (GetChar->GetArrowPoint()->GetPosObject() - GetPosObject());
		GetChar->GetArrowPoint()->MoveToPoint(p_toMove);
	}
}
#if ENABLE_EDITOR
void	OrbitObject::ToggleRotate(CONTROL_TYPE controlType)
{
	if (IsTargetSelected())
	{
		if (controlType == CONTROL_TYPE::TOGGLE_ROTATE) //forward
		{
			p_IsRotate != p_IsRotate;
		}
	}
}
#endif
/////////////////////////////////////////////////////
//--------------------------------------------------
/////////////////////////////////////////////////////
float	OrbitObject::CalculateAngle(Point p)
{
	float valueReturn = 0.f;
	Point or_p = GetOriginPoint();

	Point origin_vector(0, GetSizeOrbit().height / 2);
	Point vector_p(p.x - or_p.x, p.y - or_p.y);
	//
	if (vector_p.x == 0 && vector_p.y == 0)
		return 0;
	//
	float cos_a = ((origin_vector.x*vector_p.x) + (origin_vector.y*vector_p.y)) /
		((sqrtf(vector_p.x*vector_p.x + vector_p.y*vector_p.y))*(sqrtf(origin_vector.x*origin_vector.x + origin_vector.y*origin_vector.y)));
//	

	valueReturn = acosf(cos_a);
	valueReturn = (valueReturn * 180 / M_PI);
//
	if (p.x < or_p.x)
	{
		valueReturn = 360 - valueReturn;
	}
//
	return valueReturn;
}
/////////////////////////////////////////////////////
//--------------------------------------------------
/////////////////////////////////////////////////////
Point	OrbitObject::GenerateThePosObject()
{
	Point returnValue(0, 0);
	Point p = GetOriginPoint();
	Size  size = GetSizeOrbit();
	int minX = p.x - (size.width / 2);
	int maxX = p.x + (size.width / 2);
#ifdef OS_IOS
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-variable"
#endif
	int delta = maxX - minX;
#ifdef OS_IOS
#pragma clang diagnostic pop
#endif
	int posX = GetRandom(maxX, minX);
	returnValue.x = posX;
//
// (x-p.x)^2 + (y-p.y)^2 = p_Radius^2
//  x = posX;
//  y = sqrt(p_Radius^2 - (x-p.x)^2) + p.y
//
	float y1 = std::sqrtf((p_Radius*p_Radius) - (posX - p.x)*(posX - p.x)) + p.y;
	float y2 = -std::sqrtf((p_Radius*p_Radius) - (posX - p.x)*(posX - p.x)) + p.y;
	float posY = (GetRandom(2,0) == 0) ? y1 : y2;
///
	returnValue.y = posY;
	return returnValue;
}
/////////////////////////////////////////////////////
//--------------------------------------------------
/////////////////////////////////////////////////////
void	OrbitObject::GenerateTypeObject(int * stateCreate)
{
	if (p_CurrentTypeOrbit == TYPE_ORBIT::ORBIT_BOOM
		|| p_CurrentTypeOrbit == TYPE_ORBIT::ORBIT_BULLET)
		return;
	//init bubble
	if (stateCreate == NULL || (stateCreate && stateCreate[0] > 0))
	{
		p_Bubble = new EntityObject();
#if USE_SPRITE_STORE
		auto sprite_bu = TextureMgr->GetSpriteFromMemory("bubble");
		p_Bubble->LoadObjectFromSprite(sprite_bu,false);
#else
		auto sprite_bu = TextureMgr->GetTexureFromMemory("bubble");
		p_Bubble->LoadObjectFromSingleTexture(sprite_bu);
#endif
		p_Bubble->SetPosObject(GetPosOrbit());

		p_Bubble->SetSizeObject(GetSizeOrbit()*INCREASE_BOOST / (TextureMgr->IsUseMipMap() ? 2.f : 1.f));
		p_Bubble->SpeedMove(p_SpeedMove);
	}
	//get deco

	int numberDeco = GetRandom((int)XMLMgr->GetOrbitData()->numbnerObjectDeco.x, (int)XMLMgr->GetOrbitData()->numbnerObjectDeco.y);
	int numberInter = GetRandom((int)XMLMgr->GetOrbitData()->numberObjectInter.x, (int)XMLMgr->GetOrbitData()->numberObjectInter.y);
	if (p_CurrentTypeOrbit == TYPE_ORBIT::ORBIT_TIME_BOOM)
	{
		numberDeco = 0;
		numberInter = 1;
	}
	if (stateCreate == NULL || (stateCreate && stateCreate[1] > 0))
	{
		for (int i = 0; i < numberDeco; i++)
		{
			EntityObject* temp = new EntityObject();
			int idxNum = XMLMgr->GetOrbitData()->numberDeco;
			if (idxNum <= 0)
				break;
			std::string posfix = "";
			if (APMgr->GetCurrentEviType() == EVIROMENT_TYPE::EV_SNOW)
			{
				idxNum += 1;
				posfix = "_snow";
			}
			int idx = GetRandom(idxNum,0);
			std::string str = "deco" + std::to_string(idx) + posfix;
#if USE_SPRITE_STORE
			auto te = TextureMgr->GetSpriteFromMemory(str);
			temp->LoadObjectFromSprite(te, false);
#else
			auto te = TextureMgr->GetTexureFromMemory(str);
			temp->LoadObjectFromSingleTexture(te);
#endif		
			temp->SetPosObject(GenerateThePosObject());
			temp->SetSizeObject((temp->GetSizeObject(false) / (2 * p_Ratio))*GetScaleFactor());
			temp->SpeedMove(p_SpeedMove);
			temp->SetRotateObject(CalculateAngle(temp->GetPosObject()));
			p_ObjectDecorates.pushBack(temp);
			//temp->autorelease();
#if ! USE_SPRITE_STORE
			te->autorelease();//note
#endif
		}
	}
	if (stateCreate == NULL || (stateCreate && stateCreate[2] > 0))
	{
		for (int i = 0; i < numberInter; i++)
		{
			EntityObject* temp = new EntityObject();
			int idxNum = XMLMgr->GetOrbitData()->numberInter;
			if (idxNum <= 0)
				break;
			std::string posfix = "";
			if (APMgr->GetCurrentEviType() == EVIROMENT_TYPE::EV_SNOW)
			{
				posfix = "_snow";
			}
			int idx = GetRandom(idxNum, 0);
			std::string str = "inter" + std::to_string(idx) + posfix;
			if (p_CurrentTypeOrbit == TYPE_ORBIT::ORBIT_TIME_BOOM)
			{
				str = "time_bomb";
			}
#if USE_SPRITE_STORE
			auto te = TextureMgr->GetSpriteFromMemory(str);
			temp->LoadObjectFromSprite(te,false);
#else
			auto te = TextureMgr->GetTexureFromMemory(str);
			temp->LoadObjectFromSingleTexture(te);
#endif
			//temp->SetPosObject(GenerateThePosObject());
			//temp->SetSizeObject((temp->GetSizeObject() / (2 * p_Ratio))*GetScaleFactor());
			temp->SetPosObject(GetOriginPoint());
			temp->SetSizeObject((temp->GetSizeObject(false) / p_Ratio)  *GetScaleFactor());
			temp->SpeedMove(p_SpeedMove);
			temp->SetRotateObject(CalculateAngle(temp->GetPosObject()));
			p_ObjectInteractives.pushBack(temp);
			//temp->autorelease(); //note
#if ! USE_SPRITE_STORE
			te->autorelease();
#endif
		}
	}
}
/////////////////////////////////////////////////////
//--------------------------------------------------
/////////////////////////////////////////////////////
float OrbitObject::EffectRaius()
{
	if (APMgr->IsUseTheBoost(TYPE_BOOST::B_GRAVITY_BOOST) && !(p_CurrentTypeOrbit == TYPE_ORBIT::ORBIT_BOOM
		|| p_CurrentTypeOrbit == TYPE_ORBIT::ORBIT_BULLET))
	{
		return p_Radius * INCREASE_BOOST;
	}
	else
	{
		return p_Radius * 7.f / 6.f;
	}
}
/////////////////////////////////////////////////////
//--------------------------------------------------
/////////////////////////////////////////////////////
void OrbitObject::SetPosOrbitWithDistance(float distance)
{
	//set pos orbit
	Point cur_pos  = GetPosOrbit();
	cur_pos.x -= distance;
	SetPosOrbit(cur_pos);
	if (p_CurrentTypeOrbit == TYPE_ORBIT::ORBIT_BOOM
		|| p_CurrentTypeOrbit == TYPE_ORBIT::ORBIT_BULLET)
		return;
	//set pos bubble
	p_Bubble->SetPosObject(cur_pos);
	//deco and inter
	for (int i = 0; i < p_ObjectDecorates.size(); i++)
	{
		cur_pos = p_ObjectDecorates.at(i)->GetPosObject();
		cur_pos.x -= distance;
		p_ObjectDecorates.at(i)->SetPosObject(cur_pos);
	}
	for (int i = 0; i < p_ObjectInteractives.size(); i++)
	{
		cur_pos = p_ObjectInteractives.at(i)->GetPosObject();
		cur_pos.x -= distance;
		p_ObjectInteractives.at(i)->SetPosObject(cur_pos);
	}
		
}
/////////////////////////////////////////////////////
//--------------------------------------------------
/////////////////////////////////////////////////////
float OrbitObject::GetSpeedMoveOrbit(bool IsPreventNull /*= false*/)
{
	if (IsPreventNull)
	{
		return SpeedMove() == 0 ? XMLMgr->GetOrbitData()->speedMove : SpeedMove();
	}
	else
	{
		return SpeedMove();
	}
}
/////////////////////////////////////////////////////
//--------------------------------------------------
/////////////////////////////////////////////////////
bool OrbitObject::IsOrbitTimeOnExplosion()
{
	if (!p_isExplosive && p_CurrentTypeOrbit == TYPE_ORBIT::ORBIT_TIME_BOOM && p_CurrentCDTime <= 0)
	{
		return true;
	}
	return false;
}