#pragma once
#include "Init/DisplayView.h"
#include "Init/ParamTexture.h"

#include "GamePlay/OrbitObject.h"
#include "GamePlay/MainCharacterObject.h"

#include "Menu/ResultMenu.h"

#define NUMBER_STAR 3

enum TYPE_GAMEPLAY : int;
enum TYPE_END_RESULT : int;

struct DataXMLReturn
{
	float f;
	Vec2 v;
	DataXMLReturn()
	{
		f = 0;
		v = Vec2(0, 0);
	}
	DataXMLReturn(float f1, Vec2 v1)
	{
		f = f1;
		v = v1;
	}
	DataXMLReturn(float f1)
	{
		f = f1;
		v = Vec2(0, 0);
	}
	DataXMLReturn(Vec2 v1)
	{
		f = 0;
		v = v1;
	}
};

struct AnimateStar
{
	int direct_zoom;
	Vec2 CurrentScale;
	Vec2 MaxScale;
	Vec2 OriginScale;
	float SpeedScale;
	bool IsStarExist;
	AnimateStar()
	{
		direct_zoom = 1;
		CurrentScale = MaxScale = OriginScale = Vec2(0, 0);
		SpeedScale = 0;
		IsStarExist = true;
	}
	
};

class ManageObjectAP
{
private:
	static ManageObjectAP * p_Instance;
	Vector<OrbitObject*>	p_ListOrbits;
	Vector<EntityObject *>  p_ListStar;
	EntityObject *			p_Flag;
	void CalculateThePosOrbit(OrbitObject * previousOr , OrbitObject * t);
	int p_Score;

	SpriteBatchNode * batch;

	float p_distance_quick_move;
	float p_time_move;
	float p_delta_time_move;
	float p_delta_distance_quick_move;

	std::string p_name_explosive_effect;
	std::string p_name_gain_star_effect;

	//count down timer deadth
	float p_time_death_cd;
	float p_delta_time_death_cd;
	float p_origin_time_death_cd;

	int p_stateLoad;
	bool IsInitMainChar;

	float p_CurrentTimeCoutDown;
	float p_OriginTimeCoutDown;

	//star gain in level
	int p_CurrentStarGain;
	AnimateStar p_animateStar[NUMBER_STAR];
	bool p_IsPassLevel;
public:
	static ManageObjectAP * GetInstance()
	{
		if (!p_Instance)
		{
			p_Instance = new ManageObjectAP();
		}
		return p_Instance;
	}
	ManageObjectAP();
	~ManageObjectAP();
	void FreeObjectInAP();
	int InitTheObjectGame(TYPE_GAMEPLAY curGamePlay,bool IsInit = true);
	void UpdateListOrbit(float dt);
	void Update(float dt);
	void OnDraw(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch = NULL);
	void OnVisit(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch = NULL);
	OrbitObject* GetOrbitAtIdx(int idx);
	void CheckCollitionCharWithOrbit();
	void CheckCollitionCharWithStar();
	OrbitObject* CreateOrbit(TYPE_ORBIT typeOrbit = TYPE_ORBIT::ORBIT_DEFAULT, bool IsCalcualtePosOrbit = true);
	OrbitObject* CreateOrbitWithPara(TYPE_ORBIT typeOrbit, ObjectGamePlayDetail * obj);
	void ReloadTheGame();
	void ReleaseTheGame(bool forceResetAllBoost = false);

	void SetScore(int p);
	int GetScore();

	void ResetAll();

	void AddAChildToBatch(Sprite *p)
	{
		batch->addChild(p);
	}
	void RemoveAChildFromBatch(Sprite *p)
	{
		batch->removeChild(p,true);
	}
	//
	int GetIdxOrbitOnScreen();
	//
	void OnQuickMoveOrbit(int idxOrbitMove);
	//
	void OnCheckStatusCharWithGamePlay(TYPE_GAMEPLAY tp, float dt);
	//
	void CalCulateTimeDeath(TYPE_END_RESULT type = TYPE_END_RESULT::RESULT_NONE);
	//
	void OnPassLevel();
	//
	DataXMLReturn GetParamFromDataXML(std::string obj, bool forceGetDesign = false);
	//
	float GetCurrentTimeCD() { return p_CurrentTimeCoutDown; }
	void  SetCurrentTimeCD(float t) { p_CurrentTimeCoutDown = t; }
	//
	void ResetStateLoad() { p_stateLoad = 0; }
	//
	int GetCurrentStarGain() { return p_CurrentStarGain; }
	//
	void setExplosiveAtOrbit(int idx, bool forceCharDead = false);
};

#define OBJAPMgr  ManageObjectAP::GetInstance()