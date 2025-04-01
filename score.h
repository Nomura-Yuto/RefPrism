//===========================================================================
// score.h					スコア表示
// 製作者：野村 悠人
//===========================================================================
#pragma once
#include "gameObject.h"

class Score : public GameObject
{
private:
	const int DIV_NUM_X = 5;	// 分割数(X)
	const int DIV_NUM_Y = 5;	// 分割数(Y)
	const int MAX_COUNT = DIV_NUM_X * DIV_NUM_Y;	// 切り替えるテクスチャの最大数
	const int DIGIT_MAX = 4;	// 表示する桁数

	Vector4 m_Color = { 1.0f, 1.0f, 1.0f, 1.0f };

	Vector3 m_LeftTopPos{};
	Vector2 m_DivValue{};	// 分割後の幅
	int m_Count;

	ID3D11InputLayout*		m_VertexLayout{};
	ID3D11VertexShader*		m_VertexShader{};
	ID3D11PixelShader*		m_PixelShader{};

	ID3D11Buffer*			m_VertexBuffer{};
	ID3D11ShaderResourceView* m_Texture{};


public:
	Score() = default;
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

	void SetScore(int value) { m_Count = value; }
	void AddScore(int value) { m_Count += value; }
	void ResetScore() { m_Count = 0; }

	int GetScore() { return m_Count; }
};
