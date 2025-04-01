//===========================================================================
// component.h				Component���N���X
// ����ҁF�쑺 �I�l
//===========================================================================
#pragma once

class Component
{

protected:
	class GameObject* m_GameObject = nullptr;

public:
	Component() = delete;	// �����Ȃ��R���X�g���N�^�̍폜
	Component(GameObject* Object) { m_GameObject = Object; }
	virtual ~Component(){}

	virtual void Init() {}
	virtual void Uninit() {}
	virtual void Update() {}
	virtual void Draw() {}
};