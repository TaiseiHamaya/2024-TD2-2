#pragma once

#include <memory>

#include <Engine/Game/Behavior/AnimationBehavior.h>
#include <Engine/Game/Collider/Collider.h>
#include <Lib/Geometry/Vector2.h>

#include "./PlayerState/BasePlayerState.h"

class Player : public AnimationBehavior {
public:
	void initialize(const Vector3f& translate, float size);

	void begin();
	void update();
	void update_matrix();

	void operate_update(const Vector2f& input);
	void ungather();
	void take_damage();

public:
	Vector3f world_point() const;
	const QuaternionTransformBuffer& get_transform() const { return transform_; };
	void push_state(std::unique_ptr<BasePlayerState> state_);
	bool empty_state();
	const std::unique_ptr<Collider>& get_hit_collider() const { return hitCollider; };
	Collider* get_attack_collider() const;
	void set_sizing(float size_);
	float get_size() const { return size; };

private:
	void set_model(const std::string& file);

public:
	void SystemAttributeImGui() override;

private:
	float size;
	float scaling;
	Vector3f velocity{ kOrigin3 };
	std::list<std::unique_ptr<BasePlayerState>> stateQue;

	std::unique_ptr<Collider> hitCollider;

	Animator* animator{ nullptr };
	DeltaTimePoint animationTimer;

	bool isInvincible;
	DeltaTimePoint invincibleTimer;
	DeltaTimePoint DefaultInvincibleTime;

private:
	inline static uint32_t index = 0;

private:
	static float CreateScale(float size);

public:
	inline static float maxSize{ 5.0f };
	inline static float minSize{ 0.5f };
	inline static float ModelSize{ 2.0f };
	inline static float DefaultSize{ 5.0f };
	inline static float SizeParSec{ 1.f };
};
