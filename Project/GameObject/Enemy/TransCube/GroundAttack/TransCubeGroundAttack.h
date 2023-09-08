#pragma once

#include"Model/Model.h"

class TransCubeGroundAttack
{
public:
	TransCubeGroundAttack();
	~TransCubeGroundAttack();

	void Initialise(Vector3 poosition);

	void Update();

	void Draw(ViewProjection view);

private:


	WorldTransform worldTransform_={};
	Vector3 position;

};






