#include "EnemyRobot.h"

EnemyRobot::EnemyRobot()
{
}

EnemyRobot::~EnemyRobot()
{
}

void EnemyRobot::Initialize()
{
	enemy_.BodyModel = std::make_unique<Model>();
	enemy_.BodyModel->CreateFromOBJ("Project/Resources/EnemyObj/EnemyRobot/Body", "EnemyRobotBody.obj");
	enemy_.BodyWorldTransform.matWorld_ = MakeIdentity4x4();
	enemy_.BodyWorldTransform.scale_ = { 28,28,28 };
	enemy_.BodyWorldTransform.translation_ = { 0,20,90 };

	ArmInit();

}

void EnemyRobot::Update()
{
	ImGui::Begin("Larm");
	ImGui::SliderFloat3("Torotate", &enemy_.LarmWorldTransform.rotation_.x, -5, 5);
	ImGui::SliderFloat3("Borotate", &enemy_.LarmBoWorldTransform.rotation_.x, -5, 5);
	ImGui::End();
	ImGui::Begin("Rarm");
	ImGui::SliderFloat3("Tot", &enemy_.RarmWorldTransform.rotation_.x, -5, 5);
	ImGui::SliderFloat3("Bot", &enemy_.RarmBoWorldTransform.rotation_.x, -5, 5);
	ImGui::End();

	ImGui::Begin("Body");
	ImGui::DragFloat3("trans", &enemy_.BodyWorldTransform.translation_.x, -5, 5);
	ImGui::End();


	UpdateMatrixs();

}

void EnemyRobot::Draw(ViewProjection view)
{
	enemy_.HeadModel->Draw(enemy_.HeadWorldTransform, view);
	enemy_.BodyModel->Draw(enemy_.BodyWorldTransform, view);
	enemy_.LarmTopModel->Draw(enemy_.LarmWorldTransform, view);
	enemy_.LarmBoModel->Draw(enemy_.LarmBoWorldTransform, view);
	enemy_.RarmTopModel->Draw(enemy_.RarmWorldTransform, view);
	enemy_.RarmBoModel->Draw(enemy_.RarmBoWorldTransform, view);
}

void EnemyRobot::UpdateMatrixs()
{
	enemy_.BodyWorldTransform.UpdateMatrix();
	enemy_.CoreWorldTransform.UpdateMatrix();
	enemy_.HeadWorldTransform.UpdateMatrix();
	enemy_.LarmWorldTransform.UpdateMatrix();
	enemy_.LarmBoWorldTransform.UpdateMatrix();
	enemy_.RarmWorldTransform.UpdateMatrix();
	enemy_.RarmBoWorldTransform.UpdateMatrix();

}

void EnemyRobot::ArmInit()
{
	enemy_.HeadModel = std::make_unique<Model>();
	enemy_.HeadModel->CreateFromOBJ("Project/Resources/EnemyObj/EnemyRobot/Head", "EnemyRobotHead.obj");
	enemy_.HeadWorldTransform.parent_ = &enemy_.BodyWorldTransform;

	enemy_.LarmTopModel = std::make_unique<Model>();
	enemy_.LarmTopModel->CreateFromOBJ("Project/Resources/EnemyObj/EnemyRobot/LTopArm", "EnemyRobotLArm.obj");
	enemy_.LarmWorldTransform.parent_= &enemy_.BodyWorldTransform;
	enemy_.LarmWorldTransform.translation_.x = enemy_.LarmWorldTransform.translation_.x + 0.5f;
	enemy_.LarmWorldTransform.translation_.y = enemy_.LarmWorldTransform.translation_.y + 0.2f;
	enemy_.LarmBoModel = std::make_unique <Model>();
	enemy_.LarmBoModel->CreateFromOBJ("Project/Resources/EnemyObj/EnemyRobot/LBoArm", "LBoArm.obj");
	enemy_.LarmBoWorldTransform.parent_ = &enemy_.LarmWorldTransform;
	enemy_.LarmBoWorldTransform.translation_.x = enemy_.LarmBoWorldTransform.translation_.x + 1.0f;
    enemy_.LarmBoWorldTransform.translation_.y = enemy_.LarmBoWorldTransform.translation_.y + 0.1f;

	enemy_.RarmTopModel = std::make_unique<Model>();
	enemy_.RarmTopModel->CreateFromOBJ("Project/Resources/EnemyObj/EnemyRobot/RTopArm", "RTopArm.obj");
	enemy_.RarmWorldTransform.parent_ = &enemy_.BodyWorldTransform;
	enemy_.RarmWorldTransform.translation_.x = enemy_.RarmWorldTransform.translation_.x - 0.5f;
	enemy_.RarmWorldTransform.translation_.y = enemy_.RarmWorldTransform.translation_.y + 0.2f;
	enemy_.RarmBoModel = std::make_unique <Model>();
	enemy_.RarmBoModel->CreateFromOBJ("Project/Resources/EnemyObj/EnemyRobot/RBoArm", "EnemyRBoArm.obj");
	enemy_.RarmBoWorldTransform.parent_ = &enemy_.RarmWorldTransform;
	enemy_.RarmBoWorldTransform.translation_.x = enemy_.LarmBoWorldTransform.translation_.x -1.9f;
	enemy_.RarmBoWorldTransform.translation_.z = enemy_.RarmBoWorldTransform.translation_.z - 0.3f;
	enemy_.RarmBoWorldTransform.translation_.y = enemy_.LarmBoWorldTransform.translation_.y+0.002f;


}
