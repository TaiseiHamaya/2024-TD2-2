#pragma once

#include <Engine/System/Performance.h>

#include <list>
#include <bitset>

#include "Player.h"

class PlayerManager : public BaseBehavior {
public:


public:
	void initialize();

	void begin();
	void update();
	void update_matrix();

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

#ifdef _DEBUG
public:
	void SetAttributeImGui() override;
#endif // _DEBUG

public:
	bool isAiming;
	bool isEject;
	std::bitset<2> gatherBitset{0};
	DeltaTimePoint aimingTimer;
	Vector2f inputStickL;
	Vector2f inputStickR;
	std::list<Player> players;
	std::list<Player>::iterator operatePlayer;
};
