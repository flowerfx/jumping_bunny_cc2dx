#include "ParicleManager.h"
#include "../Utility/ObjectTexture.h"
#include "../Utility/LoadDataXML.h"
/********************************************************
*
Particle
*
*********************************************************/
Particle::Particle()
{
	p_TypeEffect = P_NONE;
	p_GlobalParticle = false;
	p_reduceEffectBaseHarware = 1;
	if (XMLMgr->GetHarwardProfile() == PMED)
	{
		p_reduceEffectBaseHarware = 2;
	}
	else if (XMLMgr->GetHarwardProfile() == PLOW)
	{
		p_reduceEffectBaseHarware = 4;
	}
}
Particle::~Particle()
{
	CC_SAFE_RELEASE_NULL(p_Emitter);
}
void Particle::InitSnowEffect()
{
	// get arg of effect snow
	Size _winsize = GetWinSize();
	//
	p_Emitter = ParticleSystemQuad::createWithTotalParticles(100 / p_reduceEffectBaseHarware);
	p_Emitter->setLife(10);
	p_Emitter->setLifeVar(1);
	p_Emitter->setDuration(-1);
	p_Emitter->setPosition(0, 0);
	p_Emitter->setPosVar(Vec2(_winsize.width, _winsize.height * 2));
	// gravity
	p_Emitter->setGravity(Vec2(0, -60));
	// speed of particles
	p_Emitter->setSpeed(0);
	p_Emitter->setSpeedVar(50);
	//
	p_Emitter->setStartSize(15 * GetScaleFactor());
	p_Emitter->setStartSizeVar(2 * GetScaleFactor());
	p_Emitter->setEndSize(30 * GetScaleFactor());
	//
	p_Emitter->setStartColor(Color4F(1.0f,1.0f,1.0f,1.0f));
	p_Emitter->setStartColorVar(Color4F(1.0f / 255.f, 1.0f / 255.f, 1.0f / 255.f, 1.0f));
	p_Emitter->setEndColor(Color4F(1.0f, 1.0f, 1.0f, 1.0f));
	p_Emitter->setEndColorVar(Color4F(1.0f / 255.f, 1.0f / 255.f, 1.0f / 255.f, 1.0f));
	//
	p_Emitter->setEmissionRate(p_Emitter->getTotalParticles() / p_Emitter->getLife());
	//
	auto te = TextureMgr->GetSpriteFromMemory("eff_snow");
	//te->autorelease();
	//
	p_Emitter->setTextureWithRect(te->getTexture(),te->getTextureRect());
	//
	BlendFunc blendFunc;
	blendFunc.src = 770.f;
	blendFunc.dst = 1.f;
	p_Emitter->setBlendFunc(blendFunc);
}

void Particle::InitCometEffect()
{
	p_Emitter = ParticleSystemQuad::createWithTotalParticles(120 / p_reduceEffectBaseHarware);
	//SetTexture
	auto te = TextureMgr->GetSpriteFromMemory("eff_Point");
	//te->autorelease();
	p_Emitter->setTextureWithRect(te->getTexture(), te->getTextureRect());
	p_Emitter->setDuration(-1);
	//SetColor
	p_Emitter->setEndColor(Color4F(Color4B(0, 63, 133, 255)));
	p_Emitter->setEndColorVar(Color4F(Color4B(0, 0, 0, 0)));
	p_Emitter->setStartColor(Color4F(Color4B(34, 8, 0, 255)));
	p_Emitter->setStartColorVar(Color4F(Color4B(0, 0, 0, 0)));
	//Set Pos
	p_Emitter->setPosition(Vec2(0, 8));
	p_Emitter->setPosVar(Vec2(0, 5));
	//
	p_Emitter->setSpeed(165.f);
	p_Emitter->setSpeedVar(29.f);
	//
	p_Emitter->setLife(0.0f);
	p_Emitter->setLifeVar(1.8f);
	p_Emitter->setStartSize(37.f* GetScaleFactor());
	p_Emitter->setStartSizeVar(5.f* GetScaleFactor());
	p_Emitter->setAngle(99.f);
	//
	p_Emitter->setEmissionRate(100);
	p_Emitter->setEmitterMode(ParticleSystem::Mode::GRAVITY);
	p_Emitter->setPositionType(ParticleSystem::PositionType::GROUPED);
	//
	BlendFunc blendFunc;
	blendFunc.src = 770.f;
	blendFunc.dst = 1.f;
	p_Emitter->setBlendFunc(blendFunc);
}

void Particle::InitWinEffect()
{
	//Size _winsize = GetWinSize();
	//
	p_Emitter = ParticleSystemQuad::createWithTotalParticles(100 / p_reduceEffectBaseHarware);
	//SetTexture
	auto te = TextureMgr->GetSpriteFromMemory("eff_starly");
	//te->autorelease();
	p_Emitter->setTextureWithRect(te->getTexture(), te->getTextureRect());
	p_Emitter->setDuration(5);
	//SetColor
	p_Emitter->setEndColor(Color4F(Color4B(47,116, 156, 25)));
	p_Emitter->setEndColorVar(Color4F(Color4B(135, 154, 119, 25)));
	p_Emitter->setStartColor(Color4F(Color4B(188, 61, 137, 255)));
	p_Emitter->setStartColorVar(Color4F(Color4B(159, 198, 79, 255)));
	//Set Pos
	p_Emitter->setPosition(Vec2(0, 0));
	p_Emitter->setPosVar(Vec2(0, 0));
	//
	p_Emitter->setSpeed(343.f* GetScaleFactor());
	p_Emitter->setSpeedVar(109.f* GetScaleFactor());
	p_Emitter->setTangentialAccel(0.0f);
	p_Emitter->setTangentialAccelVar(492.f);
	//
	p_Emitter->setLife(3.51f);
	p_Emitter->setLifeVar(1.04f);
	p_Emitter->setStartSize(17.f* GetScaleFactor());
	p_Emitter->setStartSizeVar(14.f* GetScaleFactor());
	p_Emitter->setEndSize(28.f* GetScaleFactor());
	p_Emitter->setEndSizeVar(15.f* GetScaleFactor());
	p_Emitter->setAngle(270.f);
	p_Emitter->setAngleVar(12.f);
	//
	p_Emitter->setEmissionRate(100);
	p_Emitter->setEmitterMode(ParticleSystem::Mode::GRAVITY);
	p_Emitter->setPositionType(ParticleSystem::PositionType::RELATIVE);
	//
	BlendFunc blendFunc;
	blendFunc.src = 771;
	blendFunc.dst = 1;
	p_Emitter->setBlendFunc(blendFunc);
}

void Particle::InitSunLightEffect()
{
	Size _winsize = GetWinSize();
	//
	p_Emitter = ParticleSystemQuad::createWithTotalParticles(40 / p_reduceEffectBaseHarware);
	//SetTexture
	auto te = TextureMgr->GetSpriteFromMemory("eff_distorled");
	//te->autorelease();
	p_Emitter->setTextureWithRect(te->getTexture(), te->getTextureRect());
	p_Emitter->setDuration(-1);
	//SetColor
	p_Emitter->setStartColor(Color4F(1.0f, 1.0f, 1.0f, 0.1f));
	p_Emitter->setStartColorVar(Color4F(0.0, 0.0f, 0.0f, 0.1f));
	p_Emitter->setEndColor(Color4F(1.0f, 1.0f, 1.0f, 0.1f));
	p_Emitter->setEndColorVar(Color4F(0.0, 0.0f, 0.0f, 0.1f));
	//Set Pos
	p_Emitter->setPosition(Vec2(0, 0));
	p_Emitter->setPosVar(Vec2(_winsize.width, 0));
	//
	p_Emitter->setGravity(Vec2(-98, 0));
	//
	p_Emitter->setSpeed(30.f* GetScaleFactor());
	p_Emitter->setSpeedVar(30.f* GetScaleFactor());
	//
	p_Emitter->setLife(10.0f);
	p_Emitter->setLifeVar(1.f);
	p_Emitter->setStartSize(296.f* GetScaleFactor());
	p_Emitter->setStartSizeVar(10.f* GetScaleFactor());
	p_Emitter->setEndSize(304.f* GetScaleFactor());
	p_Emitter->setAngle(180.f);
	p_Emitter->setAngleVar(1.f);
	p_Emitter->setStartSpin(127.f);
	p_Emitter->setEndSpin(132.f);
	p_Emitter->setEndSpinVar(2.f);
	//
	p_Emitter->setEmissionRate(100);
	p_Emitter->setEmitterMode(ParticleSystem::Mode::GRAVITY);
	p_Emitter->setPositionType(ParticleSystem::PositionType::RELATIVE);
	//
	BlendFunc blendFunc;
	blendFunc.src = 770;
	blendFunc.dst = 1;
	p_Emitter->setBlendFunc(blendFunc);
}

void Particle::InitStarFallEffect()
{
	Size _winsize = GetWinSize();
	//
	p_Emitter = ParticleSnow::createWithTotalParticles(60 / p_reduceEffectBaseHarware);
	//SetTexture
	auto te = TextureMgr->GetSpriteFromMemory("eff_stars");
	//te->autorelease();
	p_Emitter->setTextureWithRect(te->getTexture(), te->getTextureRect());
	p_Emitter->setDuration(-1);
	//SetColor
	p_Emitter->setEndColor(Color4F(Color4B(0, 0, 255, 0)));
	p_Emitter->setEndColorVar(Color4F(Color4B(179, 205, 237, 255)));
	p_Emitter->setStartColor(Color4F(Color4B(195, 229, 127, 255)));
	p_Emitter->setStartColorVar(Color4F(Color4B(88, 56, 145, 255)));
	//Set Pos
	p_Emitter->setPosition(Vec2(0, 0));
	p_Emitter->setPosVar(Vec2(_winsize.width , 0));
	//
	p_Emitter->setSpeed(125.f);
	p_Emitter->setSpeedVar(30);
	p_Emitter->setGravity(Vec2(0,-10));
	//
	p_Emitter->setLife(3.f);
	p_Emitter->setLifeVar(2.14f);
	p_Emitter->setStartSize(10.f* GetScaleFactor());
	p_Emitter->setStartSizeVar(16.f* GetScaleFactor());
	p_Emitter->setEndSize(7.f* GetScaleFactor());
	p_Emitter->setEndSizeVar(0.f* GetScaleFactor());
	//
	p_Emitter->setEmissionRate(p_Emitter->getTotalParticles() / p_Emitter->getLife());
	//
	BlendFunc blendFunc;
	blendFunc.src = 770;
	blendFunc.dst = 1;
	p_Emitter->setBlendFunc(blendFunc);
}

void Particle::InitOrbitExplosiveEffect()
{
	//Size _winsize = GetWinSize();
	//
	//SetTexture
	auto te = TextureMgr->GetSpriteFromMemory("eff_Point");
	p_Emitter = ParticleFlower::createWithTotalParticles(400 / p_reduceEffectBaseHarware);
	//
	//te->autorelease();
	p_Emitter->setTextureWithRect(te->getTexture(), te->getTextureRect());
	p_Emitter->setDuration(0.5f);
	//SetColor
	p_Emitter->setEndColor(Color4F(Color4B(0, 0, 0, 0)));
	p_Emitter->setEndColorVar(Color4F(Color4B(0, 0, 0, 0)));
	p_Emitter->setStartColor(Color4F(Color4B(206, 139, 104, 255)));
	p_Emitter->setStartColorVar(Color4F(Color4B(56, 59, 61, 0)));
	//Set Pos
	p_Emitter->setPosition(Vec2(0, 0));
	p_Emitter->setPosVar(Vec2(0, 0));
	//
	p_Emitter->setSpeed(900.f);
	p_Emitter->setSpeedVar(3.f);
	//
	p_Emitter->setLife(10.0f);
	p_Emitter->setLifeVar(1.17f);
	p_Emitter->setStartSize(33.f* GetScaleFactor());
	p_Emitter->setStartSizeVar(36.f* GetScaleFactor());
	p_Emitter->setEndSize(14.f* GetScaleFactor());
	p_Emitter->setEndSizeVar(0.f* GetScaleFactor());
	//
	p_Emitter->setEmissionRate(10000);
	////
	BlendFunc blendFunc;
	blendFunc.src = 770;
	blendFunc.dst = 1;
	p_Emitter->setBlendFunc(blendFunc);
}

void Particle::InitGainStarEffect()
{
	//SetTexture
	auto te = TextureMgr->GetSpriteFromMemory("eff_starly");
	p_Emitter = ParticleFlower::createWithTotalParticles(10 / p_reduceEffectBaseHarware);
	//
	p_Emitter->setGravity(Vec2(-38, 0));
	//te->autorelease();
	p_Emitter->setTextureWithRect(te->getTexture(), te->getTextureRect());
	p_Emitter->setDuration(0.5f);
	//SetColor
	p_Emitter->setEndColor(Color4F(Color4B(255, 255, 1, 0.1 * 255)));
	p_Emitter->setEndColorVar(Color4F(Color4B(18, 18, 18, 0.1 * 255)));
	p_Emitter->setStartColor(Color4F(Color4B(255, 195, 1, 0.5 * 255)));
	p_Emitter->setStartColorVar(Color4F(Color4B(18, 18, 18, 0.1 * 255)));
	//Set Pos
	p_Emitter->setPosition(Vec2(0, 0));
	p_Emitter->setPosVar(Vec2(0, 0));
	//
	p_Emitter->setSpeed(180.f);
	p_Emitter->setSpeedVar(10.f);
	//
	p_Emitter->setLife(2.5f);
	p_Emitter->setLifeVar(1.0f);
	p_Emitter->setStartSize(16.f* GetScaleFactor());
	p_Emitter->setStartSizeVar(5.f* GetScaleFactor());
	p_Emitter->setEndSize(0.f* GetScaleFactor());
	p_Emitter->setEndSizeVar(2.f* GetScaleFactor());
	//
	p_Emitter->setEmissionRate(100);
	////
	BlendFunc blendFunc;
	blendFunc.src = 770;
	blendFunc.dst = 771;
	p_Emitter->setBlendFunc(blendFunc);
}

void Particle::InitParticle(PARTICLE_TYPE type, bool IsGlobal)
{
	if (type == P_SNOW_FALL)
	{
		InitSnowEffect();
	}
	else if (type == P_CHAR_COMET)
	{
		InitCometEffect();
	}
	else if (type == P_WIN_EFFECT)
	{
		InitWinEffect();
	}
	else if (type == P_SUN_LIGHT)
	{
		InitSunLightEffect();
	}
	else if (type == P_STAR_FALL)
	{
		InitStarFallEffect();
	}
	else if (type == P_ORBIT_EXPLOSION)
	{
		InitOrbitExplosiveEffect();
	}
	else if (type == P_GAIN_STAR)
	{
		InitGainStarEffect();
	}
	p_GlobalParticle = IsGlobal;
	p_TypeEffect = type;
	p_Emitter->retain();
}
/*
draw
*/
void Particle::DrawParticle(Renderer *renderer, const Mat4& transform, uint32_t flags)
{
	CCASSERT(p_Emitter, "object not invalid");
	if (!IsAlive())
		return;
	p_Emitter->draw(renderer, transform, flags);
}
void Particle::DrawParticle()
{
	CCASSERT(p_Emitter, "object not invalid");
	if (!IsAlive())
		return;
	p_Emitter->visit();
}
/*
update
*/
void Particle::UpdateParticle(float dt)
{
	CCASSERT(p_Emitter, "object not invalid");
	p_Emitter->update(dt);
}
//method in game
void Particle::SetPosition(Point pos)
{
	CCASSERT(p_Emitter, "object not invalid");
	p_Emitter->setSourcePosition(pos);
}

void Particle::SetAngle(float angle)
{
	CCASSERT(p_Emitter, "object not invalid");
	p_Emitter->setAngle(angle);
}
//
bool Particle::IsAlive()
{
	CCASSERT(p_Emitter, "object not invalid");
	return p_Emitter->isActive();
}
void Particle::RemoveParticle(float time)
{
	CCASSERT(p_Emitter, "object not invalid");
	return p_Emitter->setDuration(time);
}

/********************************************************
*
	ParticleManager
*
*********************************************************/

ParticleManager * ParticleManager::m_Instance = NULL;

ParticleManager::ParticleManager()
{
	p_listParticle.clear();
	p_listParticleEndLife.clear();
}
ParticleManager::~ParticleManager()
{
	p_listParticle.clear();
	p_listParticleEndLife.clear();
}

void ParticleManager::onInit()
{
	//nothing here
}
/*
draw
*/
void ParticleManager::onDraw(Renderer *renderer, const Mat4& transform, uint32_t flags)
{
	if (p_listParticle.size() <= 0)
		return;

	for (auto it = p_listParticle.begin(); it != p_listParticle.end(); ++it)
	{
		if (it->second->IsGlobalParticle() && it->second->IsAlive())
		{
			(it->second)->DrawParticle(renderer, transform, flags);
		}
	}
}
void ParticleManager::onDraw()
{
	if (p_listParticle.size() <= 0)
		return;

	for (auto it = p_listParticle.begin(); it != p_listParticle.end(); ++it)
	{
		if (it->second->IsGlobalParticle() && it->second->IsAlive())
		{
			(it->second)->DrawParticle();
		}
	}
}
/*
update
*/
void ParticleManager::onUpdate(float dt)
{
	if (p_listParticle.size() <= 0)
		return;
	//
	//update
	for (auto it = p_listParticle.begin(); it != p_listParticle.end(); ++it)
	{
		if ((it->second)->IsAlive())
		{
			if (it->second->IsGlobalParticle())
			{
				(it->second)->UpdateParticle(dt);
			}
			//
			if (!(it->second)->IsAlive())
			{
				p_listParticleEndLife.push_back(it->first);
			}
		}
	}
}
void ParticleManager::AddParticle(PARTICLE_TYPE type, Point Position, std::string name, bool IsGlobal)
{
	//remove particle if it exist;
	if (p_listParticle.size() > 0)
	{
		auto it = p_listParticle.find(name);
		if (it != p_listParticle.end())
		{
			p_listParticle.erase(name);
		}
	}
	auto par = new Particle();
	par->InitParticle(type, IsGlobal);
	par->SetPosition(Position);
	//
	par->autorelease();
	//
	p_listParticle.insert(name, par);
}

void ParticleManager::KillParticle(std::string name , float time)
{
	if (p_listParticle.size() <= 0)
		return;
	auto it = p_listParticle.find(name);
	if (it != p_listParticle.end())
	{
		(it->second)->RemoveParticle(time);
	}
}
Particle * ParticleManager::GetParticle(std::string name)
{
	if (p_listParticle.size() <= 0)
		return NULL;
	if (name == "")
		return NULL;
	auto it = p_listParticle.find(name);
	if (it != p_listParticle.end())
	{
		return (it->second);
	}
	return NULL;
}

void ParticleManager::CheckAndRemoveEndLifeParticle()
{
	//remove endlife particle
	for (int i = 0; i < p_listParticleEndLife.size(); i++)
	{
		p_listParticle.erase(p_listParticleEndLife.at(i));
	}
	p_listParticleEndLife.clear();
}