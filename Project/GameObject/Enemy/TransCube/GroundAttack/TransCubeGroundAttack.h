#pragma once
#include"Model/Model.h"
//#include"../GameObject/Enemy/TransCube/TransCube.h"
#include"../GameObject/Player/Player.h"
#include"CollisionManager/Collider.h"
#include"CollisionManager/CollisionConfig.h"

class TransCubeGroundAttack :public Collider
{
public:
	TransCubeGroundAttack();
	~TransCubeGroundAttack();

	void Initialize(Vector3 position);

	void Update();

	void Draw(ViewProjection view);

	bool IsDead() const { return isDead_; }
	void SetDead(bool Flag) { isDead_ = Flag; }

	Vector3 GetWorldPosition()override;
	void OnCollision()override;

private:

	Model* BulletGround_;
	Model* Bullet_;

	WorldTransform worldTransformBulletGround_;
	WorldTransform worldTransformBullet_;

	bool isDead_ = false;

	int32_t BulletTimer = 0;
	int32_t DrawTimer = 0;
};

