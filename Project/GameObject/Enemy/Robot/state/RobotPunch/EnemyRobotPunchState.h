#pragma once

#include"../GameObject/Enemy/Robot/state/IEnemyRobotState.h"
#include"../GameObject/Enemy/Robot/EnemyRobot.h"


class EnemeyRobotPunchState:public IEnemyRobotState
{
public:

	void Initialize(EnemyRobot* state)override;

	void Update(EnemyRobot* state)override;


	void Draw(EnemyRobot* state, ViewProjection view)override;

	void SetParent(const WorldTransform* parent)override;
	void Deleate(EnemyRobot* state)override;

private:

};

