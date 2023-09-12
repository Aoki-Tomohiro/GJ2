#pragma once


#include"Model/Model.h"

class EnemyRobot;

class IEnemyRobotState
{
public:

	virtual void Initialize(EnemyRobot* state) = 0;

	virtual void Update(EnemyRobot* state) = 0;

	virtual void Draw(EnemyRobot* state, ViewProjection view) = 0;

	virtual void SetParent(const WorldTransform* parent) = 0;
	virtual void Deleate(EnemyRobot* state) = 0;
private:

};