#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"

PlayScene::PlayScene()
{
	PlayScene::start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::draw()
{
	TextureManager::Instance()->draw("playBackground", 640, 360, 0, 225, true);
	TextureManager::Instance()->draw("enemy", 625, 570, 0, 225, true, SDL_FLIP_HORIZONTAL);

	drawDisplayList();
	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);

	if (EventManager::Instance().isIMGUIActive())
	{
		GUI_Function();
	}
}

void PlayScene::update()
{
	updateDisplayList();
}

void PlayScene::clean()
{
	removeAllChildren();
}

void PlayScene::handleEvents()
{
	EventManager::Instance().update();

	// handle player movement with GameController
	if (SDL_NumJoysticks() > 0)
	{
		if (EventManager::Instance().getGameController(0) != nullptr)
		{
			const auto deadZone = 10000;
			if (EventManager::Instance().getGameController(0)->LEFT_STICK_X > deadZone)
			{
				m_pPlayer->setAnimationState(PLAYER_RUN_RIGHT);
				m_playerFacingRight = true;
			}
			else if (EventManager::Instance().getGameController(0)->LEFT_STICK_X < -deadZone)
			{
				m_pPlayer->setAnimationState(PLAYER_RUN_LEFT);
				m_playerFacingRight = false;
			}
			else
			{
				if (m_playerFacingRight)
				{
					m_pPlayer->setAnimationState(PLAYER_IDLE_RIGHT);
				}
				else
				{
					m_pPlayer->setAnimationState(PLAYER_IDLE_LEFT);
				}
			}
		}
	}


	// handle player movement if no Game Controllers found
	if (SDL_NumJoysticks() < 1)
	{
		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_A))
		{
			m_pPlayer->setAnimationState(PLAYER_RUN_LEFT);
			m_playerFacingRight = false;
		}
		else if (EventManager::Instance().isKeyDown(SDL_SCANCODE_D))
		{
			m_pPlayer->setAnimationState(PLAYER_RUN_RIGHT);
			m_playerFacingRight = true;
		}
		else
		{
			if (m_playerFacingRight)
			{
				m_pPlayer->setAnimationState(PLAYER_IDLE_RIGHT);
			}
			else
			{
				m_pPlayer->setAnimationState(PLAYER_IDLE_LEFT);
			}
		}
	}
	

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	{
		TheGame::Instance()->changeSceneState(START_SCENE);
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_2))
	{
		TheGame::Instance()->changeSceneState(END_SCENE);
	}
}

void PlayScene::start()
{
	TextureManager::Instance()->load("../Assets/textures/playBG.png", "playBackground");
	TextureManager::Instance()->load("../Assets/sprites/stormtroopers.png", "enemy");

	// Set GUI Title
	m_guiTitle = "Play Scene";
	
	// Plane Sprite
	//m_pPlaneSprite = new Plane();
	//addChild(m_pPlaneSprite);

	// Player Sprite
	m_pPlayer = new Player();
	addChild(m_pPlayer);
	m_playerFacingRight = true;

	// Bomb Sprite
	m_pBomb = new Target();
	addChild(m_pBomb);	
	
	// Back Button
	m_pBackButton = new Button("../Assets/textures/backButton.png", "backButton", BACK_BUTTON);
	m_pBackButton->getTransform()->position = glm::vec2(80.0f, 680.0f);
	m_pBackButton->addEventListener(CLICK, [&]()-> void
	{
		m_pBackButton->setActive(false);
		TheGame::Instance()->changeSceneState(START_SCENE);
	});

	m_pBackButton->addEventListener(MOUSE_OVER, [&]()->void
	{
		m_pBackButton->setAlpha(128);
	});

	m_pBackButton->addEventListener(MOUSE_OUT, [&]()->void
	{
		m_pBackButton->setAlpha(255);
	});
	addChild(m_pBackButton);

	// Next Button
	m_pNextButton = new Button("../Assets/textures/nextButton.png", "nextButton", NEXT_BUTTON);
	m_pNextButton->getTransform()->position = glm::vec2(1200.0f, 680.0f);
	m_pNextButton->addEventListener(CLICK, [&]()-> void
	{
		m_pNextButton->setActive(false);
		TheGame::Instance()->changeSceneState(END_SCENE);
	});

	m_pNextButton->addEventListener(MOUSE_OVER, [&]()->void
	{
		m_pNextButton->setAlpha(128);
	});

	m_pNextButton->addEventListener(MOUSE_OUT, [&]()->void
	{
		m_pNextButton->setAlpha(255);
	});

	addChild(m_pNextButton);

	/* Instructions Label */
	m_pInstructionsLabel = new Label("Press the backtick (`) character to toggle Debug View", "Consolas");
	m_pInstructionsLabel->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.5f, 700.0f);
	addChild(m_pInstructionsLabel);
}

void PlayScene::GUI_Function() const
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("Physics stuff", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);

	if(ImGui::Button("THROW"))
	{
		std::cout << "THROW Pressed" << std::endl;
		m_pBomb->doThrow();
	}

	static int xPlayerPos = 140;
	float Vx = 0, Vy = 0;
	float hVelocity = 0;
	float vAngle = 1 / sin(Vy / 95);
	static float velocityAngle[2] = { 0 , 0 };

	if (ImGui::Button("a) Solution"))
	{
		xPlayerPos = 140;
		m_pPlayer->getTransform()->position.x = 140;
		m_pBomb->throwPosition = glm::vec2(140, 570);
		velocityAngle[0] = 95;
		velocityAngle[1] = 15.88;
		m_pBomb->throwSpeed = glm::vec2(velocityAngle[0], -velocityAngle[1]);
		m_pBomb->doThrow();
	}

	if (ImGui::Button("b) Solution"))
	{
		xPlayerPos = 140;
		m_pPlayer->getTransform()->position.x = 140;
		m_pBomb->throwPosition = glm::vec2(140, 570);
		velocityAngle[0] = 95;
		velocityAngle[1] = 45;
		m_pBomb->throwSpeed = glm::vec2(velocityAngle[0], -velocityAngle[1]);
		m_pBomb->doThrow();
	}

	ImGui::Separator();

	// Bomb always has gravity
	m_pBomb->isGravityEnabled = true;
	
	// Player location
	if (ImGui::SliderInt("Player Position on X", &xPlayerPos, 140, 1140))
	{
		m_pPlayer->getTransform()->position.x = xPlayerPos;
		m_pBomb->throwPosition = glm::vec2(xPlayerPos, 570);
	}

	// Bomb Vx and Vy
	if (ImGui::SliderFloat2("Throw Speed | Throw Angle", velocityAngle, 0, 100))
	{
		m_pBomb->throwSpeed = glm::vec2(velocityAngle[0], -velocityAngle[1]);
	}
	
	ImGui::End();
	ImGui::EndFrame();

	// Don't Remove this
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	ImGui::StyleColorsDark();
}
