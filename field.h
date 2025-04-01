//===========================================================================
// field.h					2Dフィールド処理
// 製作者：野村 悠人
//===========================================================================
#pragma once
#include "gameObject.h"

class Field : public GameObject
{
private:
	const float DIV_MAX = 1.0f;
	const float FIELD_SIZE = 180.0f;

	ID3D11Buffer*				m_VertexBuffer = NULL;
	ID3D11ShaderResourceView*	m_Texture = NULL;

	ID3D11InputLayout*		m_VertexLayout{};
	ID3D11VertexShader*		m_VertexShader{};
	ID3D11PixelShader*		m_PixelShader{};

public:
	Field() = default;
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw()override;
};
