#include "BossActionManager.h"

#include "../BossBehavior/BaseBossBehavior.h"

#include <Lib/Adapter/Random/Random.h>

void BossActionManager::update() {
	for (auto& flow : actionFlows) {
		flow.timer.AddDeltaTime();
	}
}

std::unique_ptr<BaseBossBehavior> BossActionManager::next() {
	std::unique_ptr<BaseBossBehavior> result;
	// flowが終了している場合
	if (!nowFlow || nowFlow->flowName.size() <= nextFlowIndex) {
		nextFlowIndex = 0;
		nowFlow = select_action_flow();
		// クールダウン用タイマーのリセット
		nowFlow->timer.time = 0;
	}
	result = create(nowFlow->flowName[nextFlowIndex]);
	++nextFlowIndex;
	return std::move(result);
}

void BossActionManager::cancel_now_action() {
	nowFlow = nullptr;
}

uint32_t BossActionManager::max_hitpoint() {
	return maxHitpoint;
}

ActionFlow* BossActionManager::select_action_flow() {
	std::vector<uint32_t> actionIndex;
	for (uint32_t i = 0; auto & flow : actionFlows) {
		if (flow.timer.time >= flow.coolTime) {
			actionIndex.emplace_back(i);
		}
		++i;
	}

	if (actionIndex.empty()) {
		return defaultAction.get();
	}

	uint32_t index = Random::Generate<uint32_t>(0, static_cast<uint32_t>(actionIndex.size() - 1));

	return &actionFlows[actionIndex[index]];
}
