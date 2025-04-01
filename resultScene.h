//===========================================================================
// resultScene.h			リザルトシーン処理
// 製作者：野村 悠人
//===========================================================================
#pragma once
#include "scene.h"

class ResultScene : public Scene
{
private:
	int m_Score;

public:
	void Init() override;
	void Update() override;

	void SetScore(int score) {
		m_Score = score;
	}
};
