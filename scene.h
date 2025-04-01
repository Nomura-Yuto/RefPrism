//===========================================================================
// scene.h					シーン処理
// 製作者：野村 悠人
//===========================================================================
#pragma once
#include <list>
#include <vector>
#include "main.h"
#include "gameObject.h"

//==============================
// 列挙体
//==============================
typedef enum
{
	LAYER_SYSTEM = 0,
	LAYER_3D,
	LAYER_EFFECT,
	LAYER_2D,
	LAYER_MAX
}LAYER_TYPE;

typedef enum
{
	SCENE_TITLE = 0,
	SCENE_GAME,
	SCENE_RESULT,
	SCENE_MAX
}SCENE_NAME;

class Scene
{
protected:
	std::list<GameObject*> m_GameObjects[LAYER_MAX];

public:
	Scene() = default;
	virtual ~Scene() {}

	virtual void Init(){}
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();

	// オブジェクトの追加
	template<typename T>
	T* AddGameObject(int layer)
	{
		T* gameObj = new T();
		gameObj->Init();
		m_GameObjects[layer].push_back(gameObj);

		return gameObj;
	}

	GameObject* AddGameObject(GameObject* obj, int layer);

	// 指定した型のオブジェクトを取得
	template<typename T>
	T* GetGameObject(){
		for (int i = 0; i < LAYER_MAX; i++) {
			for (auto a : m_GameObjects[i]) {
				T* ret = dynamic_cast<T*>(a);

				if (ret != nullptr) 
					return ret;
			}
		}
		return nullptr;
	}

	// 指定した型のオブジェクトのリストを取得
	template<typename T>
	std::vector<T*> GetGameObjects()
	{
		std::vector<T*> retObjects;

		for (int i = 0; i < LAYER_MAX; i++) {
			for (auto a : m_GameObjects[i]) {
				T* ret = dynamic_cast<T*>(a);

				if (ret != nullptr)
					retObjects.push_back(ret);
			}
		}
		return retObjects;
	}
};
