//===========================================================================
// gameObject.h				GameObject���N���X
// ����ҁF�쑺 �I�l
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

	// �R���|�[�l���g�̒ǉ�
	void AddComponent(Component* comp) {
		comp->Init();
		m_Componets.push_back(comp);
	}

	// �w�肵���R���|�[�l���g�̍폜
	void RemoveComponent(Component* comp) {
		m_Componets.remove_if([comp](const Component* p) {
			if (p == comp) {
				delete p;
				return true;
			}
			return false;
		});
	}

	// �w�肵���^�̃R���|�[�l���g�̎擾
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

	// �I�u�W�F�N�g�폜�̗\��
	void Destroy() { m_Destroy = true; }
	// �I�u�W�F�N�g�̍폜
	bool IsDestroy() {
		if (m_Destroy) {
			Uninit();
			delete this;
			return true;
		}
		return false;
	}

	//----- �Q�b�^�[ -----
	Vector3 GetPos() const { return m_Pos; }
	Vector3 GetRot() const { return m_Rot; }
	Vector3 GetScl() const { return m_Scl; }
	Vector3 GetSize() const { return m_Size; }
	float GetRadius() const { return m_Radius; }

	// ������x�N�g���̎擾
	Vector3 GetUp() {
		// �I�C���[�p�����]�s��𐶐�
		XMMATRIX rotationMatrix;
		rotationMatrix = XMMatrixRotationRollPitchYaw(
			m_Rot.x, m_Rot.y, m_Rot.z);

		Vector3 up;
		StoreVector3(&up, rotationMatrix.r[1]);
		return up;
	}

	// �O�����x�N�g���̎擾
	Vector3 GetFoward() {
		// �I�C���[�p�����]�s��𐶐�
		XMMATRIX rotationMatrix;
		rotationMatrix = XMMatrixRotationRollPitchYaw(
			m_Rot.x, m_Rot.y, m_Rot.z);

		Vector3 foward;
		StoreVector3(&foward, rotationMatrix.r[2]);
		return foward;
	}

	// �E�����x�N�g���̎擾
	Vector3 GetRight() {
		// �I�C���[�p�����]�s��𐶐�
		XMMATRIX rotationMatrix;
		rotationMatrix = XMMatrixRotationRollPitchYaw(
			m_Rot.x, m_Rot.y, m_Rot.z);

		Vector3 right;
		StoreVector3(&right, rotationMatrix.r[0]);
		return right;
	}

	// �w�肵�����W�Ƃ̋����̎擾(XZ���W�ł̌v�Z)
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

	// �w�肵��ViewMatrix����̋������擾
	float GetDistance(const XMMATRIX view) const {
		// �w�肵��ViewMatrix�̃x�N�g�����擾
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


	//----- �Z�b�^�[ -----
	void SetPos(const Vector3 pos) { m_Pos = pos; }
	void SetRot(const Vector3 rot) { m_Rot = rot; }
	void SetScl(const Vector3 scl) { m_Scl = scl; }
	void SetSize(const Vector3 size) { m_Size = size; }
	void SetRadius(const float radius) { m_Radius = radius; }
};
