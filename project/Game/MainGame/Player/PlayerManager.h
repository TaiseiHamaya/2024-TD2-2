#pragma once

#include <Engine/System/Performance.h>

#include <bitset>
#include <list>

#include "Player.h"

class PlayerManager : public BaseBehavior {
public:
	PlayerManager() = default;
	~PlayerManager();

public:
	void initialize();

	void begin();
	void update();
	void update_matrix();

	void marge_collision();

private:
	void input();

	/// <summary>
	/// 集合命令
	/// </summary>
	void gather();
	void ungather();

	/// <summary>
	/// プレイヤー分離放出
	/// </summary>
	void eject();

	float create_scaling(float size);

	void search_operate_player();

#ifdef _DEBUG
public:
	void SetAttributeImGui() override;
#endif // _DEBUG

public:
	std::bitset<2> ejectBitset{ 0 };
	std::bitset<2> gatherBitset{ 0 };
	bool canEject;
	DeltaTimePoint aimingTimer;
	Vector2f inputStickL;
	Vector2f inputStickR;
	std::list<Player> players;
	Player* operatePlayer;

	float maxSize{ 5.0f };
	float minSize{ 0.5f };
	float ModelSize{ 2.0f };
	float DefaultSize{ 5.0f };
	float SizeParSec{ 1.f };

	float EjectMaxDistance{ 3.0f };
	float EjectLengthParSecond{ 3.0f };
};
