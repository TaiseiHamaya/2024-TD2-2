#pragma once

#include <Game/Scene/IScene.h>

#include <Game/TestGround.h>
#include "Player/PlayerManager.h"
#include "Boss/BossManager.h"
#include "Field.h"

class MainGameScene : public IScene {
public:
	void Init() override;

	void Term() override;

	void Update() override;

	void Draw() override;


private:
	void collision();

	void collision_boss_attack();
	void collision_player_attack();

private:
	std::unique_ptr<PlayerManager> playerManager;
	std::unique_ptr<BossManager> bossManager;
	std::unique_ptr<Field> field_;

};

