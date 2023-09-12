#include"RobotPunch.h"

RobotPunch::RobotPunch()
{
}

RobotPunch::~RobotPunch()
{
}

void RobotPunch::Initialize(Vector3 pos)
{
	model_ = std::make_unique<Model>();
	model_->CreateFromOBJ("Resources/EnemyObj/EnemyRobot/Punch", "EnemyRobotPunch.obj");
	worldTransform_.translation_ = pos;
	//worldTransform_.translation_.y = 1;
	worldTransform_.scale_ = { 3,3,6 };
	//衝突属性を設定
	SetCollisionAttribute(kCollisionAttributeEnemyBullet);
	//衝突対象を自分の属性以外に設定
	SetCollisionMask(kCollisionMaskEnemyBullet);
	AABB aabb = { {-4.0f,-4.0f,-4.0f}, {4.0f,4.0f,4.0f} };
	
	SetAABB(aabb);

}

void RobotPunch::Update()
{
	worldTransform_.translation_ = Add(worldTransform_.translation_, velosity);
	worldTransform_.UpdateMatrix();
}

void RobotPunch::Draw(ViewProjection view)
{
	model_->Draw(worldTransform_, view);
}

Vector3 RobotPunch::GetWorldPosition()
{
	Vector3 result;
	result.x = worldTransform_.matWorld_.m[3][0];
	result.y = worldTransform_.matWorld_.m[3][1];
	result.z = worldTransform_.matWorld_.m[3][2];
	return result;
}

void RobotPunch::OnCollision()
{
	isDead = true;
}
