//===========================================================================
// laserCannon.h			���[�U�[�C�䏈��
// ����ҁF�쑺 �I�l
//===========================================================================
#pragma once
#include "gameObject.h"
#include "laserBeam.h"

class LaserCannon : public GameObject
{
private:
	Vector3 m_FrontVec{};
	LaserBeam* m_Laser{};

	class Audio* m_BGM{};

	ID3D11InputLayout*		m_VertexLayout{};
	ID3D11VertexShader*		m_VertexShader{};
	ID3D11PixelShader*		m_PixelShader{};

public:
	LaserCannon() = default;
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw()override;

	void SetLaser(LaserBeam* obj) {
		m_Laser = obj;
	}
};
