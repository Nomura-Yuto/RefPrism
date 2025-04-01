//===========================================================================
// gameScene.cpp			ゲームシーン処理
// 製作者：野村 悠人
//===========================================================================
#include "titleScene.h"
#include "gameScene.h"
#include "resultScene.h"
#include "manager.h"
#include "input.h"

#include "camera.h"
#include "field.h"
#include "player.h"
#include "laserBeam.h"
#include "laserCannon.h"
#include "obstacle.h"
#include "enemy.h"
#include "enemySpawner.h"
#include "particleLaserBeam.h"
#include "particleLaserPort.h"
#include "score.h"
#include "health.h"
#include "polygon2D.h"
#include "fade.h"

void GameScene::Init()
{
	Manager::SetCurrentScene(SCENE_GAME);
	AddGameObject<Fade>(LAYER_2D)->SetFadeState(FADE_IN);

	AddGameObject<Camera>(LAYER_SYSTEM);
	AddGameObject<EnemySpawner>(LAYER_SYSTEM);

	AddGameObject<Field>(LAYER_3D);
	Player* player = AddGameObject<Player>(LAYER_3D);

	LaserBeam* laser = AddGameObject<LaserBeam>(LAYER_3D);
	laser->SetTargetObj(player);

	LaserBeam* laserRef = AddGameObject<LaserBeam>(LAYER_3D);
	laserRef->SetTargetObj(player);
	laserRef->SetPrevLaser(laser);
	laserRef->SetLaserState(STATE_REFLECT);


	Color particleColor = laser->GetColor() + Color(0.2f, 0.2f, 0.2f, 0.0f);
	{
		ParticleLaserBeam* particle = AddGameObject<ParticleLaserBeam>(LAYER_3D);
		particle->SetLaserObject(laser);
		particle->SetColor(particleColor);
	}
	{
		ParticleLaserBeam* particle = AddGameObject<ParticleLaserBeam>(LAYER_3D);
		particle->SetLaserObject(laserRef);
		particle->SetColor(particleColor);
	}
	{
		ParticleLaserPort* particle = AddGameObject<ParticleLaserPort>(LAYER_3D);
		particle->SetLaserObject(laser);
		particle->SetColor(particleColor);
	}

	AddGameObject<LaserCannon>(LAYER_3D)->SetLaser(laser);

	for (int i = 0; i < 20; i++) {
		Vector3 spawnPos;
		float spawnDistance = Manager::Randf(5.0f, 25.0f);
		float spawnAngle = Manager::Randf(0.0f, 2.0f * XM_PI);

		spawnPos.x = cosf(spawnAngle) * spawnDistance;
		spawnPos.z = sinf(spawnAngle) * spawnDistance;
		spawnPos.y = 0.0f;
		AddGameObject<Obstacle>(LAYER_3D)->SetPos(spawnPos);
	}

	AddGameObject<EnemySpawner>(LAYER_SYSTEM);
	AddGameObject<Enemy>(LAYER_3D)->SetPos(Vector3(10.0f, 0.0f, 10.0f));
	AddGameObject<Enemy>(LAYER_3D)->SetPos(Vector3(10.0f, 0.0f, -10.0f));
	AddGameObject<Enemy>(LAYER_3D)->SetPos(Vector3(-10.0f, 0.0f, 10.0f));
	AddGameObject<Enemy>(LAYER_3D)->SetPos(Vector3(-10.0f, 0.0f, -10.0f));

	AddGameObject<Score>(LAYER_2D)->SetTexPos(Vector2(1000.0f, 0.0f));
	AddGameObject<Health>(LAYER_2D)->SetTexPos(Vector2(0.0f, 0.0f));

	Polygon2D* p = AddGameObject<Polygon2D>(LAYER_2D);
	p->SetTexture(L"asset\\texture\\guide.png");
	p->SetTexPos(Vector2(0.0f, 650.0f));
	p->SetTexSize(Vector2(SCREEN_WIDTH, 80.0f));
}

void GameScene::Update()
{
	Scene::Update();

	int count = GetGameObject<Health>()->GetCount();

	if (count <= 0 || Input::GetKeyTrigger(VK_RETURN)) {
		AddGameObject<Fade>(LAYER_2D)->SetFadeState(FADE_OUT);
	}

	// フェードが終了したら次のシーンへ
	if (GetGameObject<Fade>()) {
		if (GetGameObject<Fade>()->FadeEnd()) {
			ResultScene* scene = Manager::SetScene<ResultScene>();
			int score = GetGameObject<Score>()->GetScore();
			scene->SetScore(score);
		}
	}
}
