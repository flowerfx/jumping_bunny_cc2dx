#include "LoadDataXML.h"
#include "Init/DisplayView.h"
#include "Init/ParamTexture.h"
std::vector<std::string> split(const std::string& s, char seperator)
{
	std::vector<std::string> output;

	std::string::size_type prev_pos = 0, pos = 0;

	while ((pos = s.find(seperator, pos)) != std::string::npos)
	{
		std::string substring(s.substr(prev_pos, pos - prev_pos));

		output.push_back(substring);

		prev_pos = ++pos;
	}

	output.push_back(s.substr(prev_pos, pos - prev_pos)); // Last word

	return output;
}


LoadDataXML *		 LoadDataXML::p_Instance = NULL;
LoadDataXML::LoadDataXML()
{
	p_HProfile = PHIGHT;
	if (IsDeviceLowMem())
	{
		p_HProfile = PLOW;
	}
	else
	{
		if (GetCpuCoreCount() <= 2)
		{
			p_HProfile = PMED;
		}
	}
    p_ObjectXML = NULL;
    p_MenuXML = NULL;
    p_orbitData = NULL;
    p_uiData = NULL;
    p_mainChar = NULL;
    p_EffectData = NULL;
    p_SoundData = NULL;
	p_GamePlayMgr = NULL;

}

LoadDataXML::~LoadDataXML()
{
	delete p_ObjectXML;
	p_ObjectXML = NULL;
	delete p_MenuXML;
	p_MenuXML = NULL;
	delete p_orbitData;
	p_orbitData = NULL;
	delete p_uiData;
	p_uiData = NULL;
	delete p_mainChar;
	p_mainChar = NULL;
	delete p_EffectData;
	p_EffectData = NULL;
	delete p_SoundData;
	p_SoundData = NULL;
	delete p_GamePlayMgr;
	p_GamePlayMgr = NULL;
}
void LoadDataXML::OnLoadXMLData()
{
	bool p_IsUseArkXML = false;
#ifdef USE_ARK_ARCHIVE
	p_IsUseArkXML = true;
#endif
	if (p_ObjectXML == NULL)
	{
		p_ObjectXML = new TiXmlDocument();
	}

	if (p_IsUseArkXML)
	{
		p_ObjectXML->LoadFileArk(std::string(XML_DIR) + std::string(OBJECT_XML));
		LoadObjectEntityData();
	}
	else
	{
		FILE * f = GetFile((std::string(XML_DIR) + std::string(OBJECT_XML)).c_str(), "rb");
		if (f)
		{
			p_ObjectXML->LoadFile(f);
			LoadObjectEntityData();
			fclose(f);
		}
	}
	//
	if (p_MenuXML == NULL)
	{
		p_MenuXML = new TiXmlDocument();
	}

	if (p_IsUseArkXML)
	{
		p_MenuXML->LoadFileArk(std::string(XML_DIR) + std::string(UI_XML));
		LoadMenuItemData();
	}
	else
	{
		FILE * f = GetFile((std::string(XML_DIR) + std::string(UI_XML)).c_str(), "rb");
		if (f)
		{
			p_MenuXML->LoadFile(f);
			LoadMenuItemData();
			fclose(f);
		}
	}

}
void LoadDataXML::ParseTheSpecified(TiXmlNode * node, DataObject * &d)
{
	if (!node)
		return;
	int num = atoi(node->ToElement()->Attribute("number"));
	if (num <= 0)
		return;
	TiXmlElement* rect = node->FirstChild()->ToElement();
	std::string str = "";
	do
	{
		DetailSize rec;
		if (!rect)
			continue;
		str = rect->Attribute("string");
		int numberVal = atoi(rect->Attribute("num"));
		TiXmlElement* val = rect->FirstChildElement("value")->ToElement();
		if (!val)
			continue;
		rec.numberRect = numberVal;
		for (int j = 0; j < numberVal; ++j)
		{
			if (!val)
				continue;
			auto detailRec = split(val->Attribute("val"), '-');
			CCASSERT(detailRec.size() == 4, "the detail rectangle must be 4");
			rec.Bounds.push_back(Rect(
				(float)atoi(detailRec[0].c_str()), 
				(float)atoi(detailRec[1].c_str()), 
				(float)atoi(detailRec[2].c_str()), 
				(float)atoi(detailRec[3].c_str())
				));
			val = val->NextSiblingElement();
		}
		rec.size = Size(rec.Bounds.at(0).getMaxX() - rec.Bounds.at(0).getMinX(), rec.Bounds.at(0).getMaxY() - rec.Bounds.at(0).getMinY());
		d->InsertDataSize(str, rec);
		rect = rect->NextSiblingElement();
	} while (rect);
}

void LoadDataXML::LoadObjectEntityData()
{
	p_orbitData = new OrbitObjectData();

	auto Root = p_ObjectXML->FirstChildElement("GAME");
	if (!Root)
		return;

	auto rootObject = Root->FirstChildElement("Object");
	if (!rootObject)
		return;

	auto OrbitObject = rootObject->FirstChildElement("Orbit");
	if (OrbitObject)
	{
		//p_orbitData->dataObject->NameImage = OrbitObject->FirstChildElement("TextureName")->GetText();
#if defined WP8 || defined W8
		auto raw_name = ChangeExtensionName(OrbitObject->FirstChildElement("TextureName")->GetText(), ".png");
#else
		auto raw_name = ChangeExtensionName(OrbitObject->FirstChildElement("TextureName")->GetText(), ".png");
#endif
		p_orbitData->dataObject->NameImage = raw_name;

		auto orSize = OrbitObject->FirstChildElement("orbitSize");
		if (orSize)
		{
			p_orbitData->maxOrbitPframe = atoi(orSize->Attribute("numperframe"));
			p_orbitData->distancePOrbit = atoi(orSize->Attribute("distance")) * GetScaleFactor();
			p_orbitData->ratio[0] = atoi(orSize->Attribute("minRatio"));
			p_orbitData->ratio[1] = atoi(orSize->Attribute("maxRatio"));
		}
		p_orbitData->speedRotate = atof(OrbitObject->FirstChildElement("SpeedRotate")->GetText()) ; // this not need multipli with scalefactor because it's radian
		p_orbitData->speedMove = atof(OrbitObject->FirstChildElement("SpeedMove")->GetText()) * GetScaleFactor();
		p_orbitData->speedMoveUpDown = atof(OrbitObject->FirstChildElement("SpeedMoveUpDown")->GetText()) * GetScaleFactor();
		auto ratio = split(OrbitObject->FirstChildElement("MaxObjectDec")->GetText(), '_');
		p_orbitData->numbnerObjectDeco = Point(atof(ratio[0].c_str()), atof(ratio[1].c_str())) * GetScaleFactor();
		ratio.clear();
		ratio = split(OrbitObject->FirstChildElement("MaxObjectInter")->GetText(), '_');
		p_orbitData->numberObjectInter = Point(atof(ratio[0].c_str()), atof(ratio[1].c_str())) * GetScaleFactor();
		ratio.clear();
		p_orbitData->pointAlive = atof(OrbitObject->FirstChildElement("pointAlive")->GetText());
		auto object = OrbitObject->FirstChildElement("OrbitObject");
		if (object)
		{
			p_orbitData->numberOrbit = atoi(object->ToElement()->Attribute("number"));
			ParseTheSpecified(object, p_orbitData->dataObject);
		}
		object = OrbitObject->FirstChildElement("DecoObject");
		if (object)
		{
			p_orbitData->numberDeco = atoi(object->ToElement()->Attribute("number"));
			ParseTheSpecified(object, p_orbitData->dataObject);
		}
		object = OrbitObject->FirstChildElement("InterObject");
		if (object)
		{
			p_orbitData->numberInter = atoi(object->ToElement()->Attribute("number"));
			ParseTheSpecified(object, p_orbitData->dataObject);
		}
	}

	p_mainChar = new MainCharData();
	auto objectMainChar = rootObject->FirstChildElement("Mainchar");
	if (objectMainChar)
	{
		auto Val = objectMainChar->FirstChildElement("texture");
		if (Val)
		{
#if defined WP8 || defined W8
			auto raw_name = ChangeExtensionName(Val->Attribute("path"), ".png");
#else
			auto raw_name = ChangeExtensionName(Val->Attribute("path"), ".png");
#endif
			p_mainChar->dataObject->NameImage = raw_name;
			p_mainChar->dataObject->NumberSprite = atoi(Val->Attribute("number"));
			auto rect = Val->FirstChildElement("Rect");
			do
			{
				if (rect)
				{
					auto dataRec = split(rect->Attribute("val"), '-');
					DetailSize rec;
					rec.Bounds.push_back(Rect(
						(float)atoi(dataRec[0].c_str()),
						(float)atoi(dataRec[1].c_str()),
						(float)atoi(dataRec[2].c_str()),
						(float)atoi(dataRec[3].c_str())
						));
					rec.size = Size(rec.Bounds.at(0).getMaxX() - rec.Bounds.at(0).getMinX(), rec.Bounds.at(0).getMaxY() - rec.Bounds.at(0).getMinY());
					p_mainChar->dataObject->InsertDataSize(rect->Attribute("string"), rec);
					rect = rect->NextSiblingElement();
				}

			} while (rect);			
		}

		Val = objectMainChar->FirstChildElement("Animation");
		if (Val)
		{
			std::string location = Val->Attribute("path");
			auto name = Val->FirstChildElement("texture");
			p_mainChar->pathTextureAnimation = location + name->Attribute("path");
			name = Val->FirstChildElement("plist");
			p_mainChar->pathPlistAnimation = location + name->Attribute("path");
			name = Val->FirstChildElement("json");
			p_mainChar->pathJsonAnimation = location + name->Attribute("path");

		}

		Val = objectMainChar->FirstChildElement("valueMain");
		if (Val)
		{
			p_mainChar->p_speedRotate = atof(Val->Attribute("speedRotate"));
			p_mainChar->p_JumpDuration = atof(Val->Attribute("JumpDuration"));
			p_mainChar->p_speedJump = atof(Val->Attribute("speedJump"));
			p_mainChar->p_ScaleFactor = atof(Val->Attribute("scaleRatio"));
			p_mainChar->p_speedMove = atof(Val->Attribute("speedMove"));

			p_mainChar->d_time_deadth = atof(Val->Attribute("timedeath"));
			p_mainChar->d_delta_time_death = atof(Val->Attribute("deltatimedeath"));
		}
		Val = objectMainChar->FirstChildElement("valueGravity");
		if (Val)
		{
			p_mainChar->g_GravityPress = atof(Val->Attribute("gravityPress"));
			p_mainChar->g_GravityDelta = atof(Val->Attribute("gravityDelta"));
		}
	}

	p_EffectData = new EffectData();
	auto rootObJectEffect = rootObject->FirstChildElement("Effects");
	if (rootObJectEffect)
	{
		auto Val = rootObJectEffect->FirstChildElement("texture");
		if (Val)
		{
#if defined WP8 || defined W8
			auto raw_name = ChangeExtensionName(Val->Attribute("path"), ".png");
#else
			auto raw_name = ChangeExtensionName(Val->Attribute("path"), ".png");
#endif
			p_EffectData->dataObject->NameImage = raw_name;
			p_EffectData->dataObject->NumberSprite = atoi(Val->Attribute("number"));
			auto rect = Val->FirstChildElement("Rect");
			do
			{
				if (rect)
				{
					auto dataRec = split(rect->Attribute("val"), '-');
					DetailSize rec;
					rec.Bounds.push_back(Rect(
						(float)atoi(dataRec[0].c_str()),
						(float)atoi(dataRec[1].c_str()),
						(float)atoi(dataRec[2].c_str()),
						(float)atoi(dataRec[3].c_str())
						));
					rec.size = Size(rec.Bounds.at(0).getMaxX() - rec.Bounds.at(0).getMinX(), rec.Bounds.at(0).getMaxY() - rec.Bounds.at(0).getMinY());
					p_EffectData->dataObject->InsertDataSize(rect->Attribute("string"), rec);
					rect = rect->NextSiblingElement();
				}

			} while (rect);
		}
	}
	//
	auto rootGamePlay = Root->FirstChildElement("GamePlay");
	if (rootGamePlay)
	{
		p_GamePlayMgr = new GamePlayManager();
		//auto gpDetail = new  GamePlayDetail();		
		auto gameplayDetail = rootGamePlay->FirstChildElement("gameplay");
		do
		{
			if (gameplayDetail)
			{			
				auto gpDetail = new  GamePlayDetail();
				gpDetail->Dec = gameplayDetail->Attribute("des");
				gpDetail->Time_CD = atof(gameplayDetail->Attribute("time"));
				gpDetail->ID = atoi(gameplayDetail->Attribute("id"));
				gpDetail->numberOrbit = 0;
				std::string namegameplay = gameplayDetail->Attribute("name");
				//
				auto PlayerDetail = gameplayDetail->FirstChildElement("valueMain");
				if (PlayerDetail)
				{
					gpDetail->p_PlayerDetail->speed_rotate = atof(PlayerDetail->Attribute("speedRotate"));
					gpDetail->p_PlayerDetail->speed_jump = atof(PlayerDetail->Attribute("speedJump"));
					gpDetail->p_PlayerDetail->scale_ratio = atof(PlayerDetail->Attribute("scaleRatio"));
					gpDetail->p_PlayerDetail->speed_move = atof(PlayerDetail->Attribute("speedMove"));
					gpDetail->p_PlayerDetail->gravity_press = atof(PlayerDetail->Attribute("gravityPress"));
					gpDetail->p_PlayerDetail->gravity_press_delta = atof(PlayerDetail->Attribute("gravityDelta"));
				}
				//
				auto objectDetail = gameplayDetail->FirstChildElement("obj");
				do
				{
					if (objectDetail)
					{
						auto objDetail = new ObjectGamePlayDetail();
						objDetail->id = atoi(objectDetail->Attribute("o_id"));
						objDetail->source = (objectDetail->Attribute("source"));
						std::string nameobj = objectDetail->Attribute("name");
						if (objDetail->source == "orbit")
						{
							gpDetail->numberOrbit++;

							objDetail->ratio_scale = atof(objectDetail->Attribute("ratioscale"));
							//
							auto valPos = split(objectDetail->Attribute("pos"), '_');
							//
							objDetail->pos.x = atof(valPos[0].c_str());
							objDetail->pos.y = atof(valPos[1].c_str());
							//
							objDetail->speed_rotate = atof(objectDetail->Attribute("SpeedRotate"));
							objDetail->speed_move = atof(objectDetail->Attribute("SpeedMove"));
							objDetail->orbit_type = atoi(objectDetail->Attribute("OrbitType"));
							//
							valPos = split(objectDetail->Attribute("DirectMove"), '_');
							objDetail->direct_move.x = atof(valPos[0].c_str());
							objDetail->direct_move.y = atof(valPos[1].c_str());
							objDetail->distance_move = atof(valPos[2].c_str());
							//
							valPos = split(objectDetail->Attribute("rotate_around_point"), '_');
							objDetail->point_rotate.x = atof(valPos[0].c_str());
							objDetail->point_rotate.y = atof(valPos[1].c_str());
							objDetail->speed_rotate_point = atof(valPos[2].c_str());
							//
							if (objectDetail->Attribute("stickOrbitIdx"))
							{
								objDetail->stick_idx_orbit = atoi(objectDetail->Attribute("stickOrbitIdx"));
							}
						}
						else if (objDetail->source == "star" || objDetail->source == "flag")
						{
							objDetail->ratio_scale = atof(objectDetail->Attribute("ratioscale"));
							//
							auto valPos = split(objectDetail->Attribute("pos"), '_');
							//
							objDetail->pos.x = atof(valPos[0].c_str());
							objDetail->pos.y = atof(valPos[1].c_str());
							//
							if (objDetail->source == "star")
							{
								objDetail->delta_scale = atof(objectDetail->Attribute("delta_speed_scale"));
								objDetail->max_scale = atof(objectDetail->Attribute("max_scale"));
							}
						}
						gpDetail->InsertObjectDetail(nameobj, objDetail);
						objectDetail = objectDetail->NextSiblingElement();
					}
				} while (objectDetail);

				p_GamePlayMgr->InsertGamePlay(namegameplay, gpDetail);
				gameplayDetail = gameplayDetail->NextSiblingElement();
			}
		} while (gameplayDetail);
	}
	//delete Root;
	Root = NULL;

}
void LoadDataXML::LoadMenuItemData()
{
	p_uiData = new MenuItemData();

	auto Root = p_MenuXML->FirstChildElement("GAME");
	if (!Root)
		return;

	auto rootTexture = Root->FirstChildElement("texture");
	if (rootTexture)
	{
		int numberTe = atoi(rootTexture->Attribute("number"));
		for (int i = 0; i < numberTe; i++)
		{
			auto dataObject = new DataObject();
			std::string val = std::string("tex") + std::to_string(i);
			auto tex = rootTexture->FirstChildElement(val.c_str());
			if (tex)
			{
#if defined WP8 || defined W8
				auto raw_name = ChangeExtensionName(tex->ToElement()->Attribute("namepath"),".png");
				dataObject->NameImage = raw_name;
#else
				auto raw_name = ChangeExtensionName(tex->ToElement()->Attribute("namepath"), ".png");
				dataObject->NameImage = raw_name;
#endif
				auto button = tex->ToElement()->FirstChildElement("button")->ToElement();
				do
				{
					DetailSize	 rec;
					if (button)
					{
						auto detailRec = split(button->Attribute("rect"), '-');

						CCASSERT(detailRec.size() == 4,"the detail rectangle must be 4");

						rec.Bounds.push_back(Rect(
							(float)atoi(detailRec[0].c_str()),
							(float)atoi(detailRec[1].c_str()),
							(float)atoi(detailRec[2].c_str()),
							(float)atoi(detailRec[3].c_str())
							));
						rec.size = Size(rec.Bounds.at(0).getMaxX() - rec.Bounds.at(0).getMinX(), rec.Bounds.at(0).getMaxY() - rec.Bounds.at(0).getMinY());
						std::string nameres = button->Attribute("name");
						dataObject->InsertDataSize(nameres, rec);
						dataObject->NumberSprite++;
						button = button->NextSiblingElement();
					}

				} while (button);
				//push the the global data
				p_uiData->dataObject.insert(std::pair<int, DataObject*>(i, dataObject));
							
			}
		}
	}

	auto rootMenu = Root->FirstChildElement("menu");
	if (rootMenu)
	{
		auto UI = rootMenu->FirstChildElement("UI")->ToElement();
		do
		{
			auto menuLayout = new MenuLayout();
			if (UI)
			{
				menuLayout->name = UI->Attribute("name");
				menuLayout->deltaTime = atof(UI->Attribute("deltatime"));
				auto ra = split(UI->Attribute("distanceMoveDir"), '_');
				menuLayout->distanceMoveDir = Point(atof(ra[0].c_str()), atof(ra[1].c_str()));
				auto res = UI->FirstChildElement("resource")->ToElement();
				do
				{
					auto UIlayout = new ResUILayout();
					if (res)
					{
						UIlayout->type = res->Attribute("typeRes");
						UIlayout->source = res->Attribute("source");
						auto Ratio = split(res->Attribute("pos"),'_');
						UIlayout->PosRatio = Point(atof(Ratio[0].c_str()), atof(Ratio[1].c_str()));
						Ratio = split(res->Attribute("scale"), '_');
						UIlayout->ScaleRatio = Point(atof(Ratio[0].c_str()), atof(Ratio[1].c_str()));
						if (UIlayout->type == std::string("Button"))
						{
							UIlayout->ScaleRatio = UIlayout->ScaleRatio * GetScaleFactor();
							UIlayout->ScaleRatio *= 2; //chien note this for the 1024x1024 pixel
						}
						else if (UIlayout->type == std::string("font"))
						{
							Ratio = split(res->Attribute("color"),'_');
							UIlayout->Tint[0] = atoi(Ratio[0].c_str());
							UIlayout->Tint[1] = atoi(Ratio[1].c_str());
							UIlayout->Tint[2] = atoi(Ratio[2].c_str());
							UIlayout->Tint[3] = atoi(Ratio[3].c_str());
							Ratio = split(res->Attribute("colorborder"), '_');
							UIlayout->TintBorder[0] = atoi(Ratio[0].c_str());
							UIlayout->TintBorder[1] = atoi(Ratio[1].c_str());
							UIlayout->TintBorder[2] = atoi(Ratio[2].c_str());
							UIlayout->TintBorder[3] = atoi(Ratio[3].c_str());
							UIlayout->BorderWid = atof(res->Attribute("borderwd"));
							UIlayout->fontIdx = atoi(res->Attribute("fontIdx"));
						}
						UIlayout->Rotate = atof(res->Attribute("rotate"));
						Ratio = split(res->Attribute("dirmove"), '_');
						UIlayout->DeltaMove = Point(atof(Ratio[0].c_str()), atof(Ratio[1].c_str()));
						if (UIlayout->DeltaMove.x == 0)
						{
#if USE_DX_COR
							UIlayout->DeltaMove.y = -UIlayout->DeltaMove.y;
#endif
						}
						menuLayout->InsertUIlayout(res->Attribute("name"), UIlayout);
						res = res->NextSiblingElement();
					}			
				} while (res);

				p_uiData->dataMenu.insert(std::pair<std::string, MenuLayout *>(UI->Attribute("name"), menuLayout));

				UI = UI->NextSiblingElement();
			}

		} while (UI);

	}

	p_GameBG = new GameBackground();

	auto rootBackground = Root->FirstChildElement("background");
	if (!rootBackground)
	{
		CCASSERT(rootBackground, "ERROR ! the root background have null value !");
		return;
	}

	auto rootlayer = rootBackground->FirstChildElement("Layer")->ToElement();
	do
	{
		if (rootlayer)
		{
			auto valLayer = new LayerBackground();
			valLayer->p_Name = rootlayer->Attribute("name");
			auto listname = split(rootlayer->Attribute("listRender"), ',');
			for (size_t i = 0; i < listname.size(); i++)
			{
				valLayer->p_listNamelayerRender.push_back(listname[i]);
			}
			auto layer = rootlayer->FirstChildElement("layer")->ToElement();
			do
			{
				if (layer)
				{
					auto val = new LayerData();
					auto ratio = split(layer->Attribute("dirmove"), '_');
					val->p_DirMove = Point(atof(ratio[0].c_str()), atof(ratio[1].c_str()));
					ratio.clear();
					ratio = split(layer->Attribute("chanceAppear"), '_');
					val->p_chanceAppear = Point(atof(ratio[0].c_str()), atof(ratio[1].c_str()));
					ratio.clear();
					ratio = split(layer->Attribute("numberobject"), '_');
					val->p_numberObject[0] = atof(ratio[0].c_str());
					val->p_numberObject[1] = atof(ratio[1].c_str());
					//
					if (p_HProfile == PLOW && atof(ratio[1].c_str()) > 3 && atof(ratio[0].c_str())> 3)
					{
						val->p_numberObject[0] = val->p_numberObject[0] / 2;
						val->p_numberObject[1] = val->p_numberObject[1] / 2;
					}
					//
					ratio.clear();
					val->p_Distance = atof(layer->Attribute("distance"));
					ratio = split(layer->Attribute("speedMove"), '_');
					val->p_SpeedMove[0] = atof(ratio[0].c_str());
					val->p_SpeedMove[1] = atof(ratio[1].c_str());
					ratio.clear();
					val->p_DistanceAlive = atof(layer->Attribute("distanceAlive"));
					val->p_numberIdxSource = atoi(layer->Attribute("numberIdxSource"));
					ratio = split(layer->Attribute("source"), ',');
					for (size_t i = 0; i < ratio.size(); i++)
					{
						val->p_listSource.push_back(ratio[i].c_str());
					}
					ratio.clear();
					ratio = split(layer->Attribute("ratioScale"), ',');
					for (size_t i = 0; i < ratio.size(); i++)
					{
						auto ra = split(ratio[i].c_str(), '_');
						Point p = Point(atof(ra[0].c_str()), atof(ra[1].c_str()));
						val->p_ListScale.push_back(p);
					}
					ratio.clear();
					ratio = split(layer->Attribute("ratioPosY"), ',');
					for (size_t i = 0; i < ratio.size(); i++)
					{
						auto ra = split(ratio[i].c_str(), '_');
						Point p = Point(atof(ra[0].c_str()), atof(ra[1].c_str()));
						auto t = MIN(p.x, p.y);
						if (t > 0.5f) { p.y += GetOffsetSCale(); p.x += GetOffsetSCale(); }
						val->p_ListRatioPosY.push_back(p);
					}
					ratio.clear();
					ratio = split(layer->Attribute("ratioPosX"), ',');
					for (size_t i = 0; i < ratio.size(); i++)
					{
						auto ra = split(ratio[i].c_str(), '_');
						Point p = Point(atof(ra[0].c_str()), atof(ra[1].c_str()));
						val->p_ListRatioPosX.push_back(p);
					}
					ratio.clear();
					val->p_IsFullScreen = atoi(layer->Attribute("fullscreen"));
					valLayer->InsertLayerData(layer->Attribute("name"), val);
				}


				layer = layer->NextSiblingElement();

			} while (layer);

			p_GameBG->InsertLayerBackground(valLayer->p_Name, valLayer);
		}

		rootlayer = rootlayer->NextSiblingElement();
	} while (rootlayer);

	p_SoundData = new SoundData();
	auto rootSound = Root->FirstChildElement("sound");
	if (rootSound)
	{
		auto soundDetail = rootSound->FirstChildElement("detail")->ToElement();
		do
		{
			if (soundDetail)
			{
				SoundDetail * vari = new SoundDetail();
				vari->Type = soundDetail->Attribute("type");
				vari->Name = soundDetail->Attribute("name");
				vari->SourcePath = soundDetail->Attribute("source");

				p_SoundData->InsertSoundFX(vari->Name, vari);
			}
			soundDetail = soundDetail->NextSiblingElement();
		} while (soundDetail);
	}
	//delete Root;
	Root = NULL;
}
