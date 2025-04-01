//===========================================================================
// obstacle.cpp				��Q������
// ����ҁF�쑺 �I�l
//===========================================================================
#include "main.h"
#include "obstacle.h"
#include "manager.h"
#include "renderer.h"
#include "modelRenderer.h"

void Obstacle::Init()
{
	m_Radius = m_Scl.x * 0.5f;

	ModelRenderer* comp = new ModelRenderer(this);
	comp->Load("asset\\model\\cylinder.obj");
	AddComponent(comp);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTexturePS.cso");

	InitComponent();
}

void Obstacle::Uninit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	UninitComponent();
}

void Obstacle::Update()
{

}

void Obstacle::Draw()
{
	// ���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//�V�F�[�_�ݒ� 
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// ���[���h�}�g���N�X�ݒ�
	XMMATRIX world, scale, rot, trans;
	scale = XMMatrixScaling(m_Scl.x, m_Scl.y, m_Scl.z);
	rot = XMMatrixRotationRollPitchYaw(m_Rot.x, m_Rot.y, m_Rot.z);
	trans = XMMatrixTranslation(m_Pos.x, m_Pos.y, m_Pos.z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(world);

	DrawComponent();
}

