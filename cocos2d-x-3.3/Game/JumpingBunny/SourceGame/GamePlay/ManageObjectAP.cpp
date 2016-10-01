#include "ManageObjectAP.h"
#include "Utility/ObjectTexture.h"
#include "Menu/ResultMenu.h"
#include "Menu/HUDMenu.h"
#include "GamePlay/APManager.h"
#if USE_CC_AUDIO
#include "Utility/SoundManager.h"
#endif
#include "Particle/ParicleManager.h"
ManageObjectAP * ManageObjectAP::p_Instance = NULL;

ManageObjectAP::ManageObjectAP()
{
	p_Score = 0;
	p_distance_quick_move = 0.0f;
	p_time_move = 0.0f;
	p_delta_time_move = 0.0f;
	p_delta_distance_quick_move = 0.0f;
	p_name_explosive_effect = "eff_orbit_explosive";
	//
	p_time_death_cd = XMLMgr->GetMainCharData()->d_time_deadth;
	p_delta_time_death_cd = XMLMgr->GetMainCharData()->d_delta_time_death;
	p_origin_time_death_cd = p_time_death_cd;

	p_stateLoad = 0;
	IsInitMainChar = true;

	p_OriginTimeCoutDown = p_CurrentTimeCoutDown = LIMIT_TIME_RUN;

	//
	p_CurrentStarGain = 0;

	p_Flag = NULL;
	p_IsPassLevel = false;
}

ManageObjectAP::~ManageObjectAP()
{
	FreeObjectInAP();
}

void		ManageObjectAP::ResetAll()
{

}

int		ManageObjectAP::InitTheObjectGame(TYPE_GAMEPLAY curGameplay, bool IsInit)
{

	//in all case reset the star gain
	p_CurrentStarGain = 0;
	//
	p_IsPassLevel = false;
	if (IsInit)
	{
		if (p_stateLoad == 0)
		{
			if (APMgr->GetCurrentGamePlay() == TYPE_GAMEPLAY::GAMEPLAY_ENDLESS_RUN || APMgr->GetCurrentGamePlay() == TYPE_GAMEPLAY::GAMEPLAY_TIME_RUN)
			{
				for (int i = 0; i < XMLMgr->GetOrbitData()->maxOrbitPframe; i++)
				{
					p_ListOrbits.pushBack(CreateOrbit(TYPE_ORBIT::ORBIT_DEFAULT, false));
				}
			}
			else
			{
				std::string nameGP = APMgr->convertToPartName(APMgr->GetCurrentPartPlay());
				auto cur_gameplay = XMLMgr->GetGamePlayMgr()->GetGamePlay(nameGP);
				for (int i = 0; i < cur_gameplay->numberOrbit; i++)
				{
					TYPE_ORBIT typ_orbit = TYPE_ORBIT::ORBIT_DEFAULT;
					auto objdetail = cur_gameplay->GetObjectDetail(std::string("orbit_") + std::to_string(i));
					typ_orbit = (TYPE_ORBIT)objdetail->orbit_type;
					p_ListOrbits.pushBack(CreateOrbitWithPara(typ_orbit, objdetail));
				}
				//
				p_CurrentTimeCoutDown = cur_gameplay->GetTimeCoutDown();
			}
			p_stateLoad++;
			return p_stateLoad;
		}

		//
		if (p_stateLoad == 1)
		{
			if (APMgr->GetCurrentGamePlay() == TYPE_GAMEPLAY::GAMEPLAY_ENDLESS_RUN || APMgr->GetCurrentGamePlay() == TYPE_GAMEPLAY::GAMEPLAY_TIME_RUN)
			{
				for (int i = 0; i < p_ListOrbits.size(); i++)
				{
					if (i == 0)
					{
						CalculateThePosOrbit(NULL, p_ListOrbits.at(i));
					}
					else
					{
						CalculateThePosOrbit(p_ListOrbits.at(i - 1), p_ListOrbits.at(i));
					}
				}
				//check the first value
				int * stateCreate = new int[3];
				stateCreate[0] = 1; stateCreate[1] = 0; stateCreate[2] = 1;
				p_ListOrbits.front()->GenerateTypeObject(stateCreate);

				delete stateCreate;
				stateCreate = NULL;

				for (int i = (int)p_ListOrbits.size() - 7; i < p_ListOrbits.size(); i++)
				{
					p_ListOrbits.at(i)->GenerateTypeObject();
				}
			}
			else
			{
				std::string nameGP = APMgr->convertToPartName(APMgr->GetCurrentPartPlay());
				auto cur_gameplay = XMLMgr->GetGamePlayMgr()->GetGamePlay(nameGP);
				for (int i = 0; i < NUMBER_STAR; i++)
				{

					auto star = new EntityObject();
					auto tx = TextureMgr->GetSpriteFromMemory("Big_star");
					auto objdetail = cur_gameplay->GetObjectDetail(std::string("star_") + std::to_string(i + 1));
					
					star->LoadObjectFromSprite(tx);
					Point pos(objdetail->pos.x * GetWinSize().width, objdetail->pos.y * GetWinSize().height);
					star->SetPosObject(pos);
					//
					Size SizeDisplay(tx->getContentSize() / objdetail->ratio_scale);
					if (TextureMgr->IsUseMipMap())
					{
						SizeDisplay = SizeDisplay / 2.f;
						SizeDisplay = SizeDisplay * (GetWinSize().width / 1280.f);
					}
					star->SetSizeObject(SizeDisplay, false);
					//
					p_animateStar[i].SpeedScale = objdetail->delta_scale;
					p_animateStar[i].CurrentScale = Vec2(star->GetSizeObject().width / tx->getContentSize().width, star->GetSizeObject().height / tx->getContentSize().height);
					p_animateStar[i].OriginScale = p_animateStar[i].CurrentScale;
					p_animateStar[i].MaxScale = objdetail->max_scale * p_animateStar[i].OriginScale;
					p_animateStar[i].IsStarExist = true;
					//
					p_ListStar.pushBack(star);
				}
			}
			p_stateLoad++;
			return p_stateLoad;
		}
		//
		if (p_stateLoad == 2)
		{
			if (APMgr->GetCurrentGamePlay() == TYPE_GAMEPLAY::GAMEPLAY_ENDLESS_RUN || APMgr->GetCurrentGamePlay() == TYPE_GAMEPLAY::GAMEPLAY_TIME_RUN)
			{
				//GetChar->LoadMainChar();
				//
				SetScore(0);
				//
				if (APMgr->GetCurrentGamePlay() == TYPE_GAMEPLAY::GAMEPLAY_TIME_RUN)
				{
					p_OriginTimeCoutDown = p_CurrentTimeCoutDown = LIMIT_TIME_RUN;
				}
			}
			else
			{
				std::string nameGP = APMgr->convertToPartName(APMgr->GetCurrentPartPlay());
				auto cur_gameplay = XMLMgr->GetGamePlayMgr()->GetGamePlay(nameGP);
				p_Flag = new EntityObject();
				auto tx = TextureMgr->GetSpriteFromMemory("flag");
				auto objdetail = cur_gameplay->GetObjectDetail(std::string("flag_1"));

				p_Flag->LoadObjectFromSprite(tx);
				Point pos(objdetail->pos.x * GetWinSize().width, objdetail->pos.y * GetWinSize().height);
				p_Flag->SetPosObject(pos);
				//
				Size SizeDisplay(tx->getContentSize() / objdetail->ratio_scale);
				if (TextureMgr->IsUseMipMap())
				{
					SizeDisplay = SizeDisplay / 2.f;
					SizeDisplay = SizeDisplay * (GetWinSize().width / 1280.f);
				}
				p_Flag->SetSizeObject(SizeDisplay, false);
			}

			p_stateLoad++;
			return p_stateLoad;
		}
		//
	}
	else
	{
		if (APMgr->GetCurrentGamePlay() == TYPE_GAMEPLAY::GAMEPLAY_ENDLESS_RUN || APMgr->GetCurrentGamePlay() == TYPE_GAMEPLAY::GAMEPLAY_TIME_RUN)
		{
			for (int i = 0; i < XMLMgr->GetOrbitData()->maxOrbitPframe; i++)
			{
				p_ListOrbits.pushBack(CreateOrbit(TYPE_ORBIT::ORBIT_DEFAULT, false));
			}

			//
			for (int i = 0; i < p_ListOrbits.size(); i++)
			{
				if (i == 0)
				{
					CalculateThePosOrbit(NULL, p_ListOrbits.at(i));
				}
				else
				{
					CalculateThePosOrbit(p_ListOrbits.at(i - 1), p_ListOrbits.at(i));
				}
				p_ListOrbits.at(i)->GenerateTypeObject();
			}
		}
		else
		{
			std::string nameGP = APMgr->convertToPartName(APMgr->GetCurrentPartPlay());
			auto cur_gameplay = XMLMgr->GetGamePlayMgr()->GetGamePlay(nameGP);
			for (int i = 0; i < cur_gameplay->numberOrbit; i++)
			{
				TYPE_ORBIT typ_orbit = TYPE_ORBIT::ORBIT_DEFAULT;
				auto objdetail = cur_gameplay->GetObjectDetail(std::string("orbit_") + std::to_string(i));	
				typ_orbit = (TYPE_ORBIT)objdetail->orbit_type;
				p_ListOrbits.pushBack(CreateOrbitWithPara(typ_orbit, objdetail));
			}
			p_CurrentTimeCoutDown = cur_gameplay->GetTimeCoutDown();
			//
			for (int i = 0; i < NUMBER_STAR; i++)
			{
				auto star = new EntityObject();
				auto tx = TextureMgr->GetSpriteFromMemory("Big_star");
				auto objdetail = cur_gameplay->GetObjectDetail(std::string("star_") + std::to_string(i + 1));

				star->LoadObjectFromSprite(tx);
				Point pos(objdetail->pos.x * GetWinSize().width, objdetail->pos.y * GetWinSize().height);
				star->SetPosObject(pos);
				//
				Size SizeDisplay(tx->getContentSize() / objdetail->ratio_scale);
				if (TextureMgr->IsUseMipMap())
				{
					SizeDisplay = SizeDisplay / 2.f;
					SizeDisplay = SizeDisplay * (GetWinSize().width / 1280.f);
				}
				star->SetSizeObject(SizeDisplay, false);
				//
				p_animateStar[i].SpeedScale = objdetail->delta_scale;
				p_animateStar[i].CurrentScale = Vec2(star->GetSizeObject().width / tx->getContentSize().width, star->GetSizeObject().height / tx->getContentSize().height);
				p_animateStar[i].OriginScale = p_animateStar[i].CurrentScale;
				p_animateStar[i].MaxScale = objdetail->max_scale * p_animateStar[i].OriginScale;
				p_animateStar[i].IsStarExist = true;
				//
				p_ListStar.pushBack(star);
			}
			//
			p_Flag = new EntityObject();
			auto tx = TextureMgr->GetSpriteFromMemory("flag");
			auto objdetail = cur_gameplay->GetObjectDetail(std::string("flag_1"));

			p_Flag->LoadObjectFromSprite(tx);
			Point pos(objdetail->pos.x * GetWinSize().width, objdetail->pos.y * GetWinSize().height);
			p_Flag->SetPosObject(pos);
			//
			Size SizeDisplay(tx->getContentSize() / objdetail->ratio_scale);
			if (TextureMgr->IsUseMipMap())
			{
				SizeDisplay = SizeDisplay / 2.f;
				SizeDisplay = SizeDisplay * (GetWinSize().width / 1280.f);
			}
			p_Flag->SetSizeObject(SizeDisplay, false);
		}
		//
		GetChar->ReloadMainChar();
		//
		SetScore(0);
		//
		if (APMgr->GetCurrentGamePlay() == TYPE_GAMEPLAY::GAMEPLAY_TIME_RUN)
		{
			p_OriginTimeCoutDown = p_CurrentTimeCoutDown = LIMIT_TIME_RUN;
		}

	}

	return -1;
}

void		ManageObjectAP::CalculateThePosOrbit(OrbitObject * previousOr , OrbitObject* t)
{
	Size winSize = GetWinSize();
	int maxHReach = winSize.height * 5 / 6;
	int minHReach = winSize.height * 1 / 6;
#ifdef OS_IOS
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-variable"
#endif
	int maxWReach = winSize.width * 5 / 6;
	int minWReach = winSize.width * 3 / 6;
#ifdef OS_IOS
#pragma clang diagnostic pop
#endif
	int rY = GetRandom( maxHReach, minHReach);
	int rX = winSize.width / 2.f;//(rand() % maxWReach) + minWReach;

	if (previousOr)
	{
		rX = previousOr->GetPosOrbit().x + (previousOr->GetSizeOrbit().width / 2) + 
			XMLMgr->GetOrbitData()->distancePOrbit + (t->GetSizeOrbit().width / 2);
	}
	else
	{
		rY = winSize.height / 2.f;
	}

	t->SetPosOrbit(Point(rX, rY));
}

void		ManageObjectAP::UpdateListOrbit(float dt)
{
	for (int i = 0; i < p_ListOrbits.size(); i++)
	{
		p_ListOrbits.at(i)->Update(dt);
		//check the orbit time boom has explosed
		if (p_ListOrbits.at(i)->IsOrbitTimeOnExplosion())
		{
			//is character on the orbit
			bool CharDead = (GetChar->GetCurrentOrbitIdx() == i);
			//explose it
			setExplosiveAtOrbit(i, CharDead);
		}
		//check orbit is not alive
		if (!p_ListOrbits.at(0)->IsAlive())
		{
			//
			TYPE_ORBIT typ_orbit = TYPE_ORBIT::ORBIT_DEFAULT;
			typ_orbit = (GetRandom(0, 10) <= 3) ? ORBIT_BOOM : ORBIT_DEFAULT;

			//make sure there are no 2 dead orbit continous
			if (p_ListOrbits.at(p_ListOrbits.size() - 1)->TypeOrbit() == ORBIT_BOOM)
			{
				typ_orbit = TYPE_ORBIT::ORBIT_DEFAULT;
			}
			//
			p_ListOrbits.pushBack(CreateOrbit(typ_orbit));
			p_ListOrbits.at(0)->FreeOrbit();
			p_ListOrbits.erase(0);
			GetChar->SetCurrentOrbitIdx(GetChar->GetCurrentOrbitIdx() - 1);
			GetChar->SetPreviousOrbitIdx();
			break;
		}
	}
}

void		ManageObjectAP::Update(float dt)
{
	//bool IsHaveErase = false;

	if (IsInitMainChar)
	{
		GetChar->LoadMainChar();
		IsInitMainChar = false;
	}

	if (p_time_move <= 0.0f)
	{
		UpdateListOrbit(dt);
		GetChar->Update(dt);
		p_time_move = 0.0f;
		p_distance_quick_move = 0.0f;
		p_delta_time_move = 0.0f;
		p_delta_distance_quick_move = 0.0f;
	}
	else
	{
		//move all orbit
		for (int i = 0; i < p_ListOrbits.size(); i++)
		{
			p_ListOrbits.at(i)->SetPosOrbitWithDistance(SyncWithFPS(dt * 100.f *p_delta_distance_quick_move));
			if (i == GetChar->GetCurrentOrbitIdx())
			{
				//main char
				Point cur_pos_or = GetChar->GetPosMainChar();
				cur_pos_or.x -= SyncWithFPS(dt * 100.f * p_delta_distance_quick_move);
				GetChar->SetPosMainChar(cur_pos_or);
				// point arrow
				cur_pos_or = GetChar->GetArrowPoint()->GetPosObject();
				cur_pos_or.x -= SyncWithFPS(dt * 100.f * p_delta_distance_quick_move);
				GetChar->GetArrowPoint()->SetPosObject(cur_pos_or);
			}
		}
		//
		p_time_move -= SyncWithFPS(dt * 100.f * p_delta_time_move);
	}

	CheckCollitionCharWithOrbit();
	//
	if (APMgr->IsOnStoryMode())
	{
		//update effect zoom
		for (int i = 0; i < p_ListStar.size(); i++)
		{
			if (!p_animateStar[i].IsStarExist)
			{
				continue;
			}
			AnimateStar as = p_animateStar[i];
			as.CurrentScale += (Vec2(as.SpeedScale, as.SpeedScale) * as.direct_zoom);
			if (as.CurrentScale.x >= as.MaxScale.x)
			{
				as.CurrentScale = as.MaxScale;
				as.direct_zoom *= -1;
			}
			else if (as.CurrentScale.x <= as.OriginScale.x)
			{
				as.CurrentScale = as.OriginScale;
				as.direct_zoom *= -1;
			}
			p_animateStar[i] = as;
			Size s = Size(p_ListStar.at(i)->GetContentSize().x * as.CurrentScale.x, p_ListStar.at(i)->GetContentSize().y * as.CurrentScale.y);
			if (TextureMgr->IsUseMipMap())
			{
				s = s * (GetWinSize().width / 1280.f);
			}
			p_ListStar.at(i)->SetSizeObject(s);
		}
		//
		CheckCollitionCharWithStar();
	}
	//
	OnCheckStatusCharWithGamePlay(APMgr->GetCurrentGamePlay(), dt);
	//
	auto effect = EffectMgr->GetParticle(p_name_explosive_effect);
	if (effect)
	{
		effect->UpdateParticle(dt);
	}
}


void		ManageObjectAP::OnCheckStatusCharWithGamePlay(TYPE_GAMEPLAY tp, float dt)
{
	if (tp == TYPE_GAMEPLAY::GAMEPLAY_ENDLESS_RUN)
	{
		if (!GetChar->IsAlive())
		{
			//check the boost life
			if (HUDMenu::GetInstance()->GetCurrentStatus() == STATUS_MENU::M_IDLE)
			{
				if (APMgr->IsUseTheBoost(TYPE_BOOST::B_LIFE_BOOST))
				{
					GetChar->OnReviveChar();
				}
				else
				{
					//calculate time death
					//check the result end game
					int score = ManageObjectAP::GetInstance()->GetScore();
					int HScore = MenuMgr->GetHScore();
					TYPE_END_RESULT ty = TYPE_END_RESULT::RESULT_DIE;
					if (score > HScore)
					{
						ty = TYPE_END_RESULT::RESULT_PASS_H_SCORE;
					}
					CalCulateTimeDeath(ty);
				}
			}
		}
	}
	else if (tp == TYPE_GAMEPLAY::GAMEPLAY_PLAY_STORY || tp == TYPE_GAMEPLAY::GAMEPLAY_TIME_RUN)
	{
		p_CurrentTimeCoutDown -= dt;
		if (p_CurrentTimeCoutDown <= 0)
		{
			p_CurrentTimeCoutDown = 0;
		}
		//
		if (p_CurrentTimeCoutDown <= 0 || !GetChar->IsAlive())
		{
			if (HUDMenu::GetInstance()->GetCurrentStatus() == STATUS_MENU::M_IDLE)
			{
				bool IsShowPTimeDeath = true;
				if (p_CurrentTimeCoutDown > 0)
				{
					if (APMgr->IsUseTheBoost(TYPE_BOOST::B_LIFE_BOOST))
					{
						GetChar->OnReviveChar();
						IsShowPTimeDeath = false;
					}
				}
				else
				{
					IsShowPTimeDeath = true;
				}
				//
				if (IsShowPTimeDeath)
				{
					//check the result end game
					int score = ManageObjectAP::GetInstance()->GetScore();
					int HScore = MenuMgr->GetHScore();
					TYPE_END_RESULT ty = TYPE_END_RESULT::RESULT_END_TIME;
					if (score > HScore)
					{
						ty = TYPE_END_RESULT::RESULT_PASS_H_SCORE;
					}
					else
					{
						if (p_CurrentTimeCoutDown > 0 )
						{ 
							ty = TYPE_END_RESULT::RESULT_DIE;
						}
					}
					CalCulateTimeDeath(ty);
				}
			}
		}
		else
		{
			if (tp == TYPE_GAMEPLAY::GAMEPLAY_PLAY_STORY)
			{
				if (p_Flag)
				{
					Vec2 posStar = p_Flag->GetPosObject();
					Vec2 posChar = GetChar->GetPosObject();
					//
					float distance = sqrt((posChar.x - posStar.x) * (posChar.x - posStar.x) + (posChar.y - posStar.y) * (posChar.y - posStar.y));
					float delta_w_char = GetChar->GetSizeMainChar().width / 2;
					float delta_w_star = p_Flag->GetSizeObject().width / 2;
					//
					if (distance < delta_w_char + delta_w_star && !p_IsPassLevel)
					{
						OnPassLevel();
					}
				}
			}
		}

	}
}

void		ManageObjectAP::OnPassLevel()
{
#ifdef USE_CC_AUDIO
		{
			GetSound->PauseTheCurrentMusic();
		}
#endif
		//
		HUDMenu::GetInstance()->OnHide();
		HUDMenu::GetInstance()->SetMenuSwithTo(MENULAYER::RESULTMENU);
		ResultMenu::GetInstance()->SetScoreGame(TYPE_END_RESULT::RESULT_PASS_LEVEL);
		//
		p_IsPassLevel = true;
}

void		ManageObjectAP::CalCulateTimeDeath(TYPE_END_RESULT type)
{
	p_time_death_cd -= p_delta_time_death_cd;
	if (p_time_death_cd <= 0)
	{
		p_time_death_cd = p_origin_time_death_cd;
		//
#ifdef USE_CC_AUDIO
		{
			GetSound->PauseTheCurrentMusic();
		}
#endif
		//
		if (APMgr->IsOnStoryMode() && type == TYPE_END_RESULT::RESULT_DIE)
		{
			ManageObjectAP::GetInstance()->ReloadTheGame();
			HUDMenu::GetInstance()->ResetScaleCDTimer();
#if USE_CC_AUDIO
			GetSound->PlayCurrentMusic();
#endif
			return;
		}

		//
		HUDMenu::GetInstance()->OnHide();
		HUDMenu::GetInstance()->SetMenuSwithTo(MENULAYER::RESULTMENU);
		ResultMenu::GetInstance()->SetScoreGame(type);
	}
}

void		ManageObjectAP::OnDraw(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch)
{
	//update transform
	for (int i = 0; i < p_ListOrbits.size(); i++)
	{
		p_ListOrbits.at(i)->onDrawOrbit(renderer, transform, flags, spriteBatch);
	}
	GetChar->onDrawMainChar(renderer, transform, flags, spriteBatch);
	if (APMgr->IsOnStoryMode())
	{
		if (p_Flag)
		{
			p_Flag->onDrawObject(renderer, transform, flags, spriteBatch);
		}
	}

	for (int i = 0; i < p_ListOrbits.size(); i++)
	{
		p_ListOrbits.at(i)->onDrawInteractiveObj(renderer, transform, flags, spriteBatch);
		if (APMgr->IsUseTheBoost(TYPE_BOOST::B_GRAVITY_BOOST))
		{
			p_ListOrbits.at(i)->onDrawBubble(renderer, transform, flags, spriteBatch);
		}
	}
	if (APMgr->IsOnStoryMode())
	{
		for (int i = 0; i < p_ListStar.size(); i++)
		{
			if (!p_animateStar[i].IsStarExist)
			{
				continue;
			}
			p_ListStar.at(i)->onDrawObject(renderer, transform, flags, spriteBatch);
		}
	}
	//
	auto effect = EffectMgr->GetParticle(p_name_explosive_effect);
	if (effect)
	{
		effect->DrawParticle(renderer, transform, flags);
	}

}

void		ManageObjectAP::OnVisit(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch)
{
	for (int i = 0; i < p_ListOrbits.size(); i++)
	{
		p_ListOrbits.at(i)->onVisitOrbit(renderer, transform, flags, spriteBatch);
	}
	GetChar->onVisitMainChar(renderer, transform, flags, spriteBatch);
	if (APMgr->IsOnStoryMode())
	{
		if (p_Flag)
		{
			p_Flag->onVisitObject(renderer, transform, flags, spriteBatch);
		}
	}

	for (int i = 0; i < p_ListOrbits.size(); i++)
	{
		p_ListOrbits.at(i)->onVisitInteractiveObj(renderer, transform, flags, spriteBatch);
		if (APMgr->IsUseTheBoost(TYPE_BOOST::B_GRAVITY_BOOST))
		{
			p_ListOrbits.at(i)->onVisitBubble(renderer, transform, flags, spriteBatch);
		}
	}
	if (APMgr->IsOnStoryMode())
	{
		for (int i = 0; i < p_ListStar.size(); i++)
		{
			if (p_animateStar[i].IsStarExist)
			{
				p_ListStar.at(i)->onVisitObject(renderer, transform, flags, spriteBatch);
			}
		}
	}


	//
	auto effect = EffectMgr->GetParticle(p_name_explosive_effect);
	if (effect)
	{
		effect->DrawParticle();
	}
}

OrbitObject* ManageObjectAP::GetOrbitAtIdx(int idx)
{
	if (idx >= p_ListOrbits.size() || idx < 0)
		return NULL;
	return p_ListOrbits.at(idx);
}
void		ManageObjectAP::CheckCollitionCharWithStar()
{
	for (int i = 0; i < p_ListStar.size(); i++)
	{
		if (!p_animateStar[i].IsStarExist)
		{
			continue;
		}
		Vec2 posStar = p_ListStar.at(i)->GetPosObject();
		Vec2 posChar = GetChar->GetPosObject();
		//
		float distance = sqrt((posChar.x - posStar.x) * (posChar.x - posStar.x) + (posChar.y - posStar.y) * (posChar.y - posStar.y));
		float delta_w_char = GetChar->GetSizeMainChar().width / 2;
		float delta_w_star = p_ListStar.at(i)->GetSizeObject().width / 2;
		//
		if (distance < delta_w_char + delta_w_star)
		{
			p_CurrentStarGain++;
			//p_ListStar.at(i)->IsVisible
			p_animateStar[i].IsStarExist = false;
			//add effect gain star
			EffectMgr->AddParticle(PARTICLE_TYPE::P_GAIN_STAR, posStar, "global_gain_star", true);
			//play soud
#if USE_CC_AUDIO
			GetSound->PlaySoundEffect(XMLMgr->GetPathSound("SFX_GAIN_GOLD"));
#endif
			return;
		}
	}
}

void		ManageObjectAP::CheckCollitionCharWithOrbit()
{
	for (int i = 0; i < p_ListOrbits.size(); i++)
	{
		Vec2 posOrbit = p_ListOrbits.at(i)->GetPosOrbit();
		Vec2 posChar = GetChar->GetPosObject();
		//
		float distance = sqrt((posChar.x - posOrbit.x) * (posChar.x - posOrbit.x) + (posChar.y - posOrbit.y) * (posChar.y - posOrbit.y));
		//
		float effect = p_ListOrbits.at(i)->EffectRaius() + (GetChar->GetSizeMainChar().height * GetChar->GetRatioSizeDistance());
		//
		if (GetChar->GetCurrentOrbitIdx() < 0 && distance <= effect && !p_ListOrbits.at(i)->IsExplosive())
		{
			if (GetChar->IsOnReviving() && i != GetChar->GetCurrentOrbitIdx())
			{
				continue;
			}

			//is death orbit
			if (p_ListOrbits.at(i)->TypeOrbit() == ORBIT_BOOM)
			{
				setExplosiveAtOrbit(i , true);
				//
				return;
			}

			//is time bomb orbit
			if (p_ListOrbits.at(i)->TypeOrbit() == ORBIT_TIME_BOOM)
			{
				p_ListOrbits.at(i)->ActiveTheBoom();
			}

			// not the dead orbit
			if (APMgr->GetCurrentGamePlay() == TYPE_GAMEPLAY::GAMEPLAY_ENDLESS_RUN ||
				APMgr->GetCurrentGamePlay() == TYPE_GAMEPLAY::GAMEPLAY_TIME_RUN)
			{
				if (i > GetChar->GetPreviousOrbitIdx())
				{
					int delta = i - GetChar->GetPreviousOrbitIdx();
					int score = ManageObjectAP::GetInstance()->GetScore();
					score += delta;
					ManageObjectAP::GetInstance()->SetScore(score);
				}
				//quick move
				OnQuickMoveOrbit(i);
			}

			GetChar->SetCurrentOrbitIdx(i);
#if USE_CC_AUDIO
			GetSound->PlaySoundEffect(XMLMgr->GetPathSound("SFX_END_JUMP"));
#endif
#if USE_JUMP_PARTICLE
			EffectMgr->KillParticle(GetChar->GetNameJump(), 1.0f);
#endif
			GetChar->StopJumpGravity(true);
			return;
		}
	}
}

void		ManageObjectAP::setExplosiveAtOrbit(int idx, bool forceCharDead /*= false*/)
{
	//player dead
	if (forceCharDead)
	{
		GetChar->SetPosMainChar(Point(GetChar->GetPosMainChar().x, /*GetChar->GetPosMainChar().y*/ -GetWinSize().height));
	}
	//add explosive effect
	EffectMgr->AddParticle(PARTICLE_TYPE::P_ORBIT_EXPLOSION, p_ListOrbits.at(idx)->GetOriginPoint(), p_name_explosive_effect, false);
	//disappear the orbit
	p_ListOrbits.at(idx)->SetExplosive(true);
	//
#if USE_CC_AUDIO
	GetSound->PlaySoundEffect(XMLMgr->GetPathSound("SFX_EXPLOSTION"));
#endif
}

OrbitObject* ManageObjectAP::CreateOrbitWithPara(TYPE_ORBIT typeOrbit, ObjectGamePlayDetail * obj)
{
	OrbitObject * temp = new OrbitObject();
	std::string nameOrbitLoad = "orbit";
	if (APMgr->GetCurrentEviType() == EVIROMENT_TYPE::EV_SNOW)
	{
		nameOrbitLoad = "orbit_snow";
	}
	if (typeOrbit == TYPE_ORBIT::ORBIT_BOOM)
	{
		nameOrbitLoad = "bomb";
	}
#if USE_SPRITE_STORE
	auto te = TextureMgr->GetSpriteFromMemory(nameOrbitLoad);
	CCASSERT(te, "texture invalid or not loadable");
	temp->LoadOrbitObject(3, "", NULL, te, typeOrbit, obj);
#else
	auto te = TextureMgr->GetTexureFromMemory(nameOrbitLoad);
	CCASSERT(te, "texture invalid or not loadable");
	temp->LoadOrbitObject(2, "", te, IsDeadOrbit);
#endif

	Vec2 Pos = obj->pos;
	float scaleRatio = obj->ratio_scale;

	temp->SetSizeOrbit(temp->GetSizeOrbit() / scaleRatio, scaleRatio);

	temp->SetPosOrbit(Vec2(Pos.x * GetWinSize().width, Pos.y*GetWinSize().height));

	temp->GenerateTypeObject();

#if ! USE_SPRITE_STORE
	te->autorelease(); //note
#endif
	return temp;
}

OrbitObject* ManageObjectAP::CreateOrbit(TYPE_ORBIT typeOrbit, bool IsCalcualtePosOrbit)
{
	OrbitObject * temp = new OrbitObject();
	std::string nameOrbitLoad = "orbit";
	if (APMgr->GetCurrentEviType() == EVIROMENT_TYPE::EV_SNOW)
	{
		nameOrbitLoad = "orbit_snow";
	}
	if (typeOrbit == TYPE_ORBIT::ORBIT_BOOM)
	{
		nameOrbitLoad = "bomb";
	}
#if USE_SPRITE_STORE
	auto te = TextureMgr->GetSpriteFromMemory(nameOrbitLoad);
	CCASSERT(te, "texture invalid or not loadable");
	temp->LoadOrbitObject(3, "", NULL, te, typeOrbit);
#else
	auto te = TextureMgr->GetTexureFromMemory(nameOrbitLoad);
	CCASSERT(te, "texture invalid or not loadable");
	temp->LoadOrbitObject(2, "", te, IsDeadOrbit);
#endif
	int ratio = GetRandom(XMLMgr->GetOrbitData()->ratio[1], XMLMgr->GetOrbitData()->ratio[0]);
	//if (IsDeadOrbit)
	//{
	//	//the dead orbit alway is the smallest size
	//	ratio = XMLMgr->GetOrbitData()->ratio[1];
	//}
	temp->SetSizeOrbit(temp->GetSizeOrbit() / ratio, ratio);
	if (IsCalcualtePosOrbit)
	{
		CalculateThePosOrbit(p_ListOrbits.at(p_ListOrbits.size() - 1), temp);
		temp->GenerateTypeObject();
	}
#if ! USE_SPRITE_STORE
	te->autorelease(); //note
#endif
	return temp;
}

void		ManageObjectAP::ReloadTheGame()
{
	FreeObjectInAP();
	ManageObjectAP::InitTheObjectGame(APMgr->GetCurrentGamePlay(),false);
	APMgr->ResetAllBoost(false);
}

void	 ManageObjectAP::ReleaseTheGame(bool forceResetAllBoost)
{
	FreeObjectInAP();
	//CC_SAFE_RELEASE(GetChar);
	FreeChar;
	APMgr->ResetAllBoost(forceResetAllBoost);
	IsInitMainChar = true;
}

void		ManageObjectAP::FreeObjectInAP()
{
	//CC_SAFE_RELEASE(GetChar);
	//FreeChar;
	for (int i = 0; i < p_ListOrbits.size(); i++)
	{
		p_ListOrbits.at(i)->FreeOrbit();
	}
	p_ListOrbits.clear();

	for (int i = 0; i < p_ListStar.size(); i++)
	{
		p_ListStar.at(i)->FreeObject();
	}
	p_ListStar.clear();

	if (p_Flag)
	{
		delete p_Flag;
		p_Flag = NULL;
	}
	SetScore(0);
	p_IsPassLevel = false;
}

void		ManageObjectAP::SetScore(int p)
{
	if (p_Score != SaveMgr->GetDataSave()->GetCurPoint())
	{
		CC_ASSERT("HACK DETECTION!");
		p_Score = 0;
		SaveMgr->GetDataSave()->SetCurPoint(0);
	}
	else
	{
		p_Score = p;
		SaveMgr->GetDataSave()->SetCurPoint(p_Score);
	}

}

int			ManageObjectAP::GetScore()
{
	if (p_Score != SaveMgr->GetDataSave()->GetCurPoint())
	{
		CC_ASSERT("HACK DETECTION!");
		p_Score = 0;
		return 0;
	}
	else
	{
		return p_Score;
	}
}

int			ManageObjectAP::GetIdxOrbitOnScreen()
{
	Rect rec(0, 0, GetWinSize().width, GetWinSize().height);

	for (int i = 0; i < p_ListOrbits.size(); i++)
	{
		Point p = Point(p_ListOrbits.at(i)->GetPosObject().x + p_ListOrbits.at(i)->GetRadius(), p_ListOrbits.at(i)->GetPosObject().y);
		if (rec.containsPoint(p) && !(p_ListOrbits.at(i)->TypeOrbit() == ORBIT_BOOM))
		{
			if (p_ListOrbits.at(i)->GetPosObject().x < GetWinSize().width / 3 )
			{ 
				if (!p_ListOrbits.at(i + 1)->TypeOrbit() == ORBIT_BOOM)
					return i + 1;
				else
					return i + 2;
			}
			return i ;
		}
	}
	return -1;
}

void		ManageObjectAP::OnQuickMoveOrbit(int idxOrbitMove)
{
	if (idxOrbitMove >= 0 )
	{
		//get current position of orbit idx
		Point pos = p_ListOrbits.at(idxOrbitMove)->GetPosOrbit();
		//check valid pos to move
		if (pos.x >  GetWinSize().width / 2.f)
		{
			p_time_move = 10.f;
			p_delta_time_move = 1.f;
			//calculate the distance to move
			p_distance_quick_move = abs(pos.x - GetWinSize().width / 3.f);
			//
			p_delta_distance_quick_move = p_distance_quick_move / (p_time_move / p_delta_time_move);
		}

	}
}

DataXMLReturn  ManageObjectAP::GetParamFromDataXML(std::string nameobj, bool forceGetDesign)
{
	std::string nameGP = APMgr->GetCurrentNameLevel();
	auto cur_player = XMLMgr->XMLMgr->GetGamePlayMgr()->GetCharDetailAtLevel(nameGP);
	if (nameobj == "speed_rotate_char")
	{
		if (!APMgr->IsOnStoryMode() || cur_player->speed_rotate == 0 || forceGetDesign)
		{
			return DataXMLReturn(XMLMgr->GetMainCharData()->p_speedRotate);
		}
		else
		{
			return cur_player->speed_rotate;
		}
	}
	else if (nameobj == "Gravity_press_char")
	{
		if (!APMgr->IsOnStoryMode() || cur_player->gravity_press == 0 || forceGetDesign)
		{
			return DataXMLReturn(XMLMgr->GetMainCharData()->g_GravityPress);
		}
		else
		{
			return cur_player->gravity_press;
		}
	}
	else if (nameobj == "Scale_ratio_char")
	{	
		if (!APMgr->IsOnStoryMode() || cur_player->scale_ratio == 0 || forceGetDesign)
		{
			return DataXMLReturn(XMLMgr->GetMainCharData()->p_ScaleFactor);
		}
		else
		{
			return cur_player->scale_ratio;
		}
	}
	else if (nameobj == "speed_move_char")
	{
		if (!APMgr->IsOnStoryMode() || cur_player->speed_move == 0 || forceGetDesign)
		{
			return DataXMLReturn(XMLMgr->GetMainCharData()->p_speedMove);
		}
		else
		{
			return cur_player->speed_move;
		}
	}
	else if (nameobj == "speed_jump_char")
	{
		if (!APMgr->IsOnStoryMode() || cur_player->speed_jump == 0 || forceGetDesign)
		{
			return DataXMLReturn(XMLMgr->GetMainCharData()->p_speedJump);
		}
		else
		{
			return cur_player->speed_jump;
		}
	}
	else if (nameobj == "gravity_press_delta_char")
	{
		if (!APMgr->IsOnStoryMode() || cur_player->gravity_press_delta == 0 || forceGetDesign)
		{
			return DataXMLReturn(XMLMgr->GetMainCharData()->g_GravityDelta);
		}
		else
		{
			return cur_player->gravity_press_delta;
		}
	}
	else if (nameobj == "jump_duration_char")
	{
		return DataXMLReturn(XMLMgr->GetMainCharData()->p_JumpDuration);		
	}
	return DataXMLReturn();
}