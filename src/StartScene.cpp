#include "StartScene.h"
#include <algorithm>
#include "Game.h"
#include "glm/gtx/string_cast.hpp"
#include "EventManager.h"

StartScene::StartScene()
{
	StartScene::start();
}

StartScene::~StartScene()
= default;

void StartScene::draw()
{
	TextureManager::Instance()->draw("startBG", 640, 360, 0, 255, true, SDL_FLIP_HORIZONTAL);
	drawDisplayList();
}

void StartScene::update()
{
	updateDisplayList();
}

void StartScene::clean()
{
	removeAllChildren();
}

void StartScene::handleEvents()
{
	EventManager::Instance().update();

	// Keyboard Events
	if(EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}

	if(EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	{
		TheGame::Instance()->changeSceneState(PLAY_SCENE);
	}
}

void StartScene::start()
{
	TextureManager::Instance()->load("../Assets/textures/startBG.png", "startBG");	

	const SDL_Color blue = { 0, 0, 255, 255 };
	const SDL_Color yellow = { 255, 232, 31, 255 };
	const SDL_Color red = {240, 28, 28, 255};

	m_pStartLabel = new Label("Physics Assignment 1", "StarJedi", 70, yellow, glm::vec2(640.0f, 50.0f));
	m_pStartLabel->setParent(this);
	addChild(m_pStartLabel);

	m_pInstructionsLabel = new Label("Press 1 or click START to Play", "StarJedi", 25, yellow, glm::vec2(960.0f, 120.0f));
	m_pInstructionsLabel->setParent(this);
	addChild(m_pInstructionsLabel);

	m_pNameLabel = new Label("Made by Hancong Zhang and Zijie Wang", "Consolas", 25, red, glm::vec2(275.0f, 675.0f));
	m_pNameLabel->setParent(this);
	addChild(m_pNameLabel);

	m_pIDLabel = new Label("Student IDs: 101234068    101283316", "Consolas", 25, red, glm::vec2(260.0f, 700.0f));
	m_pIDLabel->setParent(this);
	addChild(m_pIDLabel);

	//m_pShip = new Ship();
	//m_pShip->getTransform()->position = glm::vec2(400.0f, 300.0f); 
	//addChild(m_pShip); 

	// Start Button
	m_pStartButton = new Button();
	m_pStartButton->getTransform()->position = glm::vec2(640.0f, 450.0f); 

	m_pStartButton->addEventListener(CLICK, [&]()-> void
	{
		m_pStartButton->setActive(false);
		TheGame::Instance()->changeSceneState(PLAY_SCENE);
	});
	
	m_pStartButton->addEventListener(MOUSE_OVER, [&]()->void
	{
		m_pStartButton->setAlpha(128);
	});

	m_pStartButton->addEventListener(MOUSE_OUT, [&]()->void
	{
		m_pStartButton->setAlpha(255);
	});
	addChild(m_pStartButton);

	
}

