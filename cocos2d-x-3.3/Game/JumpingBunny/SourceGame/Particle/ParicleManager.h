#ifndef _PARTICLE_MANAGER_H_
#define  _PARTICLE_MANAGER_H_

#include "Init/DisplayView.h"
#include "Init/ParamTexture.h"
#include "cocos2d.h"
USING_NS_CC;
#define UPDATE_PARTICLE_THREAD 1 //the update of the particle will be in thread to improve perf
enum PARTICLE_TYPE
{
	P_NONE = 0,
	P_SNOW_FALL , // snow fall effect use in game eviroment (winter)
	P_STAR_FALL , // star fall effect use in game eviroment (moon night)
	P_SUN_LIGHT , // sun light effect use in game eviroment (daylight)
	P_WIN_EFFECT, // effect when reach hight score in result menu
	P_CHAR_EXPLOSION , // the bunny die effect
	P_ORBIT_EXPLOSION, // the planet explosion effect
	P_CHAR_COMET, // effect when the bunny jump
	P_GAIN_STAR // effect gain star
};

class Particle : public Ref
{
protected:
	ParticleSystemQuad*    p_Emitter;
	PARTICLE_TYPE		   p_TypeEffect;
	//
	void InitSnowEffect();
	void InitCometEffect();
	void InitWinEffect();
	void InitSunLightEffect();
	void InitStarFallEffect();
	void InitOrbitExplosiveEffect();
	void InitGainStarEffect();
	//
	bool				  p_GlobalParticle;
	int					  p_reduceEffectBaseHarware;
public:
	Particle();
	~Particle();
	void InitParticle(PARTICLE_TYPE type, bool IsGlobal);
	/*
			draw
	*/
	void DrawParticle(Renderer *renderer, const Mat4& transform, uint32_t flags);
	void DrawParticle();
	/*
			update
	*/
	void UpdateParticle(float dt);
	//method in game
	void SetPosition(Point pos);
	void SetAngle(float angle);
	//
	bool IsAlive();
	void RemoveParticle(float time);
	//
	bool IsGlobalParticle() { return p_GlobalParticle; }

};

class ParticleManager
{
private:
	static ParticleManager * m_Instance;
	Map<std::string , Particle*> p_listParticle; 
	std::vector<std::string> p_listParticleEndLife;
public:
	ParticleManager();
	~ParticleManager();
	static ParticleManager * GetInstance()
	{
		if (!m_Instance)
		{
			m_Instance = new ParticleManager();
		}
		return m_Instance;
	}
	void onInit();
	/*
	draw
	*/
	void onDraw(Renderer *renderer, const Mat4& transform, uint32_t flags);
	void onDraw();
	/*
	update
	*/
	void onUpdate(float dt);
	//
	void CheckAndRemoveEndLifeParticle();
	//
	void AddParticle(PARTICLE_TYPE type, Point Position, std::string name, bool IsGlobal);
	//
	void KillParticle(std::string name, float time = 0.0f);
	//
	Particle* GetParticle(std::string name);

};
#define EffectMgr ParticleManager::GetInstance() 
#endif//_PARTICLE_MANAGER_H_