#pragma once

#include "BasePlayerState.h"

class QuaternionTransformBuffer;

namespace PlayerState {

class Gather final : public BasePlayerState {
public:
	Gather(const QuaternionTransformBuffer* thisTransform_, const QuaternionTransformBuffer* target_);
	~Gather() = default;

public:
	void begin();
	Vector3f velocity();


private:
	const QuaternionTransformBuffer* thisTransform;
	const QuaternionTransformBuffer* target;
};

}

