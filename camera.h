//===========================================================================
// camera.h					ƒJƒƒ‰ˆ—
// »ìÒF–ì‘º —Il
//===========================================================================
#pragma once
#include "gameObject.h"
#include "scene.h"

class Camera : public GameObject
{
private:
	const float m_HEIGHT = 5.0f;
	const float m_DISTANCE = 30.0f;

	Vector3 m_Target{};

	XMFLOAT4X4 m_ViewMatrix{};

	SCENE_NAME m_CurrentScene{};

public:
	Camera() = default;
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void SetScenePosition(SCENE_NAME scene);

	XMMATRIX GetViewMatrix() const{
		return XMLoadFloat4x4(&m_ViewMatrix);
	}
};