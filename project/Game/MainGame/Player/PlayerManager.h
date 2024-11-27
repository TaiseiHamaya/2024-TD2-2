#pragma once

#include <Engine/System/Performance.h>

//* engine
#include <Engine/Game/SxavengerPipeline/VisualProcessLayer.h>
#include <Engine/Game/Behavior/BaseBehavior.h>

#include <bitset>
#include <list>

#include "Player.h"

//* game
#include <Game/MainGame/Effect/Cursol.h>

class GameCamera;

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

	Player* GetOperator() const { return operatePlayer; }

	void spawn(std::list<Player>&& spawnPlayers);

	bool CanShot() const;

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

		void search_operate_player();

#ifdef _DEBUG
public:
	void SetAttributeImGui() override;
#endif // _DEBUG

public:
	std::list<Player>& get_players() { return players; };
	const std::list<Player>& get_players() const { return players; };
	const Player* get_operate_player() const { return operatePlayer; };

private:
	std::bitset<2> ejectBitset{ 0 };
	std::bitset<2> gatherBitset{ 0 };
	bool canEject;
	DeltaTimePoint aimingTimer;
	Vector2f inputStickL;
	Vector2f inputStickR;
	std::list<Player> players;
	Player* operatePlayer;

	float EjectPressTime;
	float EjectMaxDistance{ 10.0f };
	float EjectMinDistance{ 1.0f };

	std::unique_ptr<Cursol> cursol_;
};
