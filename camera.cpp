//===========================================================================
// camera.cpp				カメラ処理
// 製作者：野村 悠人
//===========================================================================
#include "main.h"
#include "input.h"
#include "renderer.h"
#include "camera.h"
#include "manager.h"
#include "player.h"

void Camera::Init()
{
	m_CurrentScene = Manager::GetCurrentScene();

	SetScenePosition(m_CurrentScene);
}

void Camera::Uninit()
{

}

void Camera::Update()
{
	if (m_CurrentScene != SCENE_GAME) return;
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	m_Target = player->GetPos();

	// カメラの位置をプレイヤーの真上にセット
	m_Pos.x = player->GetPos().x;
	m_Pos.z = player->GetPos().z - 1.0f;

	//if (Input::GetKeyPress(VK_UP)) {
	//	m_Pos.z += 1.0f;
	//}
	//if (Input::GetKeyPress(VK_DOWN)) {
	//	m_Pos.z -= 1.0f;
	//}
	//if (Input::GetKeyPress(VK_LEFT)) {
	//	m_Pos.x -= 1.0f;
	//}
	//if (Input::GetKeyPress(VK_RIGHT)) {
	//	m_Pos.x += 1.0f;
	//}
	//if (Input::GetKeyPress(KEY_I)) {
	//	m_Pos.y += 0.5f;
	//}
	//if (Input::GetKeyPress(KEY_K)) {
	//	m_Pos.y -= 0.5f;
	//}
}

void Camera::Draw()
{
	Vector3 up{ 0.0f, 1.0f, 0.0f };
	XMMATRIX viewMatrix = XMMatrixLookAtLH(LoadVector3(&m_Pos), LoadVector3(&m_Target), LoadVector3(&up));

	Renderer::SetViewMatrix(viewMatrix);

	XMStoreFloat4x4(&m_ViewMatrix, viewMatrix);

	// プロジェクションマトリクスの設定
	XMMATRIX projetionMatrix;
	projetionMatrix = XMMatrixPerspectiveFovLH(1.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);

	Renderer::SetProjectionMatrix(projetionMatrix);

	// カメラ座標設定
	Renderer::SetCameraPosition(m_Pos);
}

void Camera::SetScenePosition(SCENE_NAME scene)
{
	switch (scene)
	{
	case SCENE_TITLE:
		m_Pos = Vector3(0.0f, 25.0f, -1.0f);
		m_Target = Vector3(0.0f, 0.0f, 0.0f);
		break;
	case SCENE_GAME:
		m_Pos = Vector3(0.0f, m_DISTANCE, -1.0f);
		m_Target = Vector3(0.0f, 0.0f, 0.0f);
		break;
	case SCENE_RESULT:
		m_Pos = Vector3(0.0f, 25.0f, -1.0f);
		m_Target = Vector3(0.0f, 0.0f, 0.0f);
		break;
	default:
		break;
	}
}

