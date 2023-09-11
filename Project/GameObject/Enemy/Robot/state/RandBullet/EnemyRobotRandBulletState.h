#pragma once

#include"../GameObject/Enemy/Robot/RandBullet/RandBullet.h"
#include"../GameObject/Enemy/Robot/state/IEnemyRobotState.h"
#include"../GameObject/Enemy/Robot/EnemyRobot.h"

class EnemyRobotRandBulletState : public IEnemyRobotState
{
public:


	void Initialize(EnemyRobot* state)override;

	void Update(EnemyRobot* state)override;

	void Draw(EnemyRobot* state, ViewProjection view);

	void SetParent(const WorldTransform* parent);
	void Deleate(EnemyRobot* state);

private:

	float LerpMove(float pos);


	Vector3 CircleAngleFanc(float theta);

	Vector3 SpownPosition = {};

	int32_t BulletCoolTimer = 0;

	bool MoveFlag;
	float Flame = 0;
	float EndFlame = 180;
	
	Vector3 StartRTWorldPos = {};
	Vector3 EndRTWorldPos = {};

	Vector3 StartRBWorldPos = {};
	Vector3 EndRBWorldPos = {};
};
