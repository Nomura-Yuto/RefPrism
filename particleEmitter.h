//===========================================================================
// particleEmitter.h		パーティクル装置処理
// 製作者：野村 悠人
//===========================================================================
#pragma once
#include "gameObject.h"


class ParticleEmitter : public GameObject
{
private:
	Vector3 m_StartPos{};
	Vector3 m_EndPos{};
	float m_Length{};

	GameObject* m_StartPosObj{};
	GameObject* m_EndPosObj{};

	ID3D11Buffer* m_VertexBuffer{};
	ID3D11ShaderResourceView* m_Texture{};

	ID3D11InputLayout* m_VertexLayout;
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;

	struct PARTICLE
	{
		XMFLOAT3 Pos{};
		XMFLOAT3 Vel{};
		XMFLOAT3 TargetPos{};

		int Status{};	// 状態
		int Life;		// 寿命
		bool Enable{};	// 有効か
	};

	static const int PARTICLE_MAX = 100;
	PARTICLE m_Particle[PARTICLE_MAX]{};

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw()override;

	ID3D11ShaderResourceView* GetTexture() const {
		return m_Texture;
	}

	void SetStartPos(const Vector3& pos){
		m_StartPos = pos;
	}
	void SetEndPos(const Vector3& pos) {
		m_EndPos = pos;
	}

	void SetStartPosObject(GameObject* obj) {
		m_StartPosObj = obj;
	}
	void SetEndPosObject(GameObject* obj) {
		m_EndPosObj = obj;
	}

	void SetTexture(ID3D11ShaderResourceView* texture) {
		m_Texture = texture;
	}
};
