#pragma once

#include"../GameObject/Enemy/Robot/RandBullet/RandBullet.h"
#include"../GameObject/Enemy/Robot/state/IEnemyRobotState.h"
#include"../GameObject/Enemy/Robot/EnemyRobot.h"

class EnemyRobotRandBulletState : public IEnemyRobotState
{
public:


	void Initialize(EnemyRobot* state)override;

	void Update(EnemyRobot* state)override;

	void Draw(EnemyRobot* state, ViewProjection view)override;

	void SetParent(const WorldTransform* parent)override;
	void Deleate(EnemyRobot* state)override;

private:


	Vector3 CircleAngleFanc(float theta);

	Vector3 SpownPosition = {};

	int32_t BulletCoolTimer = 0;
	int32_t StateTimer = 0;

	bool MoveFlag;
	float Flame = 0;
	float EndFlame = 180;
	
	Vector3 StartRTWorldPos = {};
	Vector3 EndRTWorldPos = {};

	Vector3 StartRBWorldPos = {};
	Vector3 EndRBWorldPos = {};

	Vector3 StartLTWorldPos = {};
	Vector3 EndLTWorldPos = {};

	Vector3 StartLBWorldPos = {};
	Vector3 EndLBWorldPos = {};
};
