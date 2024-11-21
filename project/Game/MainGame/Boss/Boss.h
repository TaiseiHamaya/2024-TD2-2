#pragma once

#include <Engine/Game/Behavior/AnimationBehavior.h>
#include <Engine/Game/Collider/Collider.h>

class BossBehavior;

class Boss : public AnimationBehavior {
public:
	Boss(int32_t hitpoint);
	~Boss() { finalize(); }

	Boss(const Boss&) = delete;
	Boss& operator=(const Boss&) = delete;

public:
	void initialize();
	void update();
	void update_matrix();

	void finalize();

	void SetAttributeImGui() override;

	Vector3f GetPosition() const { return transform_.GetWorldPosition(); }

public:
	void take_damage(int32_t damage);

public:
	void set_model(std::string file);
	bool is_dead() const;
	bool is_destroy() const;
	void set_invincible(bool val) { isInvincible = val; };

private:
	bool isInvincible{ false };
	bool isDestroy{false};
	int32_t hitpoint;

	//std::unique_ptr<BossBehavior> behavior;

	std::unique_ptr<Collider> collider_;
};
