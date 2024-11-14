#pragma once

#include <memory>

#include <Engine/Game/Behavior/ModelBehavior.h>
#include <Engine/Game/Collider/Collider.h>
#include <Lib/Geometry/Vector2.h>

#include "./PlayerState/BasePlayerState.h"

class Player : public ModelBehavior {
public:
	void initialize(const Vector3f& translate = kOrigin3);

	void begin();
	void update();
	void update_matrix();

	void operate_update(const Vector2f& input);

public:
	Vector3f world_point() const;
	const QuaternionTransform& get_transform() const { return transform_.transform; };
	void set_state(std::unique_ptr<BasePlayerState> state_) { state = std::move(state_); };

private:

private:
	Vector3f velocity{ kOrigin3 };
	std::unique_ptr<BasePlayerState> state;

	std::unique_ptr<Collider> collider_;

private:
	inline static uint32_t index = 0;
};
