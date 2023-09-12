#include "EnemyRobotRandBulletState.h"

void EnemyRobotRandBulletState::Initialize(EnemyRobot* state)
{
	SpownPosition.x = state->GetEnemy().BodyWorldTransform.matWorld_.m[3][0];

	SpownPosition.y = state->GetEnemy().BodyWorldTransform.matWorld_.m[3][1];

	SpownPosition.z = state->GetEnemy().BodyWorldTransform.matWorld_.m[3][2];
	SpownPosition.z=70;
	SpownPosition.y = 0;

	StartRTWorldPos = state->GetEnemy().RarmWorldTransform.rotation_;
	EndRTWorldPos = { 0,0,0.5 };

	StartRBWorldPos = state->GetEnemy().RarmBoWorldTransform.rotation_;
	EndRBWorldPos = { 0.0f,-1.3f,1.6f };

	StartLTWorldPos = state->GetEnemy().LarmWorldTransform.rotation_;
	EndLTWorldPos = { 0,0,-0.5f };
	
	StartLBWorldPos = state->GetEnemy().LarmBoWorldTransform.rotation_;
	EndLBWorldPos = { 0.0f,1.0f,-0.0f };

	///StartWorldPos.BodyWorldTransform = state->GetEnemy().BodyWorldTransform;
	//StartWorldPos.HeadWorldTransform = state->GetEnemy().HeadWorldTransform;
	//1.3//0.6


	MoveFlag = true;
}

void EnemyRobotRandBulletState::Update(EnemyRobot* state)
{
	StateTimer++;
	BulletCoolTimer++;
	if (BulletCoolTimer>=120)
	{
		float theta = 3.3f;
		for (int i = 0; i < 30; i++)
		{
			Vector3 velocity = CircleAngleFanc(theta);

			velocity = Multiply(velocity, 0.5f);
			state->BulletPushBack(velocity, SpownPosition);
			theta += 0.1f;
		}
		BulletCoolTimer = 0;
	}

	if (MoveFlag)
	{
		Flame++;
		
	//R
		state->GetEnemy().RarmWorldTransform.rotation_ = state->EasingFanc( StartRTWorldPos, EndRTWorldPos, Flame, EndFlame);
		state->GetEnemy().RarmBoWorldTransform.rotation_ = state->EasingFanc(StartRBWorldPos, EndRBWorldPos, Flame, EndFlame);
	//L
		state->GetEnemy().LarmWorldTransform.rotation_ = state->EasingFanc(StartLTWorldPos, EndLTWorldPos, Flame, EndFlame);
		state->GetEnemy().LarmBoWorldTransform.rotation_ = state->EasingFanc(StartLBWorldPos, EndLBWorldPos, Flame, EndFlame);

		
		if (Flame == EndFlame) {
			Flame = 0;
			MoveFlag = false;
		}
	}
	
	if (StateTimer>=420)
	{
		state->SetStartRT(state->GetEnemy().RarmWorldTransform.rotation_);
		state->SetStartRB(state->GetEnemy().RarmBoWorldTransform.rotation_);
		state->SetStartLT(state->GetEnemy().LarmWorldTransform.rotation_);
		state->SetStartLB(state->GetEnemy().LarmBoWorldTransform.rotation_);
		state->SetMoveFlag(true);
		state->SetStateEndFlag(true);
	}
}

void EnemyRobotRandBulletState::Draw(EnemyRobot* state, ViewProjection view)
{
}

void EnemyRobotRandBulletState::SetParent(const WorldTransform* parent)
{
}

void EnemyRobotRandBulletState::Deleate(EnemyRobot* state)
{
}


Vector3 EnemyRobotRandBulletState::CircleAngleFanc(float theta)
{
	Vector3 velocity = {};
	velocity.x = std::cosf(theta);
	velocity.z = std::sinf(theta);

	return velocity;
}
