//===========================================================================
// component.h				Component基底クラス
// 製作者：野村 悠人
//===========================================================================
#pragma once

class Component
{

protected:
	class GameObject* m_GameObject = nullptr;

public:
	Component() = delete;	// 引数なしコンストラクタの削除
	Component(GameObject* Object) { m_GameObject = Object; }
	virtual ~Component(){}

	virtual void Init() {}
	virtual void Uninit() {}
	virtual void Update() {}
	virtual void Draw() {}
};