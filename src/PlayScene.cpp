#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"
#include <cmath>
#include <math.h>

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"
#include "Util.h"

#define PI 3.14159265418;

PlayScene::PlayScene()
{
	PlayScene::start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::draw()
{
	TextureManager::Instance()->draw("playBackground", 640, 360, 0, 225, true);
	//TextureManager::Instance()->draw("enemy", 625, 570, 0, 225, true, SDL_FLIP_HORIZONTAL);

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
	//TextureManager::Instance()->load("../Assets/sprites/stormtroopers.png", "enemy");

	// Set GUI Title
	m_guiTitle = "Play Scene";

	// Crate Sprite
	m_pCrate = new Target();
	addChild(m_pCrate);
	
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
	m_pInstructionsLabel = new Label("Press the backtick (`) to see the ramp", "Consolas");
	m_pInstructionsLabel->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.5f, 100.0f);
	addChild(m_pInstructionsLabel);


}

void PlayScene::GUI_Function() const
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("Physics stuff", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);

	static float  run = 400.0f, rise = 300.0f, mass = 12.8, CFriction = 0.42, theta = 36.87;
	static float rotateAngle = tanh((150 + run) / (550 - rise)) * 180 / PI;
	static float startLocation, hypotenuse, netforce, acceleration, groundFriction;
	hypotenuse = sqrt(rise * rise + run * run);
	netforce = mass * (rise / hypotenuse);
	groundFriction = mass * CFriction * 9.8;

	m_pCrate->acceleration = netforce / mass;
	m_pCrate->getRigidBody()->acceleration.x = m_pCrate->acceleration * (run / hypotenuse);
	m_pCrate->getRigidBody()->acceleration.y = m_pCrate->acceleration * (rise / hypotenuse);
	m_pCrate->acceleFric = groundFriction / mass;

	glm::vec2 original = glm::vec2(150, 550);
	glm::vec2 riseEnd = glm::vec2(150, 550 - rise);
	glm::vec2 runEnd = glm::vec2(150 + run, 550);
	glm::vec4 colorOfLine = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
	Util::normalize(original);

	Util::DrawLine(original, riseEnd, colorOfLine);
	Util::DrawLine(original, runEnd, colorOfLine);
	Util::DrawLine(riseEnd, runEnd, colorOfLine);

	m_pCrate->releasePosition = glm::vec2(original.x, original.y - rise);
	glm::vec2 theVelocity = m_pCrate->getRigidBody()->velocity;

	if (ImGui::SliderFloat("Rise", &rise, 0.0f, 500.0f))
	{
		std::cout << "Rise(Y) changed!" << std::endl;
 	}

	if (ImGui::SliderFloat("Run", &run, 0.0f, 1000.0f))
	{
		std::cout << "Run(X) changed!" << std::endl;
	}

	if (ImGui::SliderFloat("Mass", &mass, 0.0f, 100.0f))
	{
		std::cout << "Mass changed!" << std::endl;
	}

	if (ImGui::SliderFloat("Friction", &CFriction, 0.0f, 10.0f))
	{
		std::cout << "Friction changed!" << std::endl;
	}

	if (ImGui::SliderFloat("Theta", &theta, 0.0f, 90.0f))
	{
		m_pCrate->getTransform()->rotation = theta;
		std::cout << "Theta changed!" << std::endl;
	}

	/******************************************************************/

	if (ImGui::Button("Solution"))
	{
		rise = 300.0f;
		run = 400.0f;
		mass = 12.8;
		CFriction = 0.42;
		m_pCrate->getTransform()->position.x = original.x + 40;
		m_pCrate->getTransform()->position.y = original.y - rise - 40;
		m_pCrate->getRigidBody()->velocity.x = 0;
		m_pCrate->getRigidBody()->velocity.y = 0;
		m_pCrate->acceleration = netforce / mass;
		m_pCrate->getTransform()->rotation = theta;
		m_pCrate->release();
	}

	/******************************************************************/

	if(ImGui::Button("RELEASE"))
	{
		std::cout << "Crate released" << std::endl;
		m_pCrate->acceleration = netforce / mass;
		m_pCrate->release();
	}

	/******************************************************************/

	if(ImGui::Button("RESET"))
	{
		rise = 300.0f;
		run = 400.0f;
		mass = 12.8;
		CFriction = 0.42;
		m_pCrate->getTransform()->position.x = original.x;
		m_pCrate->getTransform()->position.y = original.y - rise;
		m_pCrate->getRigidBody()->velocity.x = 0;
		m_pCrate->getRigidBody()->velocity.y = 0;
		m_pCrate->acceleration = 0;
		m_pCrate->getTransform()->rotation = 0;
		m_pCrate->release();
	}
	
	ImGui::Separator();

	ImGui::Value("Mass", mass);
	ImGui::Value("Position on X", m_pCrate->getTransform()->position.x);
	ImGui::Value("Position on Y", m_pCrate->getTransform()->position.y);
	ImGui::Value("Velocity on X", m_pCrate->getRigidBody()->velocity.x);
	ImGui::Value("Velocity on Y", m_pCrate->getRigidBody()->velocity.y);
	ImGui::Value("Acceleration", m_pCrate->acceleration);

	
	ImGui::End();
	ImGui::EndFrame();

	// Don't Remove this
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	ImGui::StyleColorsDark();
}

float rotateAngles(float x, float y)
{
	return tanh((150 + y) / (550 - x)) * 180 / PI;
}