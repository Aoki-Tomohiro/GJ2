#include"TranscubeGroundAttackState.h"

void TransCubeGroundAttackState::Initialize(TransCube* state)
{
	 CoolTime = 0;
	 BulletCount = 0;
}

void TransCubeGroundAttackState::Update(TransCube* state){
	CoolTime++;
	if (CoolTime > 60){
		Fire(state);
		BulletCount++;
		CoolTime = 0;
	}
	if (BulletCount>=15){
		state->StateFlag(true);
	}
	state->GetWorldTransform().rotation_.y-=0.01f;
}

void TransCubeGroundAttackState::Draw(TransCube* state, ViewProjection view){
}

void TransCubeGroundAttackState::SetParent(const WorldTransform* parent){
}

void TransCubeGroundAttackState::Deleate(TransCube* state){
	for (TransCubeGroundAttack* bullet : state->GetGroundBullets()){
		//bullet->SetDead(true);
		//delete bullet;
    }
}

void TransCubeGroundAttackState::Fire(TransCube *state)
{
	state->PushBackGroundBullet(state->GetPlayer()->GetWorldPosition());
}
