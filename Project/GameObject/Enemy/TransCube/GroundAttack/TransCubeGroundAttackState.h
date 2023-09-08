#pragma once

#include"../GameObject/Enemy/TransCube/state/ITransCubeState.h"



class TransCubeGroundAttackState:public ITransCubeState
{
public:
	TransCubeGroundAttackState();
	~TransCubeGroundAttackState();

	void Initialize(TransCube* state)override;

	void Update(TransCube* state)override;

	void Draw(TransCube* state,ViewProjection view)override;

	void Deleate(TransCube *state)override;

private:

};

TransCubeGroundAttackState::TransCubeGroundAttackState()
{
}

TransCubeGroundAttackState::~TransCubeGroundAttackState()
{
}

inline void TransCubeGroundAttackState::Initialize(TransCube* state)
{
}

inline void TransCubeGroundAttackState::Update(TransCube* state)
{
}

inline void TransCubeGroundAttackState::Draw(TransCube* state, ViewProjection view)
{
}

inline void TransCubeGroundAttackState::Deleate(TransCube *state)
{
}




