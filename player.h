//===========================================================================
// player.h					�v���C���[����
// ����ҁF�쑺 �I�l
//===========================================================================
#pragma once
#include "gameObject.h"

class Player : public GameObject
{
private:
	const float DEF_SPEED = 10.0f;	// �f�t�H���g�̃X�s�[�h
	const float PLAY_FIELD_SIZE = 50.0f;

	Vector3 m_Vel{};
	Vector3 m_OldPos{};
	Vector3 m_TargetPos{};

	float m_AnimTime{};
	bool isGround{};

	ID3D11InputLayout*		m_VertexLayout{};
	ID3D11VertexShader*		m_VertexShader{};
	ID3D11PixelShader*		m_PixelShader{};

	ID3D11ShaderResourceView* m_EnvTexture{};

	class Audio*			m_SE{};

public:
	Player() = default;
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw()override;
};
