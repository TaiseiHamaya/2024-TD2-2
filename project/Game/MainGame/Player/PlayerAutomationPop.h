#pragma once

#include <list>
#include <unordered_map>
#include <vector>
#include <string>

#include <Engine/Game/Behavior/BaseBehavior.h>
#include <Engine/System/Performance.h>

class Player;

struct PopData {
	Vector2f xzPosition;
	float delay;
};

class PlayerAutomationPop : public BaseBehavior {
public:
	PlayerAutomationPop();
	~PlayerAutomationPop() = default;

public:
	void reset();
	std::list<Player> pop(const DeltaTimePoint& transitionTimer);

private:
	void load_json();
	void export_json();

public:
	void SetAttributeImGui() override;

private:
#ifdef _DEBUG
	std::map<std::string, std::vector<PopData>> popGroups;
#else
	std::unordered_map<std::string, std::vector<PopData>> popGroups;
#endif // _DEBUG

	std::string key;
	std::vector<PopData>::iterator next;

#ifdef _DEBUG
private:
	std::string file{"PlayerAutomationPop.json"};
#endif // _DEBUG
};
