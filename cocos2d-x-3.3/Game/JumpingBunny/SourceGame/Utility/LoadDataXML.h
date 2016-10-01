#ifndef LOAD_DATA_XML_H
#define LOAD_DATA_XML_H
#pragma once
#define USE_TINY_XML2
#include "CCGeometry.h"
#include "CCVector.h"
#ifdef USE_TINY_XML2
#include "../../external/tinyxml2/tinyxml2.h"
#else
#include "../../external/tinyxml/tinyxml.h"
#endif
#include "FileManager.h"
USING_NS_CC;
#ifdef USE_TINY_XML2

#define TiXmlDocument	tinyxml2::XMLDocument
#define TiXmlNode		tinyxml2::XMLNode
#define TiXmlElement	tinyxml2::XMLElement
#endif


enum HARDWARD_PROFILE
{
	PLOW = 0,
	PMED,
	PHIGHT
};

enum TYPE_TEXTURE
{
	OBJECT_TEXTURE,
	MENU_TEXRURE,
	MENU_LAYOUT,
	OBJECT_CHAR,
	OBJECT_EFFECT,
	FONT_TEXTURE
};

struct DetailSize
{
	 int numberRect;
	 std::vector<Rect> Bounds;
	 Size size;
	 Point position;
public:
	 DetailSize():
		 Bounds(0),
		 size(0,0),
		 position(-1,-1)
	 {
	 }
	 virtual ~DetailSize()
	 {
		 Bounds.clear();
	 }
	 bool IsValidPosition()
	 {
		 if (position.x < 0 || position.y < 0)
			 return false;
		 return true;
	 }
};

struct DataObject
{
	 int NumberSprite;
	 std::map<std::string , DetailSize > dDataSize;
	 std::string NameImage;
public:
	DataObject() :
		NumberSprite(0),
		NameImage("")
	{
	}
	virtual ~DataObject()
	{
		dDataSize.clear();
	}
	DetailSize GetDataSize(std::string str)
	{
		return dDataSize.find(str)->second;
	}
	void InsertDataSize(std::string str, DetailSize  d)
	{
		dDataSize.insert(std::pair<std::string, DetailSize >(str, d));
	}
};

struct ResUILayout
{
	std::string source;
	std::string type;
	Point PosRatio;
	Point ScaleRatio;
	float Rotate;
	Point DeltaMove;
	int Tint[4];
	int TintBorder[4];
	float   BorderWid;
	int fontIdx;
public:
	ResUILayout()
	{
		type = "";
		PosRatio = Point(0, 0);
		ScaleRatio = Point(0, 0);
		Rotate = 0;
		Tint[0] = 255; Tint[1] = 255; Tint[2] = 255; Tint[3] = 255;
		TintBorder[0] = 255; TintBorder[1] = 255; TintBorder[2] = 255; TintBorder[3] = 255;
		BorderWid = 0;
		fontIdx = 0;
		source = "";
	}
	~ResUILayout()
	{

	}
};

struct MenuLayout
{
	std::string name;
	float deltaTime;
	Point distanceMoveDir;
	std::map<std::string, ResUILayout*> ListResource;
public:
	MenuLayout()
	{
		name = "";
		distanceMoveDir = Point(0, 0);
		deltaTime = 0;
	}
	~MenuLayout()
	{
		ListResource.clear();
	}
	ResUILayout* GetTheUILayout(std::string str)
	{
		CCASSERT(ListResource.size() > 0, "list resource size must bigger than 0");
		return ListResource.find(str)->second;
	}
	std::string GetSourceData(std::string str)
	{
		CCASSERT(ListResource.size() > 0, "list resource size must bigger than 0");
		return ListResource.find(str)->second->source;
	}
	void InsertUIlayout(std::string str, ResUILayout* re)
	{
		ListResource.insert(std::pair<std::string, ResUILayout*>(str, re));
	}
	int NumberResource()
	{
		return (int)ListResource.size();
	}
	std::map<std::string, ResUILayout*> GetListResource()
	{
		return ListResource;
	}
};

struct OrbitObjectData
{
	DataObject * dataObject;
	int			maxOrbitPframe;
	int			distancePOrbit;
	int			ratio[2];
	float		 speedMove;
	float		 speedMoveUpDown;
	float		 speedRotate;
	Point		 numbnerObjectDeco;
	Point		 numberObjectInter;
	float		 pointAlive;

	int			 numberOrbit;
	int			 numberDeco;
	int			 numberInter;
public:
	OrbitObjectData()
	{
		dataObject = new DataObject();
	}
	~OrbitObjectData()
	{
		delete dataObject;
		dataObject = NULL;
	}
};

struct MainCharData
{
	DataObject * dataObject;
	float p_speedRotate;
	float p_speedMove;
	float p_JumpDuration;
	float p_speedJump;
	float p_ScaleFactor;
	float g_GravityPress;
	float g_GravityDelta;
	std::string pathTextureAnimation;
	std::string pathPlistAnimation;
	std::string pathJsonAnimation;

	float d_time_deadth;
	float d_delta_time_death;
public:
	MainCharData()
	{
		dataObject = new DataObject();

		 p_speedRotate = 0.f;
		 p_speedMove = 0.f;
		 p_JumpDuration = 0.f;
		 p_speedJump = 0.f;
		 p_ScaleFactor = 0.f;
		 g_GravityPress = 0.f;
		 g_GravityDelta = 0.f;

		 pathTextureAnimation = "";
		 pathPlistAnimation = "";
		 pathJsonAnimation = "";

		 d_time_deadth = 0.0f;
		 d_delta_time_death = 0.0f;
	}
	~MainCharData()
	{
		delete dataObject;
		dataObject = NULL;
	}
};

struct MenuItemData
{
	std::map<int,DataObject *> dataObject;
	std::map<std::string, MenuLayout *> dataMenu;
public:
	MenuItemData()
	{
		
	}
	~MenuItemData()
	{
		dataObject.clear();
		dataMenu.clear();
	}

	MenuLayout * GetDataMenu(std::string str)
	{
		return dataMenu.find(str)->second;
	}
};

struct LayerData
{
	Point p_DirMove;
	float p_SpeedMove[2];
	float p_Distance;
	float p_DistanceAlive;
	int	  p_numberObject[2];
	Point p_chanceAppear;
	int	  p_numberIdxSource;
	int   p_IsFullScreen;
	std::vector<std::string> p_listSource;
	std::vector<Point> p_ListScale;
	std::vector<Point> p_ListRatioPosY;
	std::vector<Point> p_ListRatioPosX;
public:
	LayerData()
	{
		p_DirMove = Point(0, 0);
		p_SpeedMove[0] = 0;
		p_SpeedMove[1] = 0;
		p_Distance = 0;
		p_DistanceAlive = 0;
		p_chanceAppear = Point(0, 0);
		p_numberIdxSource = 0;
		p_IsFullScreen = 0;
	}
	~LayerData()
	{
		p_listSource.clear();
		p_ListScale.clear();
		p_ListRatioPosY.clear();
		p_ListRatioPosX.clear();
	}
};

struct LayerBackground
{
	std::map<std::string, LayerData*> p_ListLayer;
	std::string p_Name;
	std::vector<std::string> p_listNamelayerRender;
public:
	LayerBackground()
	{
		p_Name = "";
	}
	~LayerBackground()
	{
		p_ListLayer.clear();
	}

	LayerData * GetLayerData(std::string str)
	{
		return p_ListLayer.find(str)->second;
	}

	void InsertLayerData(std::string str, LayerData * c)
	{
		p_ListLayer.insert(std::pair<std::string, LayerData*>(str, c));
	}
};

struct GameBackground
{
	std::map<std::string, LayerBackground*> p_ListLayer;
public:
	GameBackground() 
	{
	}
	~GameBackground()
	{
		p_ListLayer.clear();
	}

	LayerBackground * GetLayerbackground(std::string str)
	{
		return p_ListLayer.find(str)->second;
	}

	void InsertLayerBackground(std::string str, LayerBackground * c)
	{
		p_ListLayer.insert(std::pair<std::string, LayerBackground*>(str, c));
	}
};

struct EffectData
{
	DataObject * dataObject;
public:
	EffectData()
	{
		dataObject = new DataObject();
	}
	~EffectData()
	{
		delete dataObject;
		dataObject = NULL;
	}
};

struct SoundDetail
{
	std::string Name;
	std::string Type;
	std::string SourcePath;
public:
	SoundDetail() { 
		Name = ""; Type = ""; SourcePath = ""; 
	}
	~SoundDetail(){
	}
};

struct SoundData
{
private:
	std::map<std::string, SoundDetail*> p_ListSoundFX;
public:
	SoundDetail* GetSoundFX(std::string name){ return p_ListSoundFX.find(name)->second; }
	void InsertSoundFX(std::string name, SoundDetail* data){ p_ListSoundFX.insert(std::pair<std::string, SoundDetail*>(name, data)); }
};

struct ObjectGamePlayDetail
{
	int id;
	std::string source;
	//std::string name;
	Vec2 pos;
	float ratio_scale;
	//orbit
	float speed_rotate;
	float speed_move;
	int orbit_type;
	Vec2 direct_move;
	float distance_move;
	Vec2 point_rotate;
	float speed_rotate_point;
	int stick_idx_orbit;
	//star
	float delta_scale;
	float max_scale;

	ObjectGamePlayDetail()
	{
		id = 0;
		source = "";
		pos = Vec2(0, 0);
		ratio_scale = 1;
		//
		speed_rotate = 0;
		speed_move = 0;
		orbit_type = 0;
		direct_move = Vec2(0, 0);
		distance_move = 0;
		point_rotate = Vec2(0, 0);
		speed_rotate_point = 0;
		//
		delta_scale = 0.f;
		max_scale = 1.f;
		//
		stick_idx_orbit = -1;
	}

};

struct CharacterGamePlayDetail
{
	float speed_rotate;
	float speed_jump;
	float scale_ratio;
	float speed_move;
	float gravity_press;
	float gravity_press_delta;

	CharacterGamePlayDetail()
	{
		speed_rotate = 0;
		speed_jump = 0;
		scale_ratio = 0;
		speed_move = 0;
		gravity_press = 0;
		gravity_press_delta = 0;
	}
};

struct GamePlayDetail
{
	float Time_CD;
	int ID;
	std::string Dec;
	int numberOrbit;
	//std::string name;
	std::map<std::string, ObjectGamePlayDetail*> p_ListObjectDetail;
	CharacterGamePlayDetail * p_PlayerDetail;


	GamePlayDetail()
	{
		Time_CD = 0;
		ID = 0;
		Dec = "";
		numberOrbit = 0;
		p_PlayerDetail = new CharacterGamePlayDetail();
	}

	ObjectGamePlayDetail* GetObjectDetail(std::string name)
	{
		auto it = p_ListObjectDetail.find(name);
		if (it != p_ListObjectDetail.end())
		{
			return it->second;
		}
		return NULL;
	}

	CharacterGamePlayDetail * GetPlayerDetailAtLevel()
	{
		return p_PlayerDetail;
	}

	void InsertObjectDetail(std::string name, ObjectGamePlayDetail * obj)
	{
		p_ListObjectDetail.insert(std::pair<std::string, ObjectGamePlayDetail*>(name, obj));
	}

	float GetTimeCoutDown()
	{
		return Time_CD;
	}


};

struct GamePlayManager
{
	std::map<std::string, GamePlayDetail*> p_ListGamePlay;

	GamePlayDetail* GetGamePlay(std::string name)
	{
		auto it = p_ListGamePlay.find(name);
		if (it != p_ListGamePlay.end())
		{
			return it->second;
		}
		return NULL;
	}

	CharacterGamePlayDetail* GetCharDetailAtLevel(std::string name)
	{
		auto it = p_ListGamePlay.find(name);
		if (it != p_ListGamePlay.end())
		{
			return it->second->GetPlayerDetailAtLevel();
		}
		return NULL;
	}


	void InsertGamePlay(std::string name, GamePlayDetail * obj)
	{
		p_ListGamePlay.insert(std::pair<std::string, GamePlayDetail*>(name, obj));
	}
};

class LoadDataXML
{
private:
	TiXmlDocument*				 p_ObjectXML;
	TiXmlDocument*				 p_MenuXML;
	static LoadDataXML *		 p_Instance;
	OrbitObjectData *			 p_orbitData;
	MainCharData	*			 p_mainChar;
	MenuItemData	*		     p_uiData;
	GameBackground  *			 p_GameBG;
	EffectData		*			 p_EffectData;
	HARDWARD_PROFILE			 p_HProfile;
	SoundData		*			 p_SoundData;

	GamePlayManager *			 p_GamePlayMgr;

public:

	static LoadDataXML *		 GetInstance()
	{
		if (!p_Instance)
		{
			p_Instance = new LoadDataXML();
		}
		return p_Instance;
	}
	LoadDataXML();
	~LoadDataXML();
	void OnLoadXMLData();
	void LoadObjectEntityData();
	void LoadMenuItemData();

	void ParseTheSpecified(TiXmlNode * node, DataObject * &);

	DataObject * GetDataObject(TYPE_TEXTURE t)
	{
		if (t == TYPE_TEXTURE::OBJECT_TEXTURE)
		{
			return p_orbitData->dataObject;
		}
		else if (t == TYPE_TEXTURE::MENU_TEXRURE)
		{
			return p_uiData->dataObject.find(0)->second;
		}
		else if (t == TYPE_TEXTURE::MENU_LAYOUT)
		{
			return p_uiData->dataObject.find(1)->second;
		}
		else if (t == TYPE_TEXTURE::OBJECT_CHAR)
		{
			return p_mainChar->dataObject;
		}
		else if (t == TYPE_TEXTURE::OBJECT_EFFECT)
		{
			return p_EffectData->dataObject;
		}
		else if (t == TYPE_TEXTURE::FONT_TEXTURE)
		{
			return p_uiData->dataObject.find(2)->second;
		}
		return 0;
	}

	OrbitObjectData * GetOrbitData()
	{
		return p_orbitData;
	}
	MainCharData * GetMainCharData()
	{
		return p_mainChar;
	}
	MenuItemData * GetUIData()
	{
		return p_uiData;
	}
	GameBackground * GetGameBG()
	{
		return p_GameBG;
	}
	EffectData * GetEffectData()
	{
		return p_EffectData;
	}
	GamePlayManager * GetGamePlayMgr()
	{
		return p_GamePlayMgr;
	}
	//
	int numberResourceUI(std::string name_ui)
	{
		return p_uiData->GetDataMenu(name_ui)->NumberResource();
	}
	std::map<std::string, ResUILayout*> GetListResourceUI(std::string name_ui)
	{
		return p_uiData->GetDataMenu(name_ui)->GetListResource();
	}

	HARDWARD_PROFILE GetHarwardProfile()
	{
		return p_HProfile;
	}

	SoundData * GetSoundData()
	{
		return p_SoundData;
	}

	std::string GetPathSound(std::string name)
	{
		return p_SoundData->GetSoundFX(name)->SourcePath;
	}
};

#define XMLMgr LoadDataXML::GetInstance()
#endif
