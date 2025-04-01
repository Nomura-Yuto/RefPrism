//===========================================================================
// particleLaserPort.cpp	���[�U�[���ˌ��̃p�[�e�B�N������
// ����ҁF�쑺 �I�l
//===========================================================================
#include "main.h"
#include "particleLaserPort.h"
#include "renderer.h"
#include "manager.h"
#include "scene.h"
#include "camera.h"
#include "laserCannon.h"

void ParticleLaserPort::Init()
{
	m_Scl = Vector3(0.5f, 0.5f, 1.0f);
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

void ParticleLaserPort::Uninit()
{
	m_VertexBuffer->Release();
	m_Texture->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void ParticleLaserPort::Update()
{
	Vector3 objPos = m_LaserObj->GetStartPos();
	Vector3 frontVec = m_LaserObj->GetFrontVec().Normalize();
	float baseScl = Manager::GetScene()->GetGameObject<LaserCannon>()->GetScl().x;

	float maxAngle = XM_PI / 180.0f * 60.0f;

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
				m_Particle[i].Pos = objPos + frontVec * baseScl;

				float angle = Manager::Randf(-maxAngle, maxAngle);
				float speed = Manager::Randf(3.0f, 6.0f);

				float cosTheta = cos(angle);
				float sinTheta = sin(angle);

				// �����x
				m_Particle[i].Vel.x = cosTheta * frontVec.x + sinTheta * frontVec.z;
				m_Particle[i].Vel.y = frontVec.y;
				m_Particle[i].Vel.z = cosTheta * frontVec.z - sinTheta * frontVec.x;

				m_Particle[i].Vel = m_Particle[i].Vel * speed;

				// �������x
				m_Particle[i].Acce.y = -0.2f;


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

void ParticleLaserPort::Draw()
{
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
