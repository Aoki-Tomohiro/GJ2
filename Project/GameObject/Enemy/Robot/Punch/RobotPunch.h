#pragma once
#include"Model/Model.h"

class RobotPunch
{
public:
	RobotPunch();
	~RobotPunch();

	void Initialize(Vector3 pos);

	void Update();

	void Draw(ViewProjection view);

	bool IsDead() { return isDead; }
	void SetIsDead(bool flag) { isDead = flag; }
	WorldTransform GetWorldTransform() { return worldTransform_; }
private:

	WorldTransform worldTransform_ = {};
	std::unique_ptr<Model> model_ = nullptr;
	Vector3 velosity = { 0,0,-1.3f };

	bool isDead = false;
};

