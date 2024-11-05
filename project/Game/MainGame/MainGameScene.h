#pragma once

#include <Game/Scene/IScene.h>

#include <Game/TestGround.h>
#include "Player/Player.h"
#include "Boss/BossManager.h"

class MainGameScene : public IScene {
public:
	void Init() override;

	void Term() override;

	void Update() override;

	void Draw() override;


private:
	std::unique_ptr<Player> player;
	std::unique_ptr<BossManager> bossManager;

#ifdef _DEBUG
	std::unique_ptr<TestGround> groundTest;
#endif // _DEBUG
};

