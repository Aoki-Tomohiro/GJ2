#pragma once
#include"Model/Model.h"
#include"ImGuiManager/ImGuiManager.h"
#include"../GameObject/Enemy/Robot/RandBullet/RandBullet.h"

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
	Player* GetPlayer() { return player_; }

private:

	void UpdateMatrixs();

	void HeadArmInit();

	SEnemyRobot enemy_ = {};


	Player* player_ = nullptr;

};




