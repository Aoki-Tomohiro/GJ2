#pragma once
#include"Model/Model.h"
#include"../GameObject/Enemy/TransCube/TransCube.h"
#include"../GameObject/Player/Player.h"

class TransCubeGroundAttack
{
public:
	TransCubeGroundAttack();
	~TransCubeGroundAttack();

	void Initialize(Vector3 position);

	void Update();

	void Draw(ViewProjection view);


private:

	std::unique_ptr<Model> BulletGround_;
	std::unique_ptr<Model> Bullet_;

	WorldTransform worldTransformBulletGround_;
	WorldTransform worldTransformBullet_;

	Vector3 position_ = {};

};

