//===========================================================================
// laserCannon.cpp			レーザー砲台処理
// 製作者：野村 悠人
//===========================================================================
#include "main.h"
#include "laserCannon.h"
#include "manager.h"
#include "renderer.h"
#include "modelRenderer.h"
#include "audio.h"
#include "player.h"
#include "enemy.h"
#include "health.h"

void LaserCannon::Init()
{
	ModelRenderer* comp = new ModelRenderer(this);
	comp->Load("asset\\model\\sphere_smooth.obj");
	AddComponent(comp);

	m_BGM = new Audio(this);
	m_BGM->Load("asset\\audio\\MusMus_BGM_cool02.wav");
	m_BGM->Play(true);


	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\cookTorranceVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\cookTorrancePS.cso");

	m_Pos = Vector3(0.0f, 0.0f, 0.0f);
	m_Scl = Vector3(2.0f, 2.0f, 2.0f);

	m_Radius = m_Scl.x;

	InitComponent();
}

void LaserCannon::Uninit()
{
	m_BGM->Uninit();
	delete m_BGM;

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	UninitComponent();
}

void LaserCannon::Update()
{
	m_FrontVec = -m_Laser->GetFrontVec();

	m_Rot.y = atan2f(m_FrontVec.x, m_FrontVec.z);

	Scene* scene = Manager::GetScene();
	std::vector<Enemy*> enemyList = scene->GetGameObjects<Enemy>();
	for (auto enemy : enemyList) {
		Vector3 enemyPos = enemy->GetPos();
		float enemyRadius = enemy->GetRadius();

		Vector3 direction = enemyPos - m_Pos;

		float length = direction.Length();

		if (length < m_Radius + enemyRadius) {
			scene->GetGameObject<Health>()->SubHealth(1);
			enemy->Destroy();
		}
	}

	UpdateComponent();
}

void LaserCannon::Draw()
{
	Player* player = Manager::GetScene()->GetGameObject<Player>();
	Vector3 playerVecNoraml = player->GetPos().Normalize();

	LIGHT light;
	light.Enable = true;
	//light.Direction = Vector4( 0.0f, -1.0f, 0.0f, 0.0f );
	//light.Direction = Vector4(playerVecNoraml.x, playerVecNoraml.y, playerVecNoraml.z, 0.0f);
	light.Diffuse = Color( 1.0f, 1.0f, 1.0f, 1.0f );
	light.Ambient = Color( 0.1f, 0.1f, 0.1f, 1.0f );

	Vector3 lightPos = playerVecNoraml * 100.0f;
	if (player) light.Position = Vector4(lightPos.x, lightPos.y, lightPos.z, 0.0f);
	else light.Position = Vector4(0.0f, 100.0f, 0.0f, 0.0f);			// ライトの座標
	light.PointLightParam = Vector4(500.0f, 1.0f, 0.1f, 0.0f);	// 光の範囲

	Renderer::SetLight(light);


	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダ設定 
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// ワールドマトリクス設定
	XMMATRIX world, scale, rot, trans;
	scale = XMMatrixScaling(m_Scl.x, m_Scl.y, m_Scl.z);
	rot = XMMatrixRotationRollPitchYaw(m_Rot.x, m_Rot.y, m_Rot.z);
	trans = XMMatrixTranslation(m_Pos.x, m_Pos.y, m_Pos.z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(world);

	DrawComponent();
}
