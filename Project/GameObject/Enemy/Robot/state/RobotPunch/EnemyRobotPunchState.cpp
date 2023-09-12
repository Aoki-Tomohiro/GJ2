#include "EnemyRobotPunchState.h"

void EnemeyRobotPunchState::Initialize(EnemyRobot* state)
{
	StartRTWorldPos = state->GetEnemy().RarmWorldTransform.rotation_;
	EndRTWorldPos = { 0,0,0.5 };

	StartRBWorldPos = state->GetEnemy().RarmBoWorldTransform.rotation_;
	EndRBWorldPos = { 0,0,2.0 };

	StartLTWorldPos = state->GetEnemy().LarmWorldTransform.rotation_;
	EndLTWorldPos = { 0,1.0f,-0.0f };

	StartLBWorldPos = state->GetEnemy().LarmBoWorldTransform.rotation_;
	EndLBWorldPos = { 0.0f,0.0f,-0.0f };
	StateTimer = 0;
	MoveFlag = true;
}

void EnemeyRobotPunchState::Update(EnemyRobot* state)
{
	position = state->GetPlayer()->GetWorldPosition();
	position.y = position.y+2;
	position.z = 85;
	BulletCoolTime_ ++;
	StateTimer++;
	if (BulletCoolTime_>120)
	{
		state->PunchPushBack(position);
		BulletCoolTime_ = 0;
	}


	if (MoveFlag)
	{
		Flame++;

		//R
		state->GetEnemy().RarmWorldTransform.rotation_ = state->EasingFanc(StartRTWorldPos, EndRTWorldPos, Flame, EndFlame);
		state->GetEnemy().RarmBoWorldTransform.rotation_ = state->EasingFanc(StartRBWorldPos, EndRBWorldPos, Flame, EndFlame);
		//L
		state->GetEnemy().LarmWorldTransform.rotation_ = state->EasingFanc(StartLTWorldPos, EndLTWorldPos, Flame, EndFlame);
		state->GetEnemy().LarmBoWorldTransform.rotation_ = state->EasingFanc(StartLBWorldPos, EndLBWorldPos, Flame, EndFlame);


		if (Flame == EndFlame) {
			Flame = 0;
			MoveFlag = false;
		}
	}

	if (StateTimer >= 420)
	{
		state->SetStartRT(state->GetEnemy().RarmWorldTransform.rotation_);
		state->SetStartRB(state->GetEnemy().RarmBoWorldTransform.rotation_);
		state->SetStartLT(state->GetEnemy().LarmWorldTransform.rotation_);
		state->SetStartLB(state->GetEnemy().LarmBoWorldTransform.rotation_);
		state->SetMoveFlag(true);
		state->SetStateEndFlag(true);
	}
}

void EnemeyRobotPunchState::Draw(EnemyRobot* state, ViewProjection view)
{
}

void EnemeyRobotPunchState::SetParent(const WorldTransform* parent)
{
}

void EnemeyRobotPunchState::Deleate(EnemyRobot* state)
{
	for (RobotPunch* bullet : state->GetPunch())
	{
		bullet->SetIsDead(true);
     }
}
