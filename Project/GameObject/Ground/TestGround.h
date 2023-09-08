#pragma once
#include"Model/Model.h"

class TestGround
{
public:


	void Initialize();

	void Draw(ViewProjection view);


private:


	std::unique_ptr<Model> model_ = nullptr;
	WorldTransform worldTransform = {};


};

