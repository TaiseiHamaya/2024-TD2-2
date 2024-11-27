#pragma once

#include "BasePlayerState.h"

namespace PlayerState {

class Spawn : public BasePlayerState {
public:
	Spawn();
	~Spawn() = default;

public:
	void begin() override;
	Vector3f velocity() override;

	const std::string get_model_name() const override { return "player_knockBack.gltf"; };

private:
	float startY;
	float displacement;
	float velocityY;
};

}
