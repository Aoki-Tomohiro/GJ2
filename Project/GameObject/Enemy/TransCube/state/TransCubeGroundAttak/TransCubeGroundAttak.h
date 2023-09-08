#pragma once
#include"../GameObject/Enemy/TransCube/state/ITransCubeState.h"
#include"../GameObject/Enemy/TransCube/TransCube.h"
#include"../GameObject/Player/Player.h"



class Player;

class TransCubeGroundAttak:public ITransCubeState
{
public:
	TransCubeGroundAttak();
	~TransCubeGroundAttak();

	void Initialize(TransCube* state)override;

	void Update(TransCube* state)override;

	void Draw(TransCube* state, ViewProjection view)override;

	void Deleate()override;

	
private:

	Player* player_ = nullptr;


};

