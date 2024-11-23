#pragma once

#include <Engine/Game/Behavior/AnimationBehavior.h>
#include <Engine/Game/Collider/Collider.h>

#include <memory>

class BaseBossBehavior;

class Boss : public AnimationBehavior {
public:
	Boss(int32_t hitpoint);
	~Boss() noexcept;

	Boss(const Boss&) = delete;
	Boss& operator=(const Boss&) = delete;

public:
	void update();
	void update_matrix();

	void finalize();

	void SetAttributeImGui() override;

public:
	bool is_end_behavior() const;
	void set_behavior(std::unique_ptr<BaseBossBehavior> behavior_);

public:
	void take_damage(int32_t damage);
	void set_model(const std::string& file);
	bool is_dead() const;
	bool is_destroy() const;
	void set_invincible(bool val) { isInvincible = val; };
	const std::unique_ptr<Collider>& get_hit_collider() const { return collider_; };
	QuaternionTransformBuffer& get_transform() { return transform_; };
	const Animator* const get_animator() const { return animator; };
	const QuaternionTransformBuffer& get_transform() const { return transform_; };

private:
	bool isInvincible{ false };
	bool isDestroy{false};
	int32_t hitpoint;

	Animator* animator;
	std::unique_ptr<BaseBossBehavior> behavior;

	std::unique_ptr<Collider> collider_;
};
