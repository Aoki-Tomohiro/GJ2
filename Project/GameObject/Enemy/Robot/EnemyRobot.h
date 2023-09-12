#pragma once
#include"Model/Model.h"
#include"ImGuiManager/ImGuiManager.h"
#include"../GameObject/Enemy/Robot/RandBullet/RandBullet.h"

#include"../GameObject/Enemy/Robot/state/RandBullet/EnemyRobotRandBulletState.h"

struct  SEnemyRobot
{

	WorldTransform CoreWorldTransform;
	WorldTransform BodyWorldTransform;
	WorldTransform HeadWorldTransform;
	WorldTransform RarmWorldTransform;
	WorldTransform RarmBoWorldTransform;
	WorldTransform LarmWorldTransform;
	WorldTransform LarmBoWorldTransform;

	std::unique_ptr<Model> CoreModel = nullptr;
	std::unique_ptr<Model> BodyModel = nullptr;
	std::unique_ptr<Model> HeadModel = nullptr;
	std::unique_ptr<Model> RarmTopModel = nullptr;
	std::unique_ptr<Model> RarmBoModel = nullptr;
	std::unique_ptr<Model> LarmTopModel = nullptr;
	std::unique_ptr<Model> LarmBoModel = nullptr;
};
class Player;
class EnemyRobot
{
public:
	EnemyRobot();
	~EnemyRobot();

	void Initialize();

	void Update();

	void Draw(ViewProjection view);

	void SetPlayer(Player* player) { player_ = player; }
	void SetRTEnemyWorldPos(Vector3 enemy) { enemy_.RarmWorldTransform.rotation_ = enemy; }

	Player* GetPlayer() { return player_; }
	SEnemyRobot &GetEnemy() { return enemy_; }
	void BulletPushBack(Vector3 velocity,Vector3 pos);

	Vector3 EasingFanc(EnemyRobot* state, Vector3 startv, Vector3 Endv, float &Flame, float EndFlame);
private:
	float LerpMove(float pos);

	void UpdateMatrixs();

	void HeadArmInit();

	void BulletUp();
	SEnemyRobot enemy_ = {};
	std::list<RobotBullet*> Bullets_ = {};

	Player* player_ = nullptr;

	std::unique_ptr<IEnemyRobotState> state = {};

};




