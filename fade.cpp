//===========================================================================
// fade.cpp	fade����
// ����ҁF�쑺 �I�l
//===========================================================================
#include "main.h"
#include "renderer.h"
#include "fade.h"

void Fade::Init()
{
	m_Size = Vector3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
	m_Pos = Vector3(0.0f, 0.0f, 0.0f);


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

	// ���_�o�b�t�@����
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
}

void Fade::Uninit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	m_VertexBuffer->Release();
	m_Texture->Release();

	UninitComponent();
}

void Fade::Update()
{
	if (m_FadeState == FADE_OUT)
	{// �t�F�[�h�A�E�g����
		m_Color.a += FADE_RATE;		// ���l�����Z���ĉ�ʂ������Ă���

		if (m_Color.a >= 1.0f)
		{
			m_FadeEnd = true;
		}
	}
	else if (m_FadeState == FADE_IN)
	{// �t�F�[�h�C������
		m_Color.a -= FADE_RATE;		// ���l�����Z���ĉ�ʂ𕂂��オ�点��
		if (m_Color.a <= 0.0f)
		{
			Destroy();
		}
	}
}

void Fade::Draw()
{
	// ���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//�V�F�[�_�ݒ� 
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// �}�g���N�X�ݒ�
	Renderer::SetWorldViewProjection2D();

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// �}�e���A���ݒ�
	MATERIAL material{};
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = m_Color;
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	// �e�N�X�`���ݒ�
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	//�v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �|���S���`��
	Renderer::GetDeviceContext()->Draw(4, 0);
}

void Fade::SetTexture(const wchar_t* fileName)
{
	TexMetadata metadata;
	ScratchImage image;
	LoadFromWICFile(fileName, WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &m_Texture);
	assert(m_Texture);
}

void Fade::SetTexPos(const Vector2& pos)
{
	m_Pos.x = pos.x;
	m_Pos.y = pos.y;

	// ���_�f�[�^��������
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

void Fade::SetTexSize(const Vector2& size)
{
	m_Size.x = size.x;
	m_Size.y = size.y;

	// ���_�f�[�^��������
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

void Fade::SetFadeState(FADE_STATE state)
{
	switch (state)
	{
	case FADE_NONE:
		m_Color.a = 1.0f;
		m_FadeState = state;
		break;
	case FADE_IN:
		m_Color.a = 1.0f;
		m_FadeState = state;
		break;
	case FADE_OUT:
		m_Color.a = 0.0f;
		m_FadeState = state;
		break;
	default:
		break;
	}
}
