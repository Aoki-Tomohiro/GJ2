#pragma once
#include"Model/Model.h"
//#include"../GameObject/Enemy/TransCube/TransCube.h"
#include"../GameObject/Player/Player.h"

class TransCubeGroundAttack
{
public:
	TransCubeGroundAttack();
	~TransCubeGroundAttack();

	void Initialize(Vector3 position);

	void Update();

	void Draw(ViewProjection view);

	bool IsDead() const { return isDead_; }
	void SetDead(bool Flag) { isDead_ = Flag; }
private:

	Model* BulletGround_;
	Model* Bullet_;

	WorldTransform worldTransformBulletGround_;
	WorldTransform worldTransformBullet_;

	bool isDead_ = false;

};

