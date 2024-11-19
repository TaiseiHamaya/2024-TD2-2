#pragma once

#include "BasePlayerState.h"

#include <Engine/System/Performance.h>

namespace PlayerState {

class Ejection final : public BasePlayerState {
public:
	Ejection(const Vector3f& direction);
	~Ejection() = default;

public:
	void begin();
	Vector3f velocity();

private:
	DeltaTimePoint timer;
	Vector3f direction;
};

}
