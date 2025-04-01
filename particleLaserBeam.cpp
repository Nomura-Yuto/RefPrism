//===========================================================================
// particleLaserBeam.cpp	���[�U�[���ӂ̃p�[�e�B�N������
// ����ҁF�쑺 �I�l
//===========================================================================
#include "main.h"
#include "particleLaserBeam.h"
#include "renderer.h"
#include "manager.h"
#include "scene.h"
#include "camera.h"

void ParticleLaserBeam::Init()
{
	m_Scl = Vector3(0.3f, 0.3f, 1.0f);
	m_Color = Color(1.0f, 1.0f, 1.0f, 0.5f);

	VERTEX_3D vertex[4];

	vertex[0].Position	= Vector3(-0.5f, 0.5f, 0.0f);
	vertex[0].Normal	= Vector3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse	= Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord	= Vector2(0.0f, 0.0f);

	vertex[1].Position	= Vector3(0.5f, 0.5f, 0.0f);
	vertex[1].Normal	= Vector3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse	= Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord	= Vector2(1.0f, 0.0f);

	vertex[2].Position	= Vector3(-0.5f, -0.5f, 0.0f);
	vertex[2].Normal	= Vector3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse	= Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord	= Vector2(0.0f, 1.0f);

	vertex[3].Position	= Vector3(0.5f, -0.5f, 0.0f);
	vertex[3].Normal	= Vector3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse	= Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord	= Vector2(1.0f, 1.0f);

	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd{};
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd{};
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	// �e�N�X�`���ǂݍ���
	TexMetadata metadata;
	ScratchImage image;
	LoadFromWICFile(L"asset\\texture\\particleOrigin.png", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &m_Texture);
	assert(m_Texture);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTexturePS.cso");
}

void ParticleLaserBeam::Uninit()
{
	m_VertexBuffer->Release();
	m_Texture->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void ParticleLaserBeam::Update()
{
	Vector3 startPos = m_LaserObj->GetStartPos();
	Vector3 endPos = m_LaserObj->GetEndPos();
	Vector3 front = (endPos - startPos).Normalize();
	Vector3 up = Vector3(0.0f, 1.0f, 0.0f);
	Vector3 right = m_LaserObj->GetRight();

	// �p�[�e�B�N������
	for (int i = 0; i < PARTICLE_MAX; i++)
	{
		switch (m_Particle[i].Status) {
		case 0: // �ҋ@���
			if (!m_Particle[i].Enable) {
				m_Particle[i].Enable = true;
				m_Particle[i].Status = 1;
			}
			break;

		case 1:	// �����ݒ�
			if (m_Particle[i].Enable) {
				m_Particle[i].Status = 2;

				// �������W
				float t = Manager::Randf();
				Vector3 randPos = (t * startPos) + ((1 - t) * endPos);

				float angle = Manager::Randf(0.0f, XM_PI * 2.0f);
				Vector3 offset = Vector3(cosf(angle) + sinf(angle), 0.0f, cosf(angle) - sinf(angle)) * 0.3f;

				m_Particle[i].Pos = randPos + offset;

				// �����x
				float speed = Manager::Randf(0.0f, 2.0f);
				m_Particle[i].Vel = offset.Normalize() * speed;

				// �������x
				m_Particle[i].Acce = offset.Normalize() * -0.01;

				// �����ݒ�
				m_Particle[i].Life = Manager::Rand(30, 50);
			}

		case 2: // �ړ����
			if (m_Particle[i].Enable) {
				m_Particle[i].Vel += m_Particle[i].Acce;

				m_Particle[i].Pos += m_Particle[i].Vel * DT;

				// �����̌���
				m_Particle[i].Life--;

				// ������0�ɂȂ�����ҋ@��Ԃ�
				if (m_Particle[i].Life <= 0) {
					m_Particle[i].Status = 0;
					m_Particle[i].Enable = false;
				}

				break;
			}
		}
	}
}

void ParticleLaserBeam::Draw()
{
	if (m_LaserObj->GetLength() == 0.0f) return;

	// ���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//�V�F�[�_�ݒ� 
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// �J�����̃r���[�}�g���N�X�擾
	Scene* scene = Manager::GetScene();
	Camera* camera = scene->GetGameObject<Camera>();
	XMMATRIX view = camera->GetViewMatrix();

	// �r���[�̋t�s��
	XMMATRIX invView;
	invView = XMMatrixInverse(nullptr, view); // �t�s��
	invView.r[3].m128_f32[0] = 0.0f;
	invView.r[3].m128_f32[1] = 0.0f;
	invView.r[3].m128_f32[2] = 0.0f;

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = m_Color;
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	// �e�N�X�`���ݒ�
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	//�v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// Z�o�b�t�@����
	Renderer::SetDepthEnable(false);

	// ���Z�����L��
	Renderer::SetBlendAddEnable(true);

	for (int i = 0; i < PARTICLE_MAX; i++)
	{
		if (m_Particle[i].Enable) {
			// ���[���h�}�g���N�X�ݒ�
			XMMATRIX world, scale, trans;
			scale = XMMatrixScaling(m_Scl.x, m_Scl.y, m_Scl.z);
			trans = XMMatrixTranslation(m_Particle[i].Pos.x, m_Particle[i].Pos.y, m_Particle[i].Pos.z);
			world = scale * invView * trans;
			Renderer::SetWorldMatrix(world);

			// �|���S���`��
			Renderer::GetDeviceContext()->Draw(4, 0);
		}
	}

	// Z�o�b�t�@�L��
	Renderer::SetDepthEnable(true);

	// ���Z�����L��
	Renderer::SetBlendAddEnable(false);
}
