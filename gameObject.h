//===========================================================================
// gameObject.h				GameObject基底クラス
// 製作者：野村 悠人
//===========================================================================
#pragma once
#include <list>
#include "component.h"
#include "vector.h"


class GameObject
{
protected:
	const float DT = 1.0f / 60.0f;

	Vector3 m_Pos = { 0.0f,0.0f,0.0f };
	Vector3 m_Rot = { 0.0f,0.0f,0.0f };
	Vector3 m_Scl = { 1.0f,1.0f,1.0f };
	Vector3 m_Size = { 1.0f,1.0f,1.0f };

	float m_Radius = 0.0f;

	bool m_Destroy = false;

	std::list<Component*> m_Componets;

public:
	GameObject() = default;
	virtual ~GameObject() {
		for (auto a : m_Componets) {
			a->Uninit();
			delete a;
		}
		m_Componets.clear();
	}

	virtual void Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	void InitComponent(){
		for (auto comp : m_Componets) {
			comp->Init();
		}
	}
	void UninitComponent(){
		for (auto comp : m_Componets) {
			comp->Uninit();
		}
		m_Componets.clear();
	}
	void UpdateComponent(){
		for (auto comp : m_Componets) {
			comp->Update();
		}
	}
	void DrawComponent(){
		for (auto comp : m_Componets) {
			comp->Draw();
		}
	}

	// コンポーネントの追加
	void AddComponent(Component* comp) {
		comp->Init();
		m_Componets.push_back(comp);
	}

	// 指定したコンポーネントの削除
	void RemoveComponent(Component* comp) {
		m_Componets.remove_if([comp](const Component* p) {
			if (p == comp) {
				delete p;
				return true;
			}
			return false;
		});
	}

	// 指定した型のコンポーネントの取得
	template <typename T>
	T* GetComponent() {
		for (auto comp : m_Componets) {
			T* ret = dynamic_cast<T*>(comp);
			if (ret != nullptr) { 
				return ret; 
			}
		}
		return nullptr;
	}

	// オブジェクト削除の予約
	void Destroy() { m_Destroy = true; }
	// オブジェクトの削除
	bool IsDestroy() {
		if (m_Destroy) {
			Uninit();
			delete this;
			return true;
		}
		return false;
	}

	//----- ゲッター -----
	Vector3 GetPos() const { return m_Pos; }
	Vector3 GetRot() const { return m_Rot; }
	Vector3 GetScl() const { return m_Scl; }
	Vector3 GetSize() const { return m_Size; }
	float GetRadius() const { return m_Radius; }

	// 上方向ベクトルの取得
	Vector3 GetUp() {
		// オイラー角から回転行列を生成
		XMMATRIX rotationMatrix;
		rotationMatrix = XMMatrixRotationRollPitchYaw(
			m_Rot.x, m_Rot.y, m_Rot.z);

		Vector3 up;
		StoreVector3(&up, rotationMatrix.r[1]);
		return up;
	}

	// 前方向ベクトルの取得
	Vector3 GetFoward() {
		// オイラー角から回転行列を生成
		XMMATRIX rotationMatrix;
		rotationMatrix = XMMatrixRotationRollPitchYaw(
			m_Rot.x, m_Rot.y, m_Rot.z);

		Vector3 foward;
		StoreVector3(&foward, rotationMatrix.r[2]);
		return foward;
	}

	// 右方向ベクトルの取得
	Vector3 GetRight() {
		// オイラー角から回転行列を生成
		XMMATRIX rotationMatrix;
		rotationMatrix = XMMatrixRotationRollPitchYaw(
			m_Rot.x, m_Rot.y, m_Rot.z);

		Vector3 right;
		StoreVector3(&right, rotationMatrix.r[0]);
		return right;
	}

	// 指定した座標との距離の取得(XZ座標での計算)
	float GetDistanceXZ(const Vector3 pos) const {
		Vector2 dist;
		dist.x = m_Pos.x - pos.x;
		dist.y = m_Pos.z - pos.z;

		return dist.Length();
	}
	float GetDistance(const Vector3 pos) const {
		Vector3 dist = m_Pos - pos;
		return dist.Length();
	}

	// 指定したViewMatrixからの距離を取得
	float GetDistance(const XMMATRIX view) const {
		// 指定したViewMatrixのベクトルを取得
		Vector3 dir;
		dir.x = view.r[0].m128_f32[2];
		dir.y = view.r[1].m128_f32[2];
		dir.z = view.r[2].m128_f32[2];

		Vector3 cameraPos;
		cameraPos.x = view.r[3].m128_f32[0];
		cameraPos.y = view.r[3].m128_f32[1];
		cameraPos.z = view.r[3].m128_f32[2];

		Vector3 dist;
		dist = m_Pos - cameraPos;

		return dir.Dot(dist);
	}


	//----- セッター -----
	void SetPos(const Vector3 pos) { m_Pos = pos; }
	void SetRot(const Vector3 rot) { m_Rot = rot; }
	void SetScl(const Vector3 scl) { m_Scl = scl; }
	void SetSize(const Vector3 size) { m_Size = size; }
	void SetRadius(const float radius) { m_Radius = radius; }
};
