#include"TranscubeGroundAttackState.h"




void TransCubeGroundAttackState::Initialize(TransCube* state)
{
	state->PushBackGroundBullet(state->GetPlayer()->GetWorldPosition());
	
}

void TransCubeGroundAttackState::Update(TransCube* state)
{


}

void TransCubeGroundAttackState::Draw(TransCube* state, ViewProjection view)
{


}

void TransCubeGroundAttackState::SetParent(const WorldTransform* parent)
{
}

void TransCubeGroundAttackState::Deleate(TransCube* state)
{
	for (TransCubeGroundAttack* bullet : state->GetGroundBullets())
	{
		bullet->SetDead(true);
    }
}
