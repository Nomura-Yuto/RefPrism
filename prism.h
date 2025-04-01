//===========================================================================
// prism.h					プリズム処理
// 製作者：野村 悠人
//===========================================================================
#pragma once
#include "gameObject.h"

class Prism : public GameObject
{
private:
	ID3D11InputLayout*		m_VertexLayout{};
	ID3D11VertexShader*		m_VertexShader{};
	ID3D11PixelShader*		m_PixelShader{};

	ID3D11ShaderResourceView* m_EnvTexture{};

	class Audio* m_BGM{};

public:
	Prism() = default;
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw()override;

	void SetBGM(const char* fileName);
};
