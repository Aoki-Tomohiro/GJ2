#pragma once

#include"../GameObject/Enemy/TransCube/state/ITransCubeState.h"



class TransCubeGroundAttackState:public ITransCubeState
{
public:
	
	void Initialize(TransCube* state)override;

	void Update(TransCube* state)override;

	void Draw(TransCube* state,ViewProjection view)override;

	
	void SetParent(const WorldTransform* parent)override;

	void Deleate(TransCube *state)override;

private:



};






