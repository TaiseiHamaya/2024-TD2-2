#include "PlayerAutomationPop.h"

#include "Player.h"

#include <format>

#include <Lib/Adapter/Json/Json.h>
#include <Lib/Adapter/Random/Random.h>

PlayerAutomationPop::PlayerAutomationPop() {
	SetName("PlayerAutomationPop");

	load_json();

	reset();

	SetToConsole();
}

void PlayerAutomationPop::reset() {
	if (popGroups.empty()) {
		return;
	}

	size_t v = Random::Generate(size_t(0), popGroups.size() - 1);
	auto itr = std::next(popGroups.begin(), v);
	key = itr->first;
	next = itr->second.begin();
}

std::list<Player> PlayerAutomationPop::pop(const DeltaTimePoint& transitionTimer) {
	if (key.empty()) {
		return {};
	}

	std::list<Player> result;
	while (next != popGroups[key].end() && next->delay >= transitionTimer.time) {
		auto& newValue = result.emplace_back();
		newValue.initialize(Vector3f{ next->xzPosition.x, 10.0f, next->xzPosition.y }, 0.4f);
		++next;
	}
	return result;
}

void PlayerAutomationPop::load_json() {
	popGroups.clear();
	Json root;
	JsonAdapter::TryLoadJson(file, root);

	for (auto& item : root.items()) {
		auto& group = popGroups.emplace(item.key(), std::vector<PopData>{}).first->second;
		for (auto& value : item.value()) {
			group.emplace_back(JsonAdapter::ToVector2f(value["XZPosition"]), value["Delay"]);
		}
		std::sort(group.begin(), group.end(),
			[](const PopData& l, const PopData& r) {return  l.delay < r.delay; }
		);
	}
}

void PlayerAutomationPop::export_json() {
	Json root;

	for (const auto& group : popGroups) {
		const std::string& name = group.first;
		const auto& values = group.second;


		for (int i = 0;  const auto & value : values) {
			auto& groupJson = root[name][std::format("{:02}", i)];
			groupJson["XZPosition"] = JsonAdapter::ToJson(value.xzPosition);
			groupJson["Delay"] = value.delay;
			++i;
		}
	}

	JsonAdapter::OverwriteJson(file, root);
}

void PlayerAutomationPop::SetAttributeImGui() {
	if (ImGui::Button("CreateGroup")) {
		popGroups.emplace(std::format("{:02}", popGroups.size()), std::vector<PopData>{});
	}

	for (auto& group : popGroups) {
		const std::string& name = group.first;
		std::vector<PopData>& data = group.second;
		if (ImGui::TreeNode(name.c_str())) {
			for (auto& pop : data) {
				ImGui::DragFloat2(std::format("XZ##{}", (void*)&pop).c_str(), &pop.xzPosition.x, 0.01f);
				ImGui::DragFloat(std::format("Delay##{}", (void*)&pop).c_str(), &pop.delay, 0.01f, 0.0f, 100.0f);
			}

			if (ImGui::Button("Add")) {
				data.emplace_back();
			}

			ImGui::TreePop();
		}
	}

	if (ImGui::Button("Export")) {
		export_json();
	}
}
