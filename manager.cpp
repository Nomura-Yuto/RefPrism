//===========================================================================
// manager.cpp				ゲームマネージャー
// 製作者：野村 悠人
//===========================================================================
#include <list>
#include "main.h"
#include "input.h"
#include "manager.h"
#include "renderer.h"
#include "modelRenderer.h"
#include "scene.h"
#include "audio.h"
#include "titleScene.h"
#include "gameScene.h"

Scene* Manager::m_Scene{};
Scene* Manager::m_NextScene{};
SCENE_NAME Manager::m_CurrentScene{};
std::mt19937 Manager::m_Rnd{ std::random_device{}() };

void Manager::Init()
{
	Renderer::Init();

	Input::Init();

	Audio::InitMaster();

	m_Scene = new TitleScene;
	//m_Scene = new GameScene;
	m_Scene->Init();
}


void Manager::Uninit()
{
	m_Scene->Uninit();
	delete m_Scene;

	ModelRenderer::UnloadAll();

	Audio::UninitMaster();

	Input::Uninit();

	Renderer::Uninit();
}

void Manager::Update()
{
	Input::Update();

	m_Scene->Update();

}

void Manager::Draw()
{
	Renderer::Begin();

	m_Scene->Draw();

	Renderer::End();

	if (m_NextScene) {
		m_Scene->Uninit();
		delete m_Scene;

		m_Scene = m_NextScene;
		m_Scene->Init();
		m_NextScene = nullptr;
	}
}

int Manager::Rand(int min, int max)
{
	std::uniform_int_distribution<int> dist(min, max);
	return dist(m_Rnd);
}

float Manager::Randf(float min, float max) {
	std::uniform_real_distribution<float> dist(min, max);
	return dist(m_Rnd);
}
