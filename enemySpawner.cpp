//===========================================================================
// enemySpawner.cpp			エネミースポナー処理
// 製作者：野村 悠人
//===========================================================================
#include "main.h"
#include "enemySpawner.h"
#include "manager.h"
#include "enemy.h"

void EnemySpawner::Init()
{
	m_Pos = Vector3(0.0f, 0.0f, 0.0f);

	InitComponent();
}

void EnemySpawner::Uninit()
{

}

void EnemySpawner::Update()
{
	m_Time++;

	if (m_Time >= SPAWN_LATE) {
		m_Time = 0;

		Vector3 spawnPos;
		float spawnDistance = Manager::Randf(NOT_SPAWN_RADIUS, SPAWN_RADIUS);
		float spawnAngle = Manager::Randf(0.0f, 2.0f * XM_PI);

		spawnPos.x = cosf(spawnAngle) * spawnDistance;
		spawnPos.z = sinf(spawnAngle) * spawnDistance;
		spawnPos.y = 0.0f;

		Enemy* enemy = Manager::GetScene()->AddGameObject<Enemy>(LAYER_3D);
		enemy->SetSpeed(Manager::Randf(1.5f, 4.0f));
		enemy->SetPos(spawnPos);
	}
}

void EnemySpawner::Draw()
{

}
