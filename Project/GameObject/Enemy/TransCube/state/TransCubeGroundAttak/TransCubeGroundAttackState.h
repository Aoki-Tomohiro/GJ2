#pragma once

#include"../GameObject/Enemy/TransCube/state/ITransCubeState.h"
#include"../GameObject/Enemy/TransCube/TransCube.h"
#include"../GameObject/Enemy/TransCube/GroundAttack/TransCubeGroundAttack.h"


class TransCubeGroundAttackState:public ITransCubeState
{
public:
	
	void Initialize(TransCube* state)override;

	void Update(TransCube* state)override;

	void Draw(TransCube* state,ViewProjection view)override;

	
	void SetParent(const WorldTransform* parent)override;
	void Deleate(TransCube *state)override;

private:

	void Fire(TransCube* state);

	int32_t CoolTime = 0;
	int32_t BulletCount = 0;

};






