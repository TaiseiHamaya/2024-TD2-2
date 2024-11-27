#pragma once

#include <Game/Scene/IScene.h>

#include <Game/MainGame/Field.h>
#include "ClearPlayer.h"

class ClearScene : public IScene {
public:

	//=========================================================================================
	// public methods
	//=========================================================================================

	void Init() override;

	void Term() override;

	void Update() override;

	void Draw() override;

private:
	std::unique_ptr<Field> field_;
	std::unique_ptr<ClearPlayer> clearPlayer;
};
