
#include "InputManager.h"
#include "Init/AppMacros.h"
InputManager * InputManager::m_Instace = NULL;
	std::map<CONTROL_TYPE, bool> m_StatusList;
	
	Scene* InputManager::createScene()
	{
		auto scene = Scene::create();

		InputManager* layer = InputManager::create();
		scene->addChild(layer);
		return scene;
	}

	void InputManager::update(float dt)
	{
		if (std::find(m_Instace->heldKeys.begin(), m_Instace->heldKeys.end(), EventKeyboard::KeyCode::KEY_ESCAPE) != m_Instace->heldKeys.end()){
			m_Instace->m_currentControl = CONTROL_TYPE::BACK_KEY;
		}
		else if (std::find(m_Instace->heldKeys.begin(), m_Instace->heldKeys.end(), EventKeyboard::KeyCode::KEY_SPACE) != m_Instace->heldKeys.end()){
			m_Instace->m_currentControl = CONTROL_TYPE::JUMP;
		}
		else if (std::find(m_Instace->heldKeys.begin(), m_Instace->heldKeys.end(), EventKeyboard::KeyCode::KEY_RIGHT_ARROW) != m_Instace->heldKeys.end()){
			m_Instace->m_currentControl = CONTROL_TYPE::MOVE_FORWARD;
		}
		else if (std::find(m_Instace->heldKeys.begin(), m_Instace->heldKeys.end(), EventKeyboard::KeyCode::KEY_LEFT_ARROW) != m_Instace->heldKeys.end()){
			m_Instace->m_currentControl = CONTROL_TYPE::MOVE_BACKWARD;
		}
		else if (std::find(m_Instace->heldKeys.begin(), m_Instace->heldKeys.end(), EventKeyboard::KeyCode::KEY_DELETE) != m_Instace->heldKeys.end()){
			m_Instace->m_currentControl = CONTROL_TYPE::CANCEL_SELECT;
		}
		else if (std::find(m_Instace->heldKeys.begin(), m_Instace->heldKeys.end(), EventKeyboard::KeyCode::KEY_UP_ARROW) != m_Instace->heldKeys.end()){
			m_Instace->m_currentControl = CONTROL_TYPE::ROTATE_FORWARD;
		}
		else if (std::find(m_Instace->heldKeys.begin(), m_Instace->heldKeys.end(), EventKeyboard::KeyCode::KEY_DOWN_ARROW) != m_Instace->heldKeys.end()){
			m_Instace->m_currentControl = CONTROL_TYPE::ROTATE_BACKWARD;
		}
		else if (std::find(m_Instace->heldKeys.begin(), m_Instace->heldKeys.end(), EventKeyboard::KeyCode::KEY_R) != m_Instace->heldKeys.end()){
			m_Instace->m_currentControl = CONTROL_TYPE::TOGGLE_ROTATE;
		}
		else
		{
			m_Instace->m_currentControl = CONTROL_TYPE::NONE;
		}
	}
	bool InputManager::init()
	{
		if (!Layer::init())
		{
			return false;
		}
		m_Instace = new InputManager();
		auto listener_key = EventListenerKeyboard::create();
		listener_key->onKeyPressed = CC_CALLBACK_2(InputManager::onKeyPressed, this);
		listener_key->onKeyReleased = CC_CALLBACK_2(InputManager::onKeyReleased, this);

		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener_key, this);

		auto listener = EventListenerTouchAllAtOnce::create();
		listener->onTouchesBegan = CC_CALLBACK_2(InputManager::onTouchesBegan, this);
		listener->onTouchesMoved = CC_CALLBACK_2(InputManager::onTouchesMoved, this);
		listener->onTouchesEnded = CC_CALLBACK_2(InputManager::onTouchesEnded, this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

		this->scheduleUpdate();

		m_Instace->p_isTouchOnScreen = false;

		return true;
	}

	void InputManager::onKeyHold(float interval)
	{

	}
	void InputManager::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
	{
		if (std::find(m_Instace->heldKeys.begin(), m_Instace->heldKeys.end(), keyCode) == m_Instace->heldKeys.end()){
			m_Instace->heldKeys.push_back(keyCode);
		}
	}
	void InputManager::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
	{
		m_Instace->heldKeys.erase(std::remove(m_Instace->heldKeys.begin(), m_Instace->heldKeys.end(), keyCode), m_Instace->heldKeys.end());
	}

	void InputManager::onTouchesBegan(const std::vector<Touch*>& touches, cocos2d::Event  *event)
	{
		for (auto &item : touches)
		{
			auto touch = item;
			TouchPoint * touchPoint = new  TouchPoint(touch->getLocation() * scaleFactorTouch);
			m_Instace->m_TouchList.insert(touch->getID(), touchPoint);
		}
		m_Instace->p_isTouchOnScreen = true;
	}
	void InputManager::onTouchesMoved(const std::vector<Touch*>& touches, cocos2d::Event  *event)
	{
		for (auto &item : touches)
		{
			auto touch = item;
			auto pTP = m_Instace->m_TouchList.at(touch->getID());
			auto location = touch->getLocation()* scaleFactorTouch;
			pTP->pos = location;
		}
	}
	void InputManager::onTouchesEnded(const std::vector<Touch*>& touches, cocos2d::Event  *event)
	{
		for (auto &item : touches)
		{
			auto touch = item;
			auto pTP = m_Instace->m_TouchList.at(touch->getID());
			m_Instace->m_TouchList.erase(touch->getID());
			m_Instace->m_TouchReleaseList.insert(touch->getID(), pTP);
		}
		m_Instace->p_isTouchOnScreen = false;
	}
	void InputManager::onTouchesCancelled(const std::vector<Touch*>& touches, cocos2d::Event  *event)
	{
		onTouchesEnded(touches, event);
	}


	InputManager::InputManager()
	{
		m_currentControl = CONTROL_TYPE::NONE;
		//this->retain();
	}

	InputManager::~InputManager()
	{
		//this->autorelease();
	}


