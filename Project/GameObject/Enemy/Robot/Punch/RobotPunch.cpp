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
	worldTransform_.scale_ = { 3,3,6 };

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
