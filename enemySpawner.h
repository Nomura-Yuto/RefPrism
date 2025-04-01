//===========================================================================
// enemySpawner.h			エネミースポナー処理
// 製作者：野村 悠人
//===========================================================================
#pragma once
#include "gameObject.h"

class EnemySpawner : public GameObject
{
private:
	const float SPAWN_RADIUS = 50.0f;
	const float NOT_SPAWN_RADIUS = 15.0f;
	const int SPAWN_LATE = 90;

	int m_Time{};

public:
	EnemySpawner() = default;
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw()override;
};
