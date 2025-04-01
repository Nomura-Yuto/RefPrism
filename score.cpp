//===========================================================================
// score.cpp				スコア表示
// 製作者：野村 悠人
//===========================================================================
#include "main.h"
#include "renderer.h"
#include "score.h"

void Score::Init()
{
	m_DivValue = Vector2(1.0f / DIV_NUM_X, 1.0f / DIV_NUM_Y);
	m_Count = 0;

	m_Size = Vector3(100.0f, 100.0f, 0.0f);
	m_Pos = Vector3(0.0f, 0.0f, 0.0f);
	m_LeftTopPos = m_Pos;


	VERTEX_3D vertex[4];

	vertex[0].Position = Vector3(m_Pos.x, m_Pos.y, 0.0f);
	vertex[0].Normal = Vector3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = Vector2(0.0f, 0.0f);

	vertex[1].Position = Vector3(m_Pos.x + m_Size.x, m_Pos.y, 0.0f);
	vertex[1].Normal = Vector3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = Vector2(1.0f, 0.0f);

	vertex[2].Position = Vector3(m_Pos.x, m_Pos.y + m_Size.y, 0.0f);
	vertex[2].Normal = Vector3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = Vector2(0.0f, 1.0f);

	vertex[3].Position = Vector3(m_Pos.x + m_Size.x, m_Pos.y + m_Size.y, 0.0f);
	vertex[3].Normal = Vector3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = Vector2(1.0f, 1.0f);

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
	LoadFromWICFile(L"asset\\texture\\number.png", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &m_Texture);
	assert(m_Texture);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTexturePS.cso");

	InitComponent();
}

void Score::Uninit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	m_VertexBuffer->Release();
	m_Texture->Release();

	UninitComponent();
}

void Score::Update()
{
	if (m_Count > 9999) {
		m_Count = 9999;
	}

	UpdateComponent();
}

void Score::Draw()
{
	int score = m_Count;

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

	for (int i = 1; i <= DIGIT_MAX; i++) 
	{
		int num = score % 10;

		// テクスチャ座標算出
		float x = num % DIV_NUM_X * (1.0f / DIV_NUM_X);
		float y = num / DIV_NUM_X * (1.0f / DIV_NUM_Y);

		SetVertex(Vector2(m_LeftTopPos.x + (m_Size.x * 0.5f) * (DIGIT_MAX - i), m_LeftTopPos.y), 
			Vector2(m_Size.x, m_Size.y), Vector2(x, y), m_DivValue);

		// ポリゴン描画
		Renderer::GetDeviceContext()->Draw(4, 0);

		score /= 10;
	}

	DrawComponent();
}

void Score::SetTexPos(const Vector2& pos)
{
	m_Pos.x = pos.x;
	m_Pos.y = pos.y;
	m_LeftTopPos = m_Pos;

	// 頂点データ書き換え
	D3D11_MAPPED_SUBRESOURCE msr;
	HRESULT hr = Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0,
		D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	vertex[0].Position	= Vector3(pos.x, pos.y, 0.0f);
	vertex[0].Normal	= Vector3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse	= Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord	= Vector2(0.0f, 0.0f);

	vertex[1].Position	= Vector3(pos.x + m_Size.x, pos.y, 0.0f);
	vertex[1].Normal	= Vector3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse	= Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord	= Vector2(1.0f, 0.0f);

	vertex[2].Position	= Vector3(pos.x, pos.y + m_Size.y, 0.0f);
	vertex[2].Normal	= Vector3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse	= Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord	= Vector2(0.0f, 1.0f);

	vertex[3].Position	= Vector3(pos.x + m_Size.x, pos.y + m_Size.y, 0.0f);
	vertex[3].Normal	= Vector3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse	= Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord	= Vector2(1.0f, 1.0f);

	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);
}

void Score::SetLeftTopPos(const Vector2& pos)
{
	// 頂点データ書き換え
	D3D11_MAPPED_SUBRESOURCE msr;
	HRESULT hr = Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0,
		D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	vertex[0].Position	= Vector3(pos.x, pos.y, 0.0f);
	vertex[0].Normal	= Vector3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse	= Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord	= Vector2(0.0f, 0.0f);

	vertex[1].Position	= Vector3(pos.x + m_Size.x, pos.y, 0.0f);
	vertex[1].Normal	= Vector3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse	= Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord	= Vector2(1.0f, 0.0f);

	vertex[2].Position	= Vector3(pos.x, pos.y + m_Size.y, 0.0f);
	vertex[2].Normal	= Vector3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse	= Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord	= Vector2(0.0f, 1.0f);

	vertex[3].Position	= Vector3(pos.x + m_Size.x, pos.y + m_Size.y, 0.0f);
	vertex[3].Normal	= Vector3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse	= Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord	= Vector2(1.0f, 1.0f);

	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);
}

void Score::SetTexSize(const Vector2& size)
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

	vertex[1].Position	= Vector3(m_Pos.x + size.x, m_Pos.y, 0.0f);
	vertex[1].Normal	= Vector3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse	= Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord	= Vector2(1.0f, 0.0f);

	vertex[2].Position	= Vector3(m_Pos.x, m_Pos.y + size.y, 0.0f);
	vertex[2].Normal	= Vector3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse	= Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord	= Vector2(0.0f, 1.0f);

	vertex[3].Position	= Vector3(m_Pos.x + size.x, m_Pos.y + size.y, 0.0f);
	vertex[3].Normal	= Vector3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse	= Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord	= Vector2(1.0f, 1.0f);

	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);
}

void Score::SetTexCoord(const Vector2& texCoord, const Vector2& divValue)
{
	// 頂点データ書き換え
	D3D11_MAPPED_SUBRESOURCE msr;
	HRESULT hr = Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0,
		D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	vertex[0].Position	= Vector3(m_Pos.x, m_Pos.y, 0.0f);
	vertex[0].Normal	= Vector3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse	= Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord	= Vector2(texCoord.x, texCoord.y);

	vertex[1].Position	= Vector3(m_Pos.x + m_Size.x, m_Pos.y, 0.0f);
	vertex[1].Normal	= Vector3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse	= Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord	= Vector2(texCoord.x + divValue.x, texCoord.y);

	vertex[2].Position	= Vector3(m_Pos.x, m_Pos.y + m_Size.y, 0.0f);
	vertex[2].Normal	= Vector3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse	= Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord	= Vector2(texCoord.x, texCoord.y + divValue.y);

	vertex[3].Position	= Vector3(m_Pos.x + m_Size.x, m_Pos.y + m_Size.y, 0.0f);
	vertex[3].Normal	= Vector3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse	= Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord	= Vector2(texCoord.x + divValue.x, texCoord.y + divValue.y);

	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);
}

void Score::SetVertex(const Vector2& pos, const Vector2& size, const Vector2& texCoord, const Vector2& divValue)
{
	m_Pos.x = pos.x;
	m_Pos.y = pos.y;
	m_Size.x = size.x;
	m_Size.y = size.y;

	// 頂点データ書き換え
	D3D11_MAPPED_SUBRESOURCE msr;
	HRESULT hr = Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0,
		D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	vertex[0].Position	= Vector3(pos.x, pos.y, 0.0f);
	vertex[0].Normal	= Vector3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse	= Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord	= Vector2(texCoord.x, texCoord.y);

	vertex[1].Position	= Vector3(pos.x + size.x, pos.y, 0.0f);
	vertex[1].Normal	= Vector3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse	= Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord	= Vector2(texCoord.x + divValue.x, texCoord.y);

	vertex[2].Position	= Vector3(pos.x, pos.y + size.y, 0.0f);
	vertex[2].Normal	= Vector3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse	= Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord	= Vector2(texCoord.x, texCoord.y + divValue.y);

	vertex[3].Position	= Vector3(pos.x + size.x, pos.y + size.y, 0.0f);
	vertex[3].Normal	= Vector3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse	= Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord	= Vector2(texCoord.x + divValue.x, texCoord.y + divValue.y);

	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);
}

