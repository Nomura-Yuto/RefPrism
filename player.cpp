//===========================================================================
// player.cpp				プレイヤー処理
// 製作者：野村 悠人
//===========================================================================
#include "main.h"
#include "player.h"
#include "input.h"
#include "renderer.h"
#include "modelRenderer.h"
#include "manager.h"
#include "audio.h"
#include "camera.h"
#include "laserBeam.h"
#include "obstacle.h"

void Player::Init()
{
	ModelRenderer* comp = new ModelRenderer(this);
	comp->Load("asset\\model\\Emerald_2.obj");
	AddComponent(comp);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\envMappingVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\envMappingPS.cso");

	TexMetadata metadata;
	ScratchImage image;
	LoadFromWICFile(L"asset\\texture\\crystal.jpg", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &m_EnvTexture);
	assert(m_EnvTexture);


	m_SE = new Audio(this);
	m_SE->Load("asset\\audio\\wan.wav");

	m_Pos = Vector3(1.0f, 0.0f, -4.0f);
	m_Scl = Vector3(2.0f, 1.0f, 1.5f);

	InitComponent();
}

void Player::Uninit()
{
	m_SE->Uninit();
	delete m_SE;

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
	m_EnvTexture->Release();

	UninitComponent();
}

void Player::Update()
{
	Scene* scene = Manager::GetScene();
	Camera* camera = scene->GetGameObject<Camera>();
	Vector3 foward = camera->GetFoward();
	Vector3 right = camera->GetRight();
	std::vector<LaserBeam*> laserList = scene->GetGameObjects<LaserBeam>();

	float groundHeight = 0.0f;

	m_Vel.x = 0.0f;
	m_Vel.z = 0.0f;
	m_OldPos = m_Pos;

	// 移動
	if (Input::GetKeyPress(KEY_W)) {
		m_Vel.z = DEF_SPEED;
	}
	if (Input::GetKeyPress(KEY_S)) {
		m_Vel.z = -DEF_SPEED;
	}
	if (Input::GetKeyPress(KEY_A)) {
		m_Vel.x = -DEF_SPEED;
	}
	if (Input::GetKeyPress(KEY_D)) {
		m_Vel.x = DEF_SPEED;
	}

	float rotSpeed = 0.02;
	for (auto laser : laserList) {
		if (laser->GetLaserState() == STATE_REFRACT) {
			rotSpeed = 0.05f;
		}
	}

	// 回転
	if (Input::GetKeyPress(VK_LEFT)) {
		m_Rot.y -= rotSpeed;
	}
	if (Input::GetKeyPress(VK_RIGHT)) {
		m_Rot.y += rotSpeed;
	}

	if (Input::GetKeyTrigger(VK_SPACE)) {
		for (auto laser : laserList) {
			if (laser->GetTargetObj() == this) {
				laser->SwapLaserState();
			}
		}
	}

	// 移動
	m_Pos += m_Vel * DT;

	if (m_Pos.x >  PLAY_FIELD_SIZE) m_Pos.x =  PLAY_FIELD_SIZE;
	if (m_Pos.x < -PLAY_FIELD_SIZE) m_Pos.x = -PLAY_FIELD_SIZE;
	if (m_Pos.z >  PLAY_FIELD_SIZE) m_Pos.z =  PLAY_FIELD_SIZE;
	if (m_Pos.z < -PLAY_FIELD_SIZE) m_Pos.z = -PLAY_FIELD_SIZE;

	// 障害物との衝突判定
	auto obstacleList = scene->GetGameObjects<Obstacle>();
	for (auto obstacle : obstacleList) {
		Vector3 obstaclePos = obstacle->GetPos();
		Vector3 obstacleScl = obstacle->GetScl();

	
		Vector3 direction;
		direction.x = obstaclePos.x - m_Pos.x;
		direction.y = obstaclePos.y - m_Pos.y;
		direction.z = obstaclePos.z - m_Pos.z;
	
		float length;
		length = sqrtf(direction.x * direction.x
			+ direction.z * direction.z);
	
		if (length < obstacleScl.x) {
			m_Pos.x = m_OldPos.x;
			m_Pos.z = m_OldPos.z;
			m_Vel.x = 0.0f;
			m_Vel.z = 0.0f;
		}
	}

	UpdateComponent();
}

void Player::Draw()
{
	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダ設定 
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// ワールドマトリクス設定
	XMMATRIX world, scale, rot, trans;
	scale = XMMatrixScaling(m_Scl.x, m_Scl.y, m_Scl.z);
	rot = XMMatrixRotationRollPitchYaw(m_Rot.x, m_Rot.y + XM_PI, m_Rot.z);
	trans = XMMatrixTranslation(m_Pos.x, m_Pos.y, m_Pos.z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(world);

	Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, &m_EnvTexture);

	DrawComponent();
}
