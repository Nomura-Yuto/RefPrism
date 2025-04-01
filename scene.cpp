//===========================================================================
// scene.cpp				�V�[������
// ����ҁF�쑺 �I�l
//===========================================================================
#include "scene.h"


void Scene::Uninit()
{
	for (int i = 0; i < LAYER_MAX; i++) {
		for (auto a : m_GameObjects[i]) {
			a->Uninit();
			delete a;
		}
		m_GameObjects[i].clear();
	}
}

void Scene::Update()
{
	for (int i = 0; i < LAYER_MAX; i++) {
		for (auto a : m_GameObjects[i]) {
			a->Update();
		}

		// �폜�\�񂳂ꂽ�I�u�W�F�N�g�̍폜
		m_GameObjects[i].remove_if([](GameObject* p) {
			return p->IsDestroy(); 
		});
	}
}

void Scene::Draw()
{
	for (int i = 0; i < LAYER_MAX; i++) {

		//if (GetGameObject<Camera>()) {
		//	XMFLOAT3 cameraPos = GetGameObject<Camera>()->GetPos();
		//	XMMATRIX view = GetGameObject<Camera>()->GetViewMatrix();
		//	
		//	m_GameObjects[i].sort([cameraPos, view](GameObject* a, GameObject* b) {
		//		return a->GetDistance(view) > b->GetDistance(view);
		//	});
		//}


		for (auto a : m_GameObjects[i]) {
			a->Draw();
		}
	}
}

GameObject* Scene::AddGameObject(GameObject* obj, int layer)
{
	obj->Init();
	m_GameObjects[layer].push_back(obj);

	return obj;
}
