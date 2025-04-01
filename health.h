//===========================================================================
// health.h					�̗͏���
// ����ҁF�쑺 �I�l
//===========================================================================
#pragma once
#include "gameObject.h"

class Health : public GameObject
{
private:
	const int DIV_NUM_X = 5;	// ������(X)
	const int DIV_NUM_Y = 5;	// ������(Y)
	const int MAX_COUNT = DIV_NUM_X * DIV_NUM_Y;	// �؂�ւ���e�N�X�`���̍ő吔
	const int DIGIT_MAX = 2;	// �\�����錅��
	const int HEALTH_MAX = 10;

	const float DEF_SIZE_X = 300.0f;

	Vector4 m_Color = { 1.0f, 1.0f, 1.0f, 1.0f };

	Vector3 m_LeftTopPos{};
	Vector2 m_DivValue{};	// ������̕�
	int m_Count;

	ID3D11InputLayout*		m_VertexLayout{};
	ID3D11VertexShader*		m_VertexShader{};
	ID3D11PixelShader*		m_PixelShader{};

	ID3D11Buffer*			m_VertexBuffer{};
	ID3D11ShaderResourceView* m_Texture{};


public:
	Health() = default;
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void SetTexPos(const Vector2& pos);
	void SetLeftTopPos(const Vector2& pos);
	void SetTexSize(const Vector2& size);
	void SetTexCoord(const Vector2& texCoord, const Vector2& divValue);
	void SetVertex(const Vector2& pos, const Vector2& size, const Vector2& texCoord, const Vector2& divValue);
	void SetColor(const Vector4& color) {
		m_Color = color;
	}

	void SetHealth(int value) { m_Count = value; }
	void AddHealth(int value) { m_Count += value; }
	void SubHealth(int value) { m_Count -= value; }
	void ResetHealth() { m_Count = 0; }

	int GetCount() { return m_Count; }
};
