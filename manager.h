//===========================================================================
// manager.h				ゲームマネージャー
// 製作者：野村 悠人
//===========================================================================
#pragma once
#include "scene.h"
#include <random>

class Manager
{
private:
	static Scene* m_Scene;
	static Scene* m_NextScene;
	static SCENE_NAME m_CurrentScene;
	static std::mt19937 m_Rnd;

public:
	static void Init();
	static void Uninit();
	static void Update();
	static void Draw();

	static int Rand(int min = 0, int max = 1);
	static float Randf(float min = 0.0f, float max = 1.0f);

	static Scene* GetScene() { return m_Scene; }
	static SCENE_NAME GetCurrentScene() { return m_CurrentScene; }

	template<typename T>
	static T* SetScene() {
		m_NextScene = new T;

		T* ret = dynamic_cast<T*>(m_NextScene);
		return ret;
	}
	static void SetCurrentScene(SCENE_NAME sceneName) {
		m_CurrentScene = sceneName;
	}
};