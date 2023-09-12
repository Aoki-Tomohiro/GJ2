#include "RandBullet.h"

RobotBullet::RobotBullet()
{
}

RobotBullet ::~RobotBullet()
{
	

}

void  RobotBullet::Initialize(Vector3& velocity, Vector3 pos)
{

	model_ = std::make_unique<Model>();
	//model_->CreateSphere();

	model_->CreateFromOBJ("Resources/EnemyObj/TransCube", "TransCube.obj");
	worldTransform_.matWorld_ = MakeIdentity4x4();
	worldTransform_.scale_ = { 1,1,1 };
	worldTransform_.translation_ = pos;
	worldTransform_.translation_.y = worldTransform_.translation_.y + 0.0f;
	velocity_ = velocity;
	//velocity_ = Multiply(velocity_, 0.0f);

	//衝突属性を設定
	SetCollisionAttribute(kCollisionAttributeEnemyBullet);
	//衝突対象を自分の属性以外に設定
	SetCollisionMask(kCollisionMaskEnemyBullet);
}

void  RobotBullet::Update()
{

	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);
	worldTransform_.UpdateMatrix();
}

void  RobotBullet::Draw(ViewProjection view)
{
	model_->Draw(worldTransform_, view);

}

Vector3  RobotBullet::GetWorldPosition()
{
	Vector3 pos = {};

	pos.x = worldTransform_.matWorld_.m[3][0];
	pos.y = worldTransform_.matWorld_.m[3][1];
	pos.z = worldTransform_.matWorld_.m[3][2];
	return pos;
}

void  RobotBullet::OnCollision()
{
	isDead = true;
}