//===========================================================================
// obstacle.h				áŠQ•¨ˆ—
// »ìÒF–ì‘º —Il
//===========================================================================
#pragma once
#include "gameObject.h"

class Obstacle : public GameObject
{
private:
	ID3D11InputLayout*	m_VertexLayout{};
	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader*	m_PixelShader{};


public:
	Obstacle() = default;
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw()override;
};
