#pragma once

#include <Engine/Game/Behavior/BaseBehavior.h>
#include <Engine/System/Performance.h>

#include <memory>
#include <string>
#include <vector>
	
class BaseBossBehavior;
struct BehaviorGraph;
class PlayerManager;

struct ActionFlow {
	DeltaTimePoint timer;
	float coolTime;
	std::vector<std::string> flowName;
};

class BossActionManager : public BaseBehavior {
public:
	BossActionManager() = default;
	virtual ~BossActionManager() = default;

public:
	void update();
	std::unique_ptr<BaseBossBehavior> next();
	void cancel_now_action();

	float max_hitpoint();

protected:
	virtual std::unique_ptr<BaseBossBehavior> create(const std::string& behaviorName) = 0;

private:
	ActionFlow* select_action_flow();

protected:
	std::vector<ActionFlow> actionFlows;
	std::unique_ptr<ActionFlow> defaultAction;
	float maxHitpoint{30};

private:
	ActionFlow* nowFlow{ nullptr };
	uint32_t nextFlowIndex{0};

public:
	static inline const PlayerManager* playerManager = nullptr;
};

