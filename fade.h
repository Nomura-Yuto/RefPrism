//===========================================================================
// fade.h	fade処理
// 製作者：野村 悠人
//===========================================================================
#pragma once
#include "gameObject.h"

enum FADE_STATE
{
	FADE_NONE = 0,
	FADE_IN,		// フェードイン
	FADE_OUT,		// フェードアウト
	FADE_MAX
};


class Fade : public GameObject
{
private:
	const float FADE_RATE = 0.05f;

	Color m_Color = { 1.0f, 1.0f, 1.0f, 0.0f };

	FADE_STATE m_FadeState = FADE_NONE;
	bool m_FadeIn{};
	bool m_FadeEnd{};

	ID3D11InputLayout* m_VertexLayout{};
	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11Buffer* m_VertexBuffer{};
	ID3D11ShaderResourceView* m_Texture{};

public:
	Fade() = default;
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void SetTexture(const wchar_t* fileName);
	void SetTexPos(const Vector2& pos);
	void SetTexSize(const Vector2& size);
	void SetColor(const Color& color) {
		m_Color = color;
	}
	void SetFadeState(FADE_STATE state);
	bool FadeEnd() {
		return m_FadeEnd;
	}
};
