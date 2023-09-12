#pragma once
#include"Model/Model.h"
#include"Sprite/Sprite.h"
#include"ImGuiManager/ImGuiManager.h"
#include"../GameObject/Enemy/Robot/RandBullet/RandBullet.h"

#include"../GameObject/Enemy/Robot/state/RandBullet/EnemyRobotRandBulletState.h"
#include"../GameObject/Enemy/Robot/state/RobotPunch/EnemyRobotPunchState.h"
#include"../GameObject/Enemy/Robot/Punch/RobotPunch.h"
#include"CollisionManager/CollisionConfig.h"
#include"CollisionManager/Collider.h"

struct  SEnemyRobot
{
	WorldTransform BodyWorldTransform;
	WorldTransform HeadWorldTransform;
	WorldTransform RarmWorldTransform;
	WorldTransform RarmBoWorldTransform;
	WorldTransform LarmWorldTransform;
	WorldTransform LarmBoWorldTransform;

	std::unique_ptr<Model> BodyModel = nullptr;
	std::unique_ptr<Model> HeadModel = nullptr;
	std::unique_ptr<Model> RarmTopModel = nullptr;
	std::unique_ptr<Model> RarmBoModel = nullptr;
	std::unique_ptr<Model> LarmTopModel = nullptr;
	std::unique_ptr<Model> LarmBoModel = nullptr;
};
class Player;
class GameSceneRobot;
class EnemyRobot: public Collider
{
public:
	EnemyRobot();
	~EnemyRobot();

	void Initialize();

	void Update();

	void Draw(ViewProjection view);

	void DrawUI();


	Vector3 GetWorldPosition()override;
	void OnCollision()override;



	void SetPlayer(Player* player) { player_ = player; }
	void SetRTEnemyWorldPos(Vector3 enemy) { enemy_.RarmWorldTransform.rotation_ = enemy; }

	void SetMoveFlag(bool Flag) {MoveFlag = Flag; }
	void SetStateEndFlag(bool Flag) { isStateEndFlag = Flag; }

	void SetStartRT(Vector3 r) { StartRTWorPos = r; }
	void SetStartRB(Vector3 r) { StartRBWorPos = r; }

	void SetStartLT(Vector3 r) { StartLTWorPos = r; }
	void SetStartLB(Vector3 r) { StartLBWorPos = r; }


	Player* GetPlayer() { return player_; }
	SEnemyRobot &GetEnemy() { return enemy_; }
	void BulletPushBack(Vector3 velocity,Vector3 pos);
	void PunchPushBack(Vector3 pos);
	std::list<RobotPunch*>GetPunch() { return punch_; }
	std::list<RobotBullet*>GetBullets() { return Bullets_; }

	Vector3 EasingFanc(Vector3 startv, Vector3 Endv, float &Flame, float EndFlame);

	void SetGameSceneRobot(GameSceneRobot* gameSceneRobot) { gameSceneRobot_ = gameSceneRobot; };

	int32_t GetLife() {
		return life_;
	}



private:
	float LerpMove(float pos);

	void UpdateMatrixs();

	void HeadArmInit();

	void BulletUp();
	SEnemyRobot enemy_ = {};

	std::unique_ptr<Model> EnemyCore = nullptr;

	std::list<RobotBullet*> Bullets_ = {};

	Player* player_ = nullptr;

	std::unique_ptr<IEnemyRobotState> state = {};

	bool MoveFlag = false;
	bool StateFlag = false;
	bool isStateEndFlag = false;

	Vector3 StartRTWorPos = {};
	Vector3 EndRTWorPos = {};

	Vector3 StartRBWorPos = {};
	Vector3 EndRBWorPos = {};

	Vector3 StartLTWorPos = {};
	Vector3 EndLTWorPos = {};

	Vector3 StartLBWorPos = {};
	Vector3 EndLBWorPos = {};

	float Flame = 0.0f;
	float EndFlame=180.0f;

	Player* player = nullptr;


	std::list<RobotPunch*>punch_ = {};


	std::unique_ptr<Model> CoreModel_ = nullptr;
	WorldTransform CoreWorldTransform = {};

	//体力
	int32_t life_ = 300;

	//体力のスプライト
	uint32_t textureHandleHP1_ = 0;
	uint32_t textureHandleHP2_ = 0;
	std::unique_ptr<Sprite> spriteHP1_ = nullptr;
	std::unique_ptr<Sprite> spriteHP2_ = nullptr;
	Vector2 spriteScale_{ 1.0f,1.0f };

	GameSceneRobot* gameSceneRobot_ = nullptr;

	std::unique_ptr<Sprite> CoreAttackTelop = nullptr;
	uint32_t TelopTex = {};
	bool TelopFlag = false;
	float TelopDrawCount = 1.0f;
};




