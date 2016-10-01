#include "MainCharacterObject.h"
#include "ManageObjectAP.h"
#include "APManager.h"
#include "MainSceneGame.h"
#if USE_CC_AUDIO
#include "Utility/SoundManager.h"
#endif
#include "Particle/ParicleManager.h"
#include "Menu/BoostMenu.h"
/////////////////////////////////////////////////////////////
//
//AnimateSprite Class
//
/////////////////////////////////////////////////////////////
void AnimateSprite::InitValue()
{
	p_DeltaTime = 0.05f;
	p_RunTime = 0;
	p_previousTIme = 0;
	p_idxSprite = 0;
}
void AnimateSprite::PushSpriteToListAnimate(Sprite * p)
{
	p_ListActionSprite.pushBack(p);
}
void AnimateSprite::SetPosition(Point p)
{
	//for (int i = 0; i < p_ListActionSprite.size(); i++)
	{
		p_ListActionSprite.at(p_idxSprite)->setPosition(p);
	}
}
void AnimateSprite::SetSize(Point s)
{
	for (int i = 0; i < p_ListActionSprite.size(); i++)
	{
		Size p_scale(1, 1);
		p_scale.width = s.x / p_ListActionSprite.at(i)->getContentSize().width;
		p_scale.height = s.y / p_ListActionSprite.at(i)->getContentSize().height;
		if (TextureMgr->IsUseMipMap())
		{
			p_scale.width = p_scale.width * 2;
			p_scale.height = p_scale.height * 2;
		}
		p_ListActionSprite.at(i)->setScaleX(p_scale.width);
		p_ListActionSprite.at(i)->setScaleY(p_scale.height);
	}
}
void AnimateSprite::SetRotate(float r)
{
	p_ListActionSprite.at(p_idxSprite)->setRotation(r);
}
void AnimateSprite::OnDrawAnimate()
{
	p_ListActionSprite.at(p_idxSprite)->visit();
}
void AnimateSprite::OnDrawAnimate(Renderer *renderer, const Mat4& transform, uint32_t flags)
{
	p_ListActionSprite.at(p_idxSprite)->draw(renderer,transform,flags);
}

void AnimateSprite::Update(float dt)
{

	struct timeval now;

	if (gettimeofday(&now, nullptr) != 0)
	{
		CCLOG("error in gettimeofday");
		return;
	}

	float cur_time = now.tv_usec / 10000000.0f;
	if (p_previousTIme == 0)
	{
		p_previousTIme = cur_time;
	}

	p_RunTime += (abs(cur_time - p_previousTIme));
	if (p_RunTime >= p_DeltaTime)
	{
		p_RunTime = 0;
		p_idxSprite++;
	}
	if (p_idxSprite >= p_ListActionSprite.size())
	{
		p_idxSprite = 0;
	}

	p_previousTIme = cur_time;
}
/////////////////////////////////////////////////////////////
//
//MainCharacterObject Class
//
/////////////////////////////////////////////////////////////

MainCharacterObject * MainCharacterObject::m_Instance = NULL;
MainCharacterObject::MainCharacterObject()
{
	InitValueMainChar();
	//
	p_IsOnReviving = false;
}
/////////////////////////////////////////////////////
//--------------------------------------------------
/////////////////////////////////////////////////////
void MainCharacterObject::InitValueMainChar()
{
	p_SpeedAroundRotate = OBJAPMgr->GetParamFromDataXML("speed_rotate_char").f;
	m_isStopMoveBezier = true;
	m_SpeedJumpDuration = 0.0f;
	m_SpeedJump = 0.0f;
	m_GravityPress = OBJAPMgr->GetParamFromDataXML("Gravity_press_char").f * GetScaleFactor();
	m_isStopJumpGravity = true;
	p_IsMoveToOrbitInit = true;
	p_IsMoveToOrbit = false;
	p_previousOrbitIdx = 0;
	DeltaPosArrowWithCharacter = Point(0, 0) * GetScaleFactor();
#if USE_JUMP_PARTICLE
	p_name_jump_particle = "char_comet";
#else
	p_Streak = NULL;
#endif
	p_RatioSizeDistance = 0.3f;
	p_CalcuRunDeltaTime = 0.0f;
	p_CalcuFlyDeltaTime = 0.0f;

}
/////////////////////////////////////////////////////
//--------------------------------------------------
/////////////////////////////////////////////////////
MainCharacterObject::~MainCharacterObject()
{
	FreeObject();
	CC_SAFE_RELEASE_NULL(p_ArrowPoint);
}
/////////////////////////////////////////////////////
//--------------------------------------------------
/////////////////////////////////////////////////////
void	MainCharacterObject::ReloadMainChar()
{
	InitValueMainChar();
	//
	SetPosMainChar(Vec2(0, 0));
	SetCurrentOrbitIdx(0);
	//
	auto p = ManageObjectAP::GetInstance()->GetOrbitAtIdx(0)->GetOriginPoint();
	MoveToPoint(p);
	//
	p_ArrowPoint->MoveToPoint(p);
	//
	DeltaPosArrowWithCharacter.y = (p_ArrowPoint->GetSizeObject().height / 2) + (GetSizeMainChar().width / 2) + (5.f * GetScaleFactor());
	p_ArrowPoint->SetPosObject(GetPosMainChar() + DeltaPosArrowWithCharacter);
	//
	SetRotation(0);

	Flip(false);
}


void	MainCharacterObject::LoadMainChar()
{
	p_IsUseAnimatEditor = true;
	Sprite * te = NULL;
	if (!p_IsUseAnimatEditor)
	{
#if USE_SPRITE_STORE
		te = TextureMgr->GetSpriteFromMemory("mainBody");
		LoadObjectFromSprite(te);
		te->retain();
		Vector<Sprite *> pList;
		for (int i = 0; i < 7; i++)
		{
			std::string str_sprite = "Run" + std::to_string(i);
			pList.pushBack(TextureMgr->GetSpriteFromMemory(str_sprite));
		}
		LoadAnimateFromSpriteList(pList);
		//LoadAnimationFromListSprite(pList);
		MainSceneGame::GetInstance()->addChild(GetObject());
#else
		Texture2D * te = TextureMgr->GetTexureFromMemory("mainBody");
		LoadObjectFromSingleTexture(te);
#endif
	}
	else
	{
		LoadAnimateFromEditor();
	}


	float scaleratio = OBJAPMgr->GetParamFromDataXML("Scale_ratio_char").f;
	SetSizeMainChar(GetSizeMainChar() / (scaleratio * (TextureMgr->IsUseMipMap() ? 2.0f : 1.0f)));
	SetCurrentOrbitIdx(0);
	//
	auto p = ManageObjectAP::GetInstance()->GetOrbitAtIdx(0)->GetOriginPoint();
	MoveToPoint(p);
	//
	p_ArrowPoint = new EntityObject();
#if USE_SPRITE_STORE
	te = TextureMgr->GetSpriteFromMemory("ArrowPoint");
	p_ArrowPoint->LoadObjectFromSprite(te);
#else
	Texture2D * te = TextureMgr->GetTexureFromMemory("ArrowPoint");
	p_ArrowPoint->LoadObjectFromSingleTexture(te);
#endif
	p_ArrowPoint->SetSizeObject(p_ArrowPoint->GetSizeObject() * 1.25f / scaleratio);
	p_ArrowPoint->MoveToPoint(p);
	//
	DeltaPosArrowWithCharacter.y = (p_ArrowPoint->GetSizeObject().height / 2) + (GetSizeMainChar().width / 2) + (5.f * GetScaleFactor());
	//
	p_ArrowPoint->SetPosObject(GetPosMainChar() + DeltaPosArrowWithCharacter);
	//
#if ! USE_JUMP_PARTICLE
	te = TextureMgr->GetSpriteFromMemory("streak");
	p_Streak = MotionStreak::create(2, 3, 32, Color3B::GREEN,te->getTexture());
	auto colorAction = RepeatForever::create(Sequence::create(
		TintTo::create(0.2f, 255, 0, 0),
		TintTo::create(0.2f, 0, 255, 0),
		TintTo::create(0.2f, 0, 0, 255),
		TintTo::create(0.2f, 0, 255, 255),
		TintTo::create(0.2f, 255, 255, 0),
		TintTo::create(0.2f, 255, 0, 255),
		TintTo::create(0.2f, 255, 255, 255),
		nullptr));
	colorAction->setTag(AC_COLOR_REPEAT);
	p_Streak->runAction(colorAction);
#endif
}
/////////////////////////////////////////////////////
//--------------------------------------------------
/////////////////////////////////////////////////////
void MainCharacterObject::LoadAnimateFromEditor()
{

	bool IsUseArk = false;
#if USE_ARK_ARCHIVE
	IsUseArk = true;
#endif
	//async loading export file
	ArmatureDataManager::getInstance()->addArmatureFileInfo(
		IsUseArk ? XMLMgr->GetMainCharData()->pathTextureAnimation: ResolveNamePath(XMLMgr->GetMainCharData()->pathTextureAnimation),
		IsUseArk ? XMLMgr->GetMainCharData()->pathPlistAnimation : ResolveNamePath(XMLMgr->GetMainCharData()->pathPlistAnimation),
		IsUseArk ? XMLMgr->GetMainCharData()->pathJsonAnimation  : ResolveNamePath(XMLMgr->GetMainCharData()->pathJsonAnimation), 
		IsUseArk,
		true);

	//Creat armature by name
	p_Armature = CCArmature::create("BunnyAnimation");

	//retain
	p_Armature->retain();

	//play animation by animation index
	p_Armature->getAnimation()->playByIndex(0);

	p_Armature->getAnimation()->setSpeedScale(2.0f);

}
/////////////////////////////////////////////////////
//--------------------------------------------------
/////////////////////////////////////////////////////
float	MainCharacterObject::GetRotation()
{
	return GetRotateObject();
}
void	MainCharacterObject::SetRotation(float r)
{
	SetRotateObject(r);
}
/////////////////////////////////////////////////////
//--------------------------------------------------
/////////////////////////////////////////////////////
void	MainCharacterObject::Update(float dt)
{
	p_CalcuRunDeltaTime += dt;
	p_CalcuFlyDeltaTime += dt;
#if ENABLE_EDITOR
	UpdateControlEdit();
#endif
	//get the info of the object orbit at
	auto orb = ManageObjectAP::GetInstance()->GetOrbitAtIdx(p_currentOrbitIdx);
	if (orb && m_isStopMoveBezier && m_isStopJumpGravity)
	{
		p_SpeedMove = orb->GetSpeedMoveOrbit(true);
		//////////move to the orgin-point of orbit
		/////
		float distance = sqrt((GetPosObject().x  - orb->GetPosOrbit().x) * (GetPosObject().x - orb->GetPosOrbit().x) +
			((GetPosObject().y - orb->GetPosOrbit().y) * (GetPosObject().y - orb->GetPosOrbit().y)));
		/////////self rotate//
		if (!p_IsMoveToOrbitInit)
		{
			CalculateSelfRotateObject(dt,orb, CONTROL_TYPE::MOVE_BACKWARD);
			CalculateRotateAroundObject(dt,orb, CONTROL_TYPE::MOVE_BACKWARD);
			//
			p_SpeedMove = orb->GetSpeedMoveOrbit(true);
			//
			if (APMgr->IsUseTheBoost(TYPE_BOOST::B_JUMP_BOOST))
			{
				p_SpeedAroundRotate = OBJAPMgr->GetParamFromDataXML("speed_rotate_char").f * INCREASE_BOOST;
			}
			else
			{
				p_SpeedAroundRotate = OBJAPMgr->GetParamFromDataXML("speed_rotate_char").f;
			}
			//
			p_ArrowPoint->CalculateSelfRotateObject(dt,orb, CONTROL_TYPE::MOVE_BACKWARD);
			p_ArrowPoint->CalculateRotateAroundObject(dt,orb, CONTROL_TYPE::MOVE_BACKWARD);
#if USE_CC_AUDIO
			if (p_CalcuRunDeltaTime > dt * 20)
			{
				GetSound->PlaySoundEffect(XMLMgr->GetPathSound("SFX_RUNNING"));
				p_CalcuRunDeltaTime = 0;
			}
#endif
		}
		else
		{
			p_SpeedMove = orb->GetSpeedMoveOrbit(true) * OBJAPMgr->GetParamFromDataXML("speed_move_char").f;
		}
		p_ArrowPoint->SpeedMove(p_SpeedMove);
		p_ArrowPoint->SpeedAroundRotate(p_SpeedAroundRotate);
		//
		//move bezier line
		if (InputMgr->IsTouchOnScreen())
		{
			//reset the streak to begin draw
#if USE_CC_AUDIO
			GetSound->PlaySoundEffect(XMLMgr->GetPathSound("SFX_BEGIN_JUMP"));
#endif
#if USE_JUMP_PARTICLE
			if (APMgr->IsUseTheBoost(TYPE_BOOST::B_JUMP_BOOST) || APMgr->IsOnStoryMode())
			{
				EffectMgr->AddParticle(PARTICLE_TYPE::P_CHAR_COMET, GetPosMainChar(), p_name_jump_particle, false);
			}
#else
			p_Streak->reset();
#endif
			//
			BeginJump(orb);
			p_previousOrbitIdx = p_currentOrbitIdx;
			InputMgr->SetIsTouchOnScreen();
			return;
		}
		//
		if (orb->IsStopMove())
		{
			DurationMove(0);
			p_ArrowPoint->DurationMove(0);
		}
		//
		if (orb->IsRotateAround())
		{
			float speed_r_orbit = orb->GetSpeedRotateAroundAPoint();
			CalculateSelfRotateAPoint(speed_r_orbit, speed_r_orbit > 0 ? CONTROL_TYPE::MOVE_FORWARD : CONTROL_TYPE::MOVE_BACKWARD);
			SetRotateAroundAPoint(orb->GetPointRotateAround(), speed_r_orbit);
			p_ArrowPoint->CalculateSelfRotateAPoint(speed_r_orbit, speed_r_orbit > 0 ? CONTROL_TYPE::MOVE_FORWARD : CONTROL_TYPE::MOVE_BACKWARD);
			p_ArrowPoint->SetRotateAroundAPoint(orb->GetPointRotateAround(), speed_r_orbit);
		}
		//
		if (p_IsMoveToOrbitInit)
		{
			if (distance > orb->GetRadius() + (GetSizeMainChar().height * p_RatioSizeDistance))
			{
				MoveCharacterToPoint(orb->GetOriginPoint(),true,orb);
				orb->IsTheCharacterOnTheOrbit(false);
			}
			else if (distance <= orb->GetRadius() + (GetSizeMainChar().height * p_RatioSizeDistance))
			{
				orb->IsTheCharacterOnTheOrbit(true);
				UpdateMoveCharacter(dt, orb, false);
				p_IsMoveToOrbitInit = false;
				p_IsOnReviving = false;
			}
		}
		else
		{
			float distance_delta = (orb->IsMoveWithDirect() || orb->IsRotateAround()) ? 
				orb->IsMoveWithDirect() ? orb->GetSpeedMoveOrbit() : orb->GetSpeedRotateAroundAPoint() 
				: 0;
			float distance = sqrt((GetPosObject().x - orb->GetPosOrbit().x) * (GetPosObject().x - orb->GetPosOrbit().x) +
				((GetPosObject().y - orb->GetPosOrbit().y) * (GetPosObject().y - orb->GetPosOrbit().y)));
			if (distance > orb->GetRadius() + (GetSizeMainChar().height * p_RatioSizeDistance) - distance_delta 
				&& distance <= orb->EffectRaius() + (GetSizeMainChar().height * p_RatioSizeDistance) + distance_delta)
			{
					MoveCharacterToPoint(orb->GetOriginPoint(), true,orb);
					orb->IsTheCharacterOnTheOrbit(false);
			}
			else if (distance <= orb->GetRadius() + (GetSizeMainChar().height * p_RatioSizeDistance) - distance_delta)
			{
				if (previousDistance > orb->EffectRaius() + (GetSizeMainChar().height * p_RatioSizeDistance))
				{
					orb->IsTheCharacterOnTheOrbit(false);
					p_IsMoveToOrbit = true;
				}
				else if (previousDistance <= orb->EffectRaius() + (GetSizeMainChar().height * p_RatioSizeDistance))
				{
					orb->IsTheCharacterOnTheOrbit(true);
					UpdateMoveCharacter(dt, orb, false);
				}
				p_IsMoveToOrbitInit = false;
			}
			else
			{
				orb->IsTheCharacterOnTheOrbit(false);
				p_currentOrbitIdx = -1;
			}
			//if (p_previousOrbitIdx != p_currentOrbitIdx)
			//{
			//	if (p_currentOrbitIdx > p_previousOrbitIdx && or->IsTheCharacterOnTheOrbit())
			//	{
			//		int delta = p_currentOrbitIdx - p_previousOrbitIdx;
			//		int score = ManageObjectAP::GetInstance()->GetScore();
			//		score += delta;
			//		ManageObjectAP::GetInstance()->SetScore(score);
			//		p_previousOrbitIdx = p_currentOrbitIdx;
			//	}
			//}

		}

		UpdateMoveCharacter(dt,orb);

		previousDistance = distance;
#if ENABLE_EDITOR
		SeftRotate(InputMgr->GetCurrentControlType());
#endif
	}
	else if (orb)
	{
		float distance = sqrt((GetPosObject().x - orb->GetPosOrbit().x) * (GetPosObject().x - orb->GetPosOrbit().x) +
			(GetPosObject().y - orb->GetPosOrbit().y) * (GetPosObject().y - orb->GetPosOrbit().y));
		if (!p_IsMoveToOrbitInit && distance > orb->EffectRaius() + (GetSizeMainChar().height * p_RatioSizeDistance))
		{
			orb->IsTheCharacterOnTheOrbit(false);
			p_currentOrbitIdx = -1;
		}

		previousDistance = distance;
	}
	// calculate update of bezier line
	if (m_ListBezierMove.size() > 1)
	{
		MoveCharacterToPoint(m_ListBezierMove.at(1));
		UpdateMoveCharacter(dt);
		//rotate the char as the path
		Point direct_move = Point(m_ListBezierMove.at(1) - GetPosMainChar());
		float angleOrigin = AngleBy2Vector(Point(0, 1), direct_move);
		float finalAngle = angleOrigin - 90.f;
		SetRotation(finalAngle);
		//
		if (IsStopMove())
		{
			m_ListBezierMove.erase(m_ListBezierMove.begin());
		}
	}
	else
	{
		m_isStopMoveBezier = true;
		m_ListBezierMove.clear();
	}
	//calculate jump gravity
	if (!m_isStopJumpGravity)
	{
		UpdateJumpGravity(dt);
	}
	else
	{

		m_SpeedJumpDuration = 0;
		m_GravityPress = OBJAPMgr->GetParamFromDataXML("Gravity_press_char").f * GetScaleFactor();
		if (!orb) // prevent the main char stay at the sky
		{
			m_SpeedJump = (OBJAPMgr->GetParamFromDataXML("speed_jump_char").f + GetOffsetSCale() * 3) * GetScaleFactor();
			if (APMgr->IsUseTheBoost(TYPE_BOOST::B_JUMP_BOOST))
			{
				m_SpeedJump *= INCREASE_BOOST;
			}
			m_isStopJumpGravity = false;
		}
		else
		{
			m_SpeedJump = 0.0f;
		}
	}
	//update the animation of sprite
	if (p_IsUseAnimatEditor && m_isStopJumpGravity)
	{
		GetObject()->update(dt); //update animation
	}
	else if (!p_IsUseAnimatEditor)
	{
		if (p_Animate)
		{
			GetObject()->getActionByTag(ACTION_TAG::AC_ANIMATE)->step(dt);
		}
	}
	//
	if (APMgr->IsUseTheBoost(TYPE_BOOST::B_JUMP_BOOST) || APMgr->IsOnStoryMode())
	{
#if USE_JUMP_PARTICLE
		auto effect = EffectMgr->GetParticle(p_name_jump_particle);
		if (effect)
		{
			effect->SetPosition(GetPosMainChar());
			effect->SetAngle(GetRotation());
			effect->UpdateParticle(dt);
		}
#else
		p_Streak->setPosition(GetPosMainChar());
		p_Streak->update(dt);
		//update the action 
		p_Streak->getActionByTag(AC_COLOR_REPEAT)->step(dt);
#endif
	}
#if ENABLE_EDITOR
	UpdateMoveCharacter(dt,NULL, false);
#endif
}
/////////////////////////////////////////////////////
//--------------------------------------------------
/////////////////////////////////////////////////////
//use the normal draw only
void	MainCharacterObject::onDrawMainChar(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch )
{
	onDrawObject(renderer, transform, flags, spriteBatch);
	if (APMgr->IsUseTheBoost(TYPE_BOOST::B_JUMP_BOOST) || APMgr->IsOnStoryMode())
	{
		if (p_currentOrbitIdx > -1 && m_isStopJumpGravity && m_isStopMoveBezier && IsAlive() && !p_IsMoveToOrbitInit)
		{
			p_ArrowPoint->onDrawObject(renderer, transform, flags, spriteBatch);
		}

		if (!m_isStopJumpGravity)
		{
#if ! USE_JUMP_PARTICLE
			p_Streak->draw(renderer, transform, flags);
#endif
		}
#if USE_JUMP_PARTICLE
		auto effect = EffectMgr->GetParticle(p_name_jump_particle);
		if (effect)
		{
			effect->DrawParticle(renderer, transform, flags);
		}
#endif
	}


}
/////////////////////////////////////////////////////
//--------------------------------------------------
/////////////////////////////////////////////////////
void	MainCharacterObject::onVisitMainChar(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch)
{
	onVisitObject(renderer, transform, flags, spriteBatch);
	if (APMgr->IsUseTheBoost(TYPE_BOOST::B_JUMP_BOOST) || APMgr->IsOnStoryMode())
	{
		if (p_currentOrbitIdx > -1 && m_isStopJumpGravity && m_isStopMoveBezier&& IsAlive() && !p_IsMoveToOrbitInit)
		{
			p_ArrowPoint->onVisitObject(renderer, transform, flags, spriteBatch);
		}

		if (!m_isStopJumpGravity)
		{
#if ! USE_JUMP_PARTICLE
			p_Streak->visit();
#endif
		}
#if USE_JUMP_PARTICLE
		auto effect = EffectMgr->GetParticle(p_name_jump_particle);
		if (effect)
		{
			effect->DrawParticle();
		}
#endif
	}
}
/////////////////////////////////////////////////////
//--------------------------------------------------
/////////////////////////////////////////////////////
void MainCharacterObject::MoveBezierCurve(Point directVector, float distance)
{
	static int bezierLevel = 5;
	Point m_InitialPoint[4];
	m_InitialPoint[0] = GetPosMainChar();
	m_InitialPoint[3] = GetPosMainChar() + (directVector*distance);
	//the 1 point have x less half of 3.x and higher than 0.x , and y have higher than 0.y
	m_InitialPoint[1] = Point(GetRandom((int)(m_InitialPoint[3].x / 2), (int)(m_InitialPoint[0].x)), GetRandom((int)(m_InitialPoint[0].y + 10), (int)m_InitialPoint[0].y));
	//the 2 point have x higher  than half of 3.x and less than 3.x , and y have higher than 0.y
	m_InitialPoint[2] = Point(GetRandom((int)(m_InitialPoint[3].x), (int)(m_InitialPoint[3].x / 2)), GetRandom((int)(m_InitialPoint[0].y + 10),(int)m_InitialPoint[0].y));
	//
	std::vector<Point> ListMovePoint;
	for (int i = 0; i < bezierLevel; i++)
	{
		std::vector<Point> t_ListMovePoint;
		if (i == 0)
		{
			for (int j = 0; j < 4; j++)
			{
				t_ListMovePoint.push_back(m_InitialPoint[j]);
			}
		}
		else
		{
			for (size_t j = 0; j <= ListMovePoint.size(); j++)
			{
				if (j == 0)
				{
					t_ListMovePoint.push_back(ListMovePoint.at(0));
				}
				else if (j == ListMovePoint.size())
				{
					t_ListMovePoint.push_back(ListMovePoint.at(ListMovePoint.size() - 1));
				}
				else
				{
					t_ListMovePoint.push_back(GetHalfPoint(ListMovePoint.at(j - 1), ListMovePoint.at(j)));
				}
			}
		}
		ListMovePoint.clear();
		ListMovePoint = t_ListMovePoint;
	}
	m_isStopMoveBezier = false;
	p_IsStopMove = false;
	m_ListBezierMove = ListMovePoint;
}
/////////////////////////////////////////////////////
//--------------------------------------------------
/////////////////////////////////////////////////////
void MainCharacterObject::BeginMoveBezier(OrbitObject * orb)
{
	Point vectorDirect = Point((GetPosMainChar().y - orb->GetPosOrbit().y), -(GetPosMainChar().x - orb->GetPosOrbit().x));
	vectorDirect = NormalizeVector(vectorDirect);
	MoveBezierCurve(vectorDirect, GetWinSize().width - GetPosMainChar().x);
}
/////////////////////////////////////////////////////
//--------------------------------------------------
/////////////////////////////////////////////////////
void MainCharacterObject::UpdateJumpGravity(float dt)
{
	//m_SpeedJumpDuration -= 0.5f;
	//if (m_SpeedJumpDuration <= 0)
	//{
	//	return;
	//}
	m_isStopJumpGravity = false;
	//Pre pos;
	Point prePos = GetPosObject();
	//move as straight line
	Point cur_pos = GetPosObject();
	Point p = NormalizeVector(m_directToJump);
	SetPosMainChar(cur_pos + p *SyncWithFPS( dt * 100.f * m_SpeedJump));
	//move as stringt line with arrow point
	p_ArrowPoint->SetPosObject(p_ArrowPoint->GetPosObject() + p *  SyncWithFPS(dt * 100.f * m_SpeedJump));
	//under the gravity pressure
	cur_pos = GetPosObject();
	p = Point(0, -1);
	SetPosMainChar(cur_pos + p *  SyncWithFPS(dt * 100.f * m_GravityPress));
	//arrowpoint
	p_ArrowPoint->SetPosObject(p_ArrowPoint->GetPosObject() + p * SyncWithFPS( dt * 100.f * m_GravityPress));
	//rotate the char as the path
	Point direct_move = Point(GetPosMainChar() - prePos);
	float angleOrigin = AngleBy2Vector(Point(0, 1), direct_move);

	if (GetPosMainChar().x < prePos.x)
	{
		angleOrigin = 180 - angleOrigin;
	}
	float finalAngle = angleOrigin - 90.f;
	//rotate main character
	SetRotation(finalAngle);
	//rotate arrow point
	p_ArrowPoint->SetRotateObject(finalAngle);
	if (m_GravityPress == -OBJAPMgr->GetParamFromDataXML("Gravity_press_char").f * GetScaleFactor())
	{
		//apply new pos
		p_ArrowPoint->SetRotateAroundAPoint(GetPosMainChar(), angleOrigin);
	}
	//
	m_GravityPress += SyncWithFPS(dt * 100.f * (OBJAPMgr->GetParamFromDataXML("gravity_press_delta_char").f) * GetScaleFactor());
}
/////////////////////////////////////////////////////
//--------------------------------------------------
/////////////////////////////////////////////////////
void MainCharacterObject::BeginJump(OrbitObject * orb)
{
	m_SpeedJumpDuration = OBJAPMgr->GetParamFromDataXML("jump_duration_char").f;

	m_SpeedJump = (OBJAPMgr->GetParamFromDataXML("speed_jump_char").f + GetOffsetSCale() * 3) * GetScaleFactor();
	if (APMgr->IsUseTheBoost(TYPE_BOOST::B_JUMP_BOOST))
	{
		m_SpeedJump *= INCREASE_BOOST;
	}
	m_GravityPress = -m_GravityPress;
	m_directToJump = GetPosMainChar() - orb->GetOriginPoint();
	if (m_directToJump.x >= 0)
	{
		Flip(false);
	}
	else
	{
		Flip(true);
	}
	m_isStopJumpGravity = false;
}
/////////////////////////////////////////////////////
//--------------------------------------------------
/////////////////////////////////////////////////////
void MainCharacterObject::MoveCharacterToPoint(Point p, bool IsOnMoveToOrbit, EntityObject * orb)
{
	//if (!or)
	{
		MoveToPoint(p);
		//
		Point p_delta = p + DeltaPosArrowWithCharacter;
		p_ArrowPoint->MoveToPoint(p_delta);
	}
	/*else
	{
		Point p_toMove = p + (GetPosObject() - or->GetPosObject());
		MoveToPoint(p_toMove);
		p_toMove = p + (p_ArrowPoint->GetPosObject() - or->GetPosObject());
		p_ArrowPoint->MoveToPoint(p_toMove);
	}*/

	//
	p_IsMoveToOrbit = IsOnMoveToOrbit;
}
/////////////////////////////////////////////////////
//--------------------------------------------------
/////////////////////////////////////////////////////
void MainCharacterObject::UpdateMoveCharacter(float dt, OrbitObject * orb, bool UpdatePosition)
{
	updateObject(dt, UpdatePosition);
	p_ArrowPoint->updateObject(dt, UpdatePosition);
	if ((IsStopMove() || orb->IsTheCharacterOnTheOrbit() == true) && p_IsMoveToOrbit)
	{
		if (orb)
		{
 			Point vectorWithOrginPoint = GetPosMainChar() - orb->GetPosOrbit();
			Point OriginVector(0, 1);
			float angle = AngleBy2Vector(OriginVector, vectorWithOrginPoint);
			if (GetPosMainChar().x < orb->GetPosOrbit().x)
			{
				angle = 360 - angle;
			}
			SetRotation(angle);
			p_ArrowPoint->SetRotateObject(angle);
			//p_ArrowPoint->SetRotateAroundAPoint(GetPosMainChar(),360.f - angle);
			//calculate the pos of the arrow point
			Point normalVector = vectorWithOrginPoint / (sqrt((vectorWithOrginPoint.x * vectorWithOrginPoint.x) + (vectorWithOrginPoint.y * vectorWithOrginPoint.y)));
			/*
			Point(x-GetPosMainChar().x,y-GetPosMainChar().y) = normalVector * sqrt(DeltaPosArrowWithCharacter.x*DeltaPosArrowWithCharacter.x + DeltaPosArrowWithCharacter.y * DeltaPosArrowWithCharacter.y);			
			*/
			Point PosArrowPoint(0, 0);
			PosArrowPoint.x = GetPosMainChar().x + normalVector.x * sqrt((DeltaPosArrowWithCharacter.x*DeltaPosArrowWithCharacter.x) + (DeltaPosArrowWithCharacter.y * DeltaPosArrowWithCharacter.y));
			PosArrowPoint.y = GetPosMainChar().y + normalVector.y * sqrt((DeltaPosArrowWithCharacter.x*DeltaPosArrowWithCharacter.x) + (DeltaPosArrowWithCharacter.y * DeltaPosArrowWithCharacter.y));
			//
			p_ArrowPoint->SetPosObject(PosArrowPoint);
		}
		p_IsMoveToOrbit = false;
	}
}
/////////////////////////////////////////////////////
//--------------------------------------------------
/////////////////////////////////////////////////////
void MainCharacterObject::OnReviveChar()
{
	m_isStopJumpGravity = true;
	p_IsMoveToOrbitInit = true;
	p_IsMoveToOrbit = false;
	p_IsOnReviving = true;
	if (p_currentOrbitIdx <= -1)
	{
		p_currentOrbitIdx = ManageObjectAP::GetInstance()->GetIdxOrbitOnScreen();
	}
	//
	if (BoostMenu::GetInstance()->IsCanUse("Item_life", false))
	{
		BoostMenu::GetInstance()->ProcessUse("Item_life");
		APMgr->SetUseTheBoost(TYPE_BOOST::B_LIFE_BOOST, true);
	}
	else
	{
		APMgr->SetUseTheBoost(TYPE_BOOST::B_LIFE_BOOST, false);
	}
}