#pragma once

#include <Engine/Game/Behavior/BaseBehavior.h>

#include "Boss.h"
#include "BossActionManager/BossActionManager.h"

class PlayerManager;
class PlayerAutomationPop;

class BossManager : public BaseBehavior {
public:
	BossManager() = default;
	~BossManager() = default;

	BossManager(const BossManager&) = delete;
	BossManager& operator=(const BossManager&) = delete;

public:
	void initialize(const PlayerManager* player, PlayerAutomationPop* playerAutomationPop_);
	void begin();
	void update();
	void update_matrix();

	void attack_hit_callback();
	void damaged_hit_callback(float damage, const Vector3f& playerTranslate);

	void DrawAdaptive(_MAYBE_UNUSED const Camera3D* camera) override;

public:
	bool is_end_all() const;
	bool is_Invincible() const;
	bool is_transition() const;
	Collider* get_attack_collider() const;
	Collider* get_hit_collider() const;

	Boss* GetBoss() const { return boss.get(); }

private:
	void next_phase();
	void initialize_action();

	void DrawHP(int32_t wave);

#ifdef _DEBUG
public:
	void SetAttributeImGui() override;
#endif // _DEBUG

private:
	std::unique_ptr<Boss> boss;
	std::unique_ptr<BossActionManager> bossActionManager;
	int32_t phase;
	bool isPhaseTransition{ false };
	static constexpr int32_t MaxWave{ 3 };

	bool isEndAll{ false };

	//* hp bar *//

	Texture* hpFrameTexture_;
	Texture* hpFrameBackTexture_;
	std::array<Texture*, MaxWave> hpTexture_;

  
	PlayerAutomationPop* playerAutomationPop;
};
