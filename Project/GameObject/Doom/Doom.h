#pragma once

#include"Model/Model.h"

class Doom{
public:


	void Initialize();

	void Draw(ViewProjection view);


private:


	std::unique_ptr<Model> model_ = nullptr;
	WorldTransform worldTransform = {};


};