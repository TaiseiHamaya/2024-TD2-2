#pragma once

#include <memory>

#include <Engine/Game/Behavior/ModelBehavior.h>
#include <Engine/Game/Collider/Collider.h>
#include <Lib/Geometry/Vector2.h>

#include "./PlayerState/BasePlayerState.h"

class Player : public ModelBehavior {
public:
	void initialize(const Vector3f& translate, float scaling);

	void begin();
	void update();
	void update_matrix();

	void operate_update(const Vector2f& input);
	void ungather();

public:
	Vector3f world_point() const;
	const QuaternionTransformBuffer& get_transform() const { return transform_; };
	void push_state(std::unique_ptr<BasePlayerState> state_) { stateQue.emplace_back(std::move(state_)); };
	bool empty_state();
	const std::unique_ptr<Collider>& get_collider() const { return collider_; };
	void set_scaling(float scale_);
	float get_scaling() const { return scaling; };

private:

public:
	void SystemAttributeImGui() override;

private:
	float scaling;
	Vector3f velocity{ kOrigin3 };
	std::list<std::unique_ptr<BasePlayerState>> stateQue;

	std::unique_ptr<Collider> collider_;

private:
	inline static uint32_t index = 0;
};
