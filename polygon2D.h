//===========================================================================
// polygon2D.h				2Dポリゴン表示
// 製作者：野村 悠人
//===========================================================================
#pragma once
#include "gameObject.h"

class Polygon2D : public GameObject
{
private:
	Vector4 m_Color = { 1.0f, 1.0f, 1.0f, 1.0f };

	ID3D11InputLayout*		m_VertexLayout{};
	ID3D11VertexShader*		m_VertexShader{};
	ID3D11PixelShader*		m_PixelShader{};
	ID3D11Buffer*			m_VertexBuffer{};
	ID3D11ShaderResourceView*	m_Texture{};

	class Audio*			m_BGM{};

public:
	Polygon2D() = default;
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void SetTexture(const wchar_t* fileName);
	void SetTexPos(const Vector2& pos);
	void SetTexSize(const Vector2& size);
	void SetColor(const Vector4& color) {
		m_Color = color;
	}
};
