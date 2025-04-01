//===========================================================================
// resultScene.h			���U���g�V�[������
// ����ҁF�쑺 �I�l
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
