#pragma once

#include "BasePlayerState.h"

#include <Engine/System/Performance.h>

namespace PlayerState {

class Knockback final : public BasePlayerState {
public:
	Knockback(const Vector3f& direction_);
	~Knockback() = default;

public:
	void begin();
	Vector3f velocity();

private:
	DeltaTimePoint timer;
	Vector3f direction;
};

}
