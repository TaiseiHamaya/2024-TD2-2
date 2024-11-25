#pragma once

#include "BasePlayerState.h"

#include <Engine/System/Performance.h>

namespace PlayerState {

class Knockback final : public BasePlayerState {
public:
	Knockback(const Vector3f& direction_);
	~Knockback() = default;

public:
	void begin() override;
	Vector3f velocity() override;

	const std::string get_model_name() const override { return "player_knockBack.gltf"; };

private:
	DeltaTimePoint timer;
	Vector3f direction;
};

}
