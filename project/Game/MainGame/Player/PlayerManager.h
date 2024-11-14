#pragma once

#include <list>

#include "Player.h"

class PlayerManager {
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

public:
	Vector2f inputStick;
	std::list<Player> players;
	std::list<Player>::iterator operatePlayer;
};
