//===========================================================================
// prism.cpp				プリズム処理
// 製作者：野村 悠人
//===========================================================================
#include "main.h"
#include "prism.h"
#include "renderer.h"
#include "modelRenderer.h"
#include "manager.h"
#include "audio.h"

void Prism::Init()
{
	ModelRenderer* comp = new ModelRenderer(this);
	comp->Load("asset\\model\\Prism.obj");
	AddComponent(comp);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\envMappingVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\envMappingPS.cso");

	TexMetadata metadata;
	ScratchImage image;
	LoadFromWICFile(L"asset\\texture\\water-bg-pattern-04.jpg", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &m_EnvTexture);
	assert(m_EnvTexture);

	m_Pos = Vector3(0.0f, 0.0f, 0.0f);
	m_Scl = Vector3(5.0f, 5.0f, 5.0f);


	InitComponent();
}

void Prism::Uninit()
{
	if (m_BGM) {
		m_BGM->Uninit();
		delete m_BGM;
	}

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
	m_EnvTexture->Release();

	UninitComponent();
}

void Prism::Update()
{
	m_Rot.y += 0.01f;
	if (m_Rot.y >= XM_PI * 2)m_Rot.y = 0.0f;

	UpdateComponent();
}

void Prism::Draw()
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

	Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, &m_EnvTexture);

	DrawComponent();
}

void Prism::SetBGM(const char* fileName)
{
	m_BGM = new Audio(this);
	m_BGM->Load(fileName);
	m_BGM->Play(true);
}
