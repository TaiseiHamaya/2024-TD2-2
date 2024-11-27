#pragma once

#include <Engine/Game/Behavior/AnimationBehavior.h>
#include <Engine/Game/Collider/Collider.h>

#include <memory>

#include <Game/MainGame/Effect/EffectLanding.h>
#include <Game/MainGame/Effect/Shadow.h>

class GameCamera;

class BaseBossBehavior;

struct AnimationModel {
	Model* model;
	std::unique_ptr<Animator> animator;
};

class Boss : public AnimationBehavior {
public:
	Boss();
	~Boss() noexcept;

	Boss(const Boss&) = delete;
	Boss& operator=(const Boss&) = delete;

public:
	void begin();
	void update();
	void update_matrix();

	void finalize();

	void SetAttributeImGui() override;

	Vector3f GetPosition() const { return transform_.GetWorldPosition(); }

	void CreateLandingParticle(const Vector3f& velocity);

	void HitReactionUpdate();

	void SetGacamera(GameCamera* camera) { camera_ = camera; }
	void ShakeCamera(DeltaTimePoint time, int32_t subdivision, float strength);

	void CreateAnimation(const std::string& filename);

public:
	bool is_end_behavior() const;
	void set_behavior(std::unique_ptr<BaseBossBehavior> behavior_);
	void set_phase_uv(uint32_t phase);

public:
	void take_damage(int32_t damage);
	void set_model(const std::string& file);
	bool is_dead() const;
	bool is_invincible() const;
	bool is_destroy() const;
	void set_invincible(bool val) { isInvincible = val; };
	Collider* get_hit_collider() const;
	Collider* get_attack_collider() const;
	QuaternionTransformBuffer& get_transform() { return transform_; };
	const Animator* const get_animator() const { return animator_; };
	const QuaternionTransformBuffer& get_transform() const { return transform_; };
	std::unique_ptr<BaseBossBehavior>& get_behavior() { return behavior; };
	const std::unique_ptr<BaseBossBehavior>& get_behavior() const { return behavior; };
	void hit_callback();
	void reset_hitpoint(uint32_t hitpoint_);

	int32_t GetHitPoint() const { return hitpoint; }

private:
	bool isInvincible{ false };
	bool isDestroy{ false };
	int32_t hitpoint;

	//Animator* animator;
	std::unique_ptr<BaseBossBehavior> behavior;

	DeltaTimePoint damagedInvincibleTimer;
	std::unique_ptr<Collider> collider_;

	std::unique_ptr<EffectLanding> landing_;

	GameCamera* camera_ = nullptr;

	std::unique_ptr<Shadow> shadow_;

	std::unordered_map<std::string, AnimationModel> models;
};
