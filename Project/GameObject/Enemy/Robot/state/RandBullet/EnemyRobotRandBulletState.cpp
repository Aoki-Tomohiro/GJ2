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

	///StartWorldPos.BodyWorldTransform = state->GetEnemy().BodyWorldTransform;
	//StartWorldPos.HeadWorldTransform = state->GetEnemy().HeadWorldTransform;
	//1.3//0.6


	MoveFlag = true;
}

void EnemyRobotRandBulletState::Update(EnemyRobot* state)
{
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
		//Vector3 pos;
		
		state->GetEnemy().RarmWorldTransform.rotation_.x = StartRTWorldPos.x + (EndRTWorldPos.x - StartRTWorldPos.x) * LerpMove(Flame / EndFlame);
		state->GetEnemy().RarmWorldTransform.rotation_.z = StartRTWorldPos.z + (EndRTWorldPos.z - StartRTWorldPos.z) * LerpMove(Flame / EndFlame);
		state->GetEnemy().RarmWorldTransform.rotation_.y = StartRTWorldPos.y + (EndRTWorldPos.y - StartRTWorldPos.y) * LerpMove(Flame / EndFlame);

		state->GetEnemy().RarmBoWorldTransform.rotation_.x = StartRBWorldPos.x + (EndRBWorldPos.x - StartRBWorldPos.x) * LerpMove(Flame / EndFlame);
		state->GetEnemy().RarmBoWorldTransform.rotation_.z = StartRBWorldPos.z + (EndRBWorldPos.z - StartRBWorldPos.z) * LerpMove(Flame / EndFlame);
		state->GetEnemy().RarmBoWorldTransform.rotation_.y = StartRBWorldPos.y + (EndRBWorldPos.y - StartRBWorldPos.y) * LerpMove(Flame / EndFlame);



		
		if (Flame == EndFlame) {
			Flame = 0;
			MoveFlag = false;
		}
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
float EnemyRobotRandBulletState::LerpMove(float pos)
{

	float result;

	result = pos < 0.5 ? 4 * pos * pos * pos : 1 - std::powf(-2.0f * pos + 2, 3) / 2.0f;

	return result;

}