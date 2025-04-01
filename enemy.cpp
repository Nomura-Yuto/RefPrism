//===========================================================================
// enemy.cpp				エネミー処理
// 製作者：野村 悠人
//===========================================================================
#include "main.h"
#include "enemy.h"
#include "manager.h"
#include "renderer.h"
#include "modelRenderer.h"
#include "laserBeam.h"

void Enemy::Init()
{
	m_Radius = m_Scl.x * 0.5f;

	m_Speed = DEF_SPEED;

	ModelRenderer* comp = new ModelRenderer(this);
	comp->Load("asset\\model\\enemyModel.obj");
	AddComponent(comp);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTexturePS.cso");

	InitComponent();
}

void Enemy::Uninit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	UninitComponent();
}

void Enemy::Update()
{
	Scene* scene = Manager::GetScene();
	std::vector<LaserBeam*> laserList = scene->GetGameObjects<LaserBeam>();

	Vector3 laserBasePos = laserList.front()->GetStartPos();

	Vector3 direction = laserBasePos - m_Pos;
	Vector3 normal = direction.Normalize();

	m_Pos += normal * (m_Speed * DT);

	m_Rot.y = atan2f(normal.x, normal.z);


	UpdateComponent();
}

void Enemy::Draw()
{
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

