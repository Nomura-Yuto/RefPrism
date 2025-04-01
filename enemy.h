//===========================================================================
// enemy.h					エネミー処理
// 製作者：野村 悠人
//===========================================================================
#pragma once
#include "gameObject.h"

class Enemy : public GameObject
{
private:
	const float DEF_SPEED = 2.0f;

	float m_Speed{};

	ID3D11InputLayout*	m_VertexLayout{};
	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader*	m_PixelShader{};



public:
	Enemy() = default;
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw()override;

	void SetSpeed(float speed) {
		m_Speed = speed;
	}
};
