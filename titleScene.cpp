//===========================================================================
// titleScene.cpp			タイトルシーン処理
// 製作者：野村 悠人
//===========================================================================
#include "titleScene.h"
#include "gameScene.h"
#include "manager.h"
#include "input.h"
#include "camera.h"
#include "polygon2D.h"
#include "fade.h"
#include "player.h"
#include "prism.h"
#include "laserBeam.h"

void TitleScene::Init()
{
	Manager::SetCurrentScene(SCENE_TITLE);
	AddGameObject<Fade>(LAYER_2D)->SetFadeState(FADE_IN);

	AddGameObject<Camera>(LAYER_SYSTEM);

	Prism* prism = AddGameObject<Prism>(LAYER_3D);
	prism->SetBGM("asset\\audio\\MusMus_BGM_cool01.wav");

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
		Polygon2D* titleRogo = AddGameObject<Polygon2D>(LAYER_2D);
		titleRogo->SetTexture(L"asset\\texture\\Title_RefPrism.png");
		titleRogo->SetTexPos(Vector2(50.0f, 50.0f));
		titleRogo->SetTexSize(Vector2(700.0f, 200.0f));

		Polygon2D* startText = AddGameObject<Polygon2D>(LAYER_2D);
		startText->SetTexture(L"asset\\texture\\PressEnter.png");
		startText->SetTexPos(Vector2(500.0f, 550.0f));
		startText->SetTexSize(Vector2(700.0f, 100.0f));
	}
}

void TitleScene::Update()
{
	Scene::Update();

	if (Input::GetKeyTrigger(VK_RETURN)) {
		AddGameObject<Fade>(LAYER_2D)->SetFadeState(FADE_OUT);
	}

	// フェードが終了したら次のシーンへ
	if (GetGameObject<Fade>()) {
		if (GetGameObject<Fade>()->FadeEnd()) {
			Manager::SetScene<GameScene>();
		}
	}
}
