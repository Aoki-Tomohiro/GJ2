#include "TestGround.h"

void TestGround::Initialize()
{

	model_ = std::make_unique<Model>();

	model_.get()->CreateFromOBJ("Resources/Ground", "Ground.obj");
	worldTransform.matWorld_ = MakeIdentity4x4();
	
	worldTransform.scale_ = { 2,2,2 };
	worldTransform.UpdateMatrix();

}

void TestGround::Draw(ViewProjection view)
{
	model_.get()->Draw(worldTransform, view);


}
