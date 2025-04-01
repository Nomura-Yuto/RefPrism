//===========================================================================
// polygon2D.cpp			2Dポリゴン表示
// 製作者：野村 悠人
//===========================================================================
#include "main.h"
#include "renderer.h"
#include "polygon2D.h"
#include "audio.h"

void Polygon2D::Init()
{
	m_Size = Vector3(100.0f, 100.0f, 0.0f);
	m_Pos = Vector3(0.0f, 0.0f, 0.0f);
	

	VERTEX_3D vertex[4];

	vertex[0].Position	= Vector3(m_Pos.x, m_Pos.y, 0.0f);
	vertex[0].Normal	= Vector3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse	= Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord	= Vector2(0.0f, 0.0f);

	vertex[1].Position	= Vector3(m_Pos.x + m_Size.x, m_Pos.y, 0.0f);
	vertex[1].Normal	= Vector3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse	= Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord	= Vector2(1.0f, 0.0f);

	vertex[2].Position	= Vector3(m_Pos.x, m_Pos.y + m_Size.y, 0.0f);
	vertex[2].Normal	= Vector3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse	= Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord	= Vector2(0.0f, 1.0f);

	vertex[3].Position	= Vector3(m_Pos.x + m_Size.x, m_Pos.y + m_Size.y, 0.0f);
	vertex[3].Normal	= Vector3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse	= Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord	= Vector2(1.0f, 1.0f);

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	TexMetadata metadata;
	ScratchImage image;
	LoadFromWICFile(L"asset\\texture\\white.png", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &m_Texture);
	assert(m_Texture);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTexturePS.cso");

	//m_BGM = new Audio(this);
	//m_BGM->Load("asset\\audio\\kotori_and_yousei.wav");
	//m_BGM->Play(true);

	InitComponent();
}

void Polygon2D::Uninit()
{
	//m_BGM->Uninit();
	//delete m_BGM;

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	m_VertexBuffer->Release();
	m_Texture->Release();

	UninitComponent();
}

void Polygon2D::Update()
{
	UpdateComponent();
}

void Polygon2D::Draw()
{
	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);
	
	//シェーダ設定 
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// マトリクス設定
	Renderer::SetWorldViewProjection2D();

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// マテリアル設定
	MATERIAL material{};
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = m_Color;
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	// テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	//プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ポリゴン描画
	Renderer::GetDeviceContext()->Draw(4, 0);

	DrawComponent();
}

void Polygon2D::SetTexture(const wchar_t* fileName)
{
	TexMetadata metadata;
	ScratchImage image;
	LoadFromWICFile(fileName, WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &m_Texture);
	assert(m_Texture);
}

void Polygon2D::SetTexPos(const Vector2& pos)
{
	m_Pos.x = pos.x;
	m_Pos.y = pos.y;

	// 頂点データ書き換え
	D3D11_MAPPED_SUBRESOURCE msr;
	HRESULT hr = Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0,
		D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	vertex[0].Position	= Vector3(m_Pos.x, m_Pos.y, 0.0f);
	vertex[0].Normal	= Vector3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse	= Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord	= Vector2(0.0f, 0.0f);

	vertex[1].Position	= Vector3(m_Pos.x + m_Size.x, m_Pos.y, 0.0f);
	vertex[1].Normal	= Vector3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse	= Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord	= Vector2(1.0f, 0.0f);

	vertex[2].Position	= Vector3(m_Pos.x, m_Pos.y + m_Size.y, 0.0f);
	vertex[2].Normal	= Vector3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse	= Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord	= Vector2(0.0f, 1.0f);

	vertex[3].Position	= Vector3(m_Pos.x + m_Size.x, m_Pos.y + m_Size.y, 0.0f);
	vertex[3].Normal	= Vector3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse	= Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord	= Vector2(1.0f, 1.0f);

	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);
}

void Polygon2D::SetTexSize(const Vector2& size)
{
	m_Size.x = size.x;
	m_Size.y = size.y;

	// 頂点データ書き換え
	D3D11_MAPPED_SUBRESOURCE msr;
	HRESULT hr = Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0,
		D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	vertex[0].Position	= Vector3(m_Pos.x, m_Pos.y, 0.0f);
	vertex[0].Normal	= Vector3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse	= Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord	= Vector2(0.0f, 0.0f);

	vertex[1].Position	= Vector3(m_Pos.x + m_Size.x, m_Pos.y, 0.0f);
	vertex[1].Normal	= Vector3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse	= Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord	= Vector2(1.0f, 0.0f);

	vertex[2].Position	= Vector3(m_Pos.x, m_Pos.y + m_Size.y, 0.0f);
	vertex[2].Normal	= Vector3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse	= Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord	= Vector2(0.0f, 1.0f);

	vertex[3].Position	= Vector3(m_Pos.x + m_Size.x, m_Pos.y + m_Size.y, 0.0f);
	vertex[3].Normal	= Vector3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse	= Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord	= Vector2(1.0f, 1.0f);

	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);
}
