#include "MainSceneGame.h"
#include "GamePlay/APManager.h"
#include "Menu/MenuManager.h"
#include "Utility/BoostManager.h"
#include "Utility/IAPManager.h"
#include "Social/SocialManager.h"
#include "Particle/ParicleManager.h"
#if USE_CC_AUDIO
#include "Utility/SoundManager.h"
#endif
#include "FXFlatform/XFlatform.h" //for the cross flatform wp8/android/ios
MainSceneGame * MainSceneGame::p_Instance = NULL;
int p_loadingStep = 0;
using namespace FXSocialService;
std::mutex mtx;
MainSceneGame::MainSceneGame()
{
	//this->retain();
#if USE_SKY_MENU
	p_deltaTime = 0.0f;
	p_effectUpdateFinish = true;
#endif
}


MainSceneGame::~MainSceneGame()
{
	//this->autorelease();
}

bool MainSceneGame::init()
{
	p_IsInitialized = false;
	//
	Size winsize = cocos2d::Director::getInstance()->getWinSize();
	//
	p_BGLoading = Sprite::create(ResolveNamePath("bg_loading.png"));
	p_BGLoading->setScale(winsize.width / 800.f, winsize.height / 480.f);
	p_BGLoading->setPosition(400.f * winsize.width / 800.f, 240.f* winsize.height / 480.f);
	p_BGLoading->retain();
	//
	p_loadingText = Label::create("Loading...", ResolveNamePath("gamefonts/JandaManateeSolid.ttf"),30);
	p_loadingText->setPosition(646.f * winsize.width / 800.f, (480.f - 445.f)* winsize.height / 480.f);
	p_loadingText->setColor(Color3B::WHITE);
	p_loadingText->retain();
	//
	this->scheduleUpdate();
	return true;
}
//virtual bool update();
// there's no 'id' in cpp, so we recommend returning the class instance pointer
cocos2d::Scene* MainSceneGame::scene()
{
	// 'scene' is an autorelease object
	Scene *scene = Scene::create();
	// 'layer' is an autorelease object
	MainSceneGame *layer = MainSceneGame::create();
	// add layer as a child to scene
	scene->addChild(layer);
	//
	scene->addChild(InputManager::create());
	// return the scene
	return scene;
}

void MainSceneGame::update(float dt)
{
#if CC_REF_LEAK_DETECTION
	Ref::printLeaks();
#endif


	if (!p_IsInitialized)
	{
		if (XFlatform::IsAtLogoState() == true)
		{
			return;
		}
		//
		p_loadingText->setString(std::string("Loading...") + std::to_string(p_loadingStep * 10) + "%");
		if (p_loadingStep == 0)
		{
			InitValue();
		    p_loadingStep++; 
			return;
		}
		//
		if (p_loadingStep == 1)
		{
			//load ark file
			Director::getInstance()->InitModuleRKFile();
			p_loadingStep++;
			return;
		}
		//
		if (p_loadingStep == 2)
		{
			SaveMgr->LoadTheGameSave();
			SaveMgr->LoadTheLevelFromFile();
			p_loadingStep++;
			return;
		}
		//
		if (p_loadingStep == 3)
		{
			LoadDataXML::GetInstance()->OnLoadXMLData(); p_loadingStep++; return;
		}
		//
		if (p_loadingStep == 4)
		{
			ObjectTexture::GetInstance()->Load(); p_loadingStep++; return;
		}
		//
		if (p_loadingStep == 5)
		{
#if USE_SPRITE_STORE
			ObjectTexture::GetInstance()->ParseSpriteToMemory(); p_loadingStep++; return;
#else
			ObjectTexture::GetInstance()->ParseTextureToMemory(); p_loadingStep++; return;
#endif
		}
		if (p_loadingStep == 6)
		{
			//
			p_loadingStep++;
			return;
		}
		//
		if (p_loadingStep == 7)
		{
			int state = MenuMgr->Init();
			if (state == 6)
			{
#if USE_CC_AUDIO
				GetSound->InitSoundEngine();
#endif
				p_loadingStep++; 
			}
			return;
		}

		//
		if (p_loadingStep == 8 )
		{
			if (APMgr->init())
			{
				p_loadingStep++;
			}
			return;
		}
		if (p_loadingStep == 9 )
		{
			BoostMgr->LoadXMLData(); 
			BoostMgr->InitValue(); 
			p_loadingStep++; return;
		}
		//
		if (p_loadingStep == 10 )
		{
			IAPManager::GetInstance()->OnInitialize();
			SocialMgr->InitSocialMgr(); 
			EffectMgr->onInit();
			EffectMgr->AddParticle(PARTICLE_TYPE::P_STAR_FALL, Point(GetWinSize().width / 2 , GetWinSize().height ), "StarFall_Global", true);
			//
#if USE_SKY_MENU
			{
				p_threadUpdateEffect = RKUtils::RKThread_Create("ThreadUpdateEffect",HandleThreadEffect, this);
				RKUtils::RKThread_Start(p_threadUpdateEffect);
			}
#endif
			GetSound->PlayMusicEffect(XMLMgr->GetPathSound("MUSIC_1"));
			//remove unused texture
			TextureCache::getInstance()->removeUnusedTextures();
			TextureCache::getInstance()->removeAllTextures();
			//
			p_loadingStep++; return;
		}

		p_IsInitialized = true;
	}
	else
	{
		SocialMgr->Update(dt);
		if (MenuMgr->IsPopUpAppear())
		{
			MenuMgr->OnUpdatePopUp(dt);
			if (MenuMgr->IsPopUpAppear()) //check 2 times for the program run correct
			{
				return;
			}
		}

		MenuMgr->Update(dt);
		if (MenuMgr->GetCurrentMenuLayer() == MENULAYER::ACTION_PHASE && MenuMgr->IsInActionPhase())
		{
			APMgr->update(dt);
		}
#if USE_SKY_MENU
		if (p_effectUpdateFinish)
		{
				p_deltaTime = dt;
				MenuMgr->CheckAndRemoveBG();
				EffectMgr->CheckAndRemoveEndLifeParticle();
				p_effectUpdateFinish = false;
				RKUtils::RKThread_Start(p_threadUpdateEffect);
		}
		EffectMgr->onUpdate(dt);
#else
		MenuMgr->UpdateBG(dt);
		//
		//update effect in game
		//
		EffectMgr->onUpdate(dt);
#endif
	}
	//SetBackkey(false);
}


void MainSceneGame::OnDraw(const Mat4 &transform, uint32_t flags)
{
	auto renderer = Director::getInstance()->getRenderer();
	if (!p_IsInitialized)
	{
		p_BGLoading->visit(renderer, transform, flags);
		p_loadingText->visit(renderer, transform, flags);
	}
	else
	{
		auto renderer = Director::getInstance()->getRenderer();
#if USE_SKY_MENU
		MenuMgr->VisitBG(renderer, transform, flags);
#endif
		if (MenuMgr->IsInActionPhase())
		{
			APMgr->OnVisit(renderer, transform, flags);
		}
		MenuMgr->Visit(renderer, transform, flags);
		//
		//draw the effect
		//
		EffectMgr->onDraw(renderer, transform, flags);
		//
	}
	renderer = NULL;
}


void MainSceneGame::draw(Renderer *renderer, const Mat4& transform, uint32_t flags)
{
	Node::draw(renderer, transform, flags);
	_customCommand.init(_globalZOrder);
	_customCommand.func = CC_CALLBACK_0(MainSceneGame::OnDraw, this, transform, flags);
	renderer->addCommand(&_customCommand);
	//
	if (!p_IsInitialized)
	{
		p_BGLoading->draw(renderer, transform, flags);
		p_loadingText->draw(renderer, transform, flags);
	}
	else
	{

#if USE_SKY_MENU
		MenuMgr->DrawBG(renderer, transform, flags);
#endif
		if (MenuMgr->IsInActionPhase())
		{
			APMgr->OnDraw(renderer, transform, flags);
		}
		MenuMgr->Draw(renderer, transform, flags);
		//
		//draw the effect
		//
		EffectMgr->onDraw();
		//
	}
}

void MainSceneGame::EndScence()
{
	this->unscheduleUpdate();
}

#if USE_SKY_MENU
uint32 MainSceneGame::HandleThreadEffect(void* pThreadData)
{
	MainSceneGame* This = static_cast<MainSceneGame*>(pThreadData);
	if (This)
	{
		This->p_effectUpdateFinish = false;
		//
		//update background sky in game
		//	
		MenuMgr->UpdateBG(This->p_deltaTime);
		//
		//update effect in game
		//
		//EffectMgr->onUpdate(This->p_deltaTime);
		//
		This->p_effectUpdateFinish = true;
	}
	return 0;
}

#endif