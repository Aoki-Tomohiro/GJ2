#include "TransCubeBullet.h"

TransCubeBullet::TransCubeBullet()
{
}

TransCubeBullet::~TransCubeBullet()
{
	delete model_;
}

void TransCubeBullet::Initialize(Vector3 &velocity,Vector3 pos)
{

	model_ = new Model();
	model_->CreateSphere();

	//model_->CreateFromOBJ("Project/Resources/EnemyObj/TransCube", "TransCube.obj");
	worldTransform_.matWorld_ = MakeIdentity4x4();
	worldTransform_.scale_ = { 1,1,1 };
	worldTransform_.translation_ = pos;
	worldTransform_.translation_.y = worldTransform_.translation_.y+3.0f;
	velocity_ = velocity;
}

void TransCubeBullet::Update()
{

	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);
	worldTransform_.UpdateMatrix();
}

void TransCubeBullet::Draw(ViewProjection view)
{
	model_->Draw(worldTransform_, view);

}

Vector3 TransCubeBullet::GetWorldPosition()
{
	Vector3 pos = {};

	pos.x = worldTransform_.matWorld_.m[3][0];
	pos.y = worldTransform_.matWorld_.m[3][1];
	pos.z = worldTransform_.matWorld_.m[3][2];
	return pos;
}
