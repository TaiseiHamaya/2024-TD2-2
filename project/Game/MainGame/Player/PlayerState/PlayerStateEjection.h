#pragma once

#include "BasePlayerState.h"

#include <Engine/System/Performance.h>

namespace PlayerState {

class Ejection final : public BasePlayerState {
public:
	Ejection(const Vector3f& direction, float ejectDistance);
	~Ejection() = default;

public:
	void begin() override;
	Vector3f velocity() override;

	const std::string get_model_name() const override { return "player_spike_test.gltf"; };

private:
	float Time;
	DeltaTimePoint timer;
	Vector3f direction;
};

}
