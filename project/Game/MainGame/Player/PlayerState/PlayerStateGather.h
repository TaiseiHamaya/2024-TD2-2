#pragma once

#include "BasePlayerState.h"

class QuaternionTransformBuffer;
class PlayerManager;

namespace PlayerState {

class Gather final : public BasePlayerState {
public:
	Gather(const QuaternionTransformBuffer* thisTransform_);
	~Gather() = default;

public:
	void begin() override;
	Vector3f velocity() override;

	const std::string get_model_name() const override { return "player_spike_test.gltf"; };

private:
	const QuaternionTransformBuffer* thisTransform;

public:
	inline static const PlayerManager* playerManager{ nullptr };
};

}

