//===========================================================================
// resultScene.cpp			リザルトシーン処理
// 製作者：野村 悠人
//===========================================================================
#include "titleScene.h"
#include "resultScene.h"
#include "manager.h"
#include "input.h"
#include "camera.h"
#include "polygon2D.h"
#include "fade.h"
#include "player.h"
#include "prism.h"
#include "laserBeam.h"
#include "score.h"

void ResultScene::Init()
{
	Manager::SetCurrentScene(SCENE_RESULT);
	AddGameObject<Fade>(LAYER_2D)->SetFadeState(FADE_IN);

	AddGameObject<Camera>(LAYER_SYSTEM);

	Prism* prism = AddGameObject<Prism>(LAYER_3D);
	prism->SetBGM("asset\\audio\\MusMus_BGM_cool03.wav");

	{
		Player* player = AddGameObject<Player>(LAYER_3D);
		player->SetPos(Vector3(12.0f, 0.0f, -3.0f));
		player->SetRot(Vector3(0.0f, XM_PI / 2, 0.0f));

		LaserBeam* laser = AddGameObject<LaserBeam>(LAYER_3D);
		laser->SetTargetObj(player);
		laser->SetColor(Color(0.5f, 0.0f, 0.0f, 1.0f));

		LaserBeam* laserRef = AddGameObject<LaserBeam>(LAYER_3D);
		laserRef->SetTargetObj(player);
		laserRef->SetPrevLaser(laser);
		laserRef->SetColor(Color(0.5f, 0.0f, 0.0f, 1.0f));
		laserRef->SetLaserState(STATE_REFRACT);
	}
	{
		Player* player = AddGameObject<Player>(LAYER_3D);
		player->SetPos(Vector3(15.0f, 0.0f, 10.0f));
		player->SetRot(Vector3(0.0f, XM_PI / 9, 0.0f));

		LaserBeam* laser = AddGameObject<LaserBeam>(LAYER_3D);
		laser->SetTargetObj(player);
		laser->SetColor(Color(0.0f, 0.5f, 0.0f, 1.0f));

		LaserBeam* laserRef = AddGameObject<LaserBeam>(LAYER_3D);
		laserRef->SetTargetObj(player);
		laserRef->SetPrevLaser(laser);
		laserRef->SetColor(Color(0.0f, 0.5f, 0.0f, 1.0f));
		laserRef->SetLaserState(STATE_REFLECT);
	}
	{
		Player* player = AddGameObject<Player>(LAYER_3D);
		player->SetPos(Vector3(-13.0f, 0.0f, -10.0f));
		player->SetRot(Vector3(0.0f, XM_PI / 6, 0.0f));

		LaserBeam* laser = AddGameObject<LaserBeam>(LAYER_3D);
		laser->SetTargetObj(player);
		laser->SetColor(Color(0.0f, 0.0f, 0.5f, 1.0f));

		LaserBeam* laserRef = AddGameObject<LaserBeam>(LAYER_3D);
		laserRef->SetTargetObj(player);
		laserRef->SetPrevLaser(laser);
		laserRef->SetColor(Color(0.0f, 0.0f, 0.5f, 1.0f));
		laserRef->SetLaserState(STATE_REFLECT);
	}
	{
		Player* player = AddGameObject<Player>(LAYER_3D);
		player->SetPos(Vector3(-17.0f, 0.0f, 2.0f));
		player->SetRot(Vector3(0.0f, XM_PI / 3, 0.0f));

		LaserBeam* laser = AddGameObject<LaserBeam>(LAYER_3D);
		laser->SetTargetObj(player);
		laser->SetColor(Color(0.5f, 0.5f, 0.5f, 1.0f));

		LaserBeam* laserRef = AddGameObject<LaserBeam>(LAYER_3D);
		laserRef->SetTargetObj(player);
		laserRef->SetPrevLaser(laser);
		laserRef->SetColor(Color(0.5f, 0.5f, 0.5f, 1.0f));
		laserRef->SetLaserState(STATE_REFRACT);
	}
	{
		Polygon2D* scoreText = AddGameObject<Polygon2D>(LAYER_2D);
		scoreText->SetTexture(L"asset\\texture\\scoreText.png");
		scoreText->SetTexPos(Vector2(330.0f, 100.0f));
		scoreText->SetTexSize(Vector2(300.0f, 140.0f));

		Score* score = AddGameObject<Score>(LAYER_2D);
		score->SetTexPos(Vector2(400.0f, 200.0f));
		score->SetTexSize(Vector2(200.0f, 200.0f));
		score->SetScore(m_Score);

		Polygon2D* toTitleText = AddGameObject<Polygon2D>(LAYER_2D);
		toTitleText->SetTexture(L"asset\\texture\\toTitle.png");
		toTitleText->SetTexPos(Vector2(500.0f, 550.0f));
		toTitleText->SetTexSize(Vector2(700.0f, 100.0f));
	}
}

void ResultScene::Update()
{
	Scene::Update();

	if (Input::GetKeyTrigger(VK_RETURN)) {
		AddGameObject<Fade>(LAYER_2D)->SetFadeState(FADE_OUT);
	}

	// フェードが終了したら次のシーンへ
	if (GetGameObject<Fade>()) {
		if (GetGameObject<Fade>()->FadeEnd()) {
			Manager::SetScene<TitleScene>();
		}
	}
}
