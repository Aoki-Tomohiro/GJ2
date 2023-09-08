#include "TransCubeGroundAttack.h"

TransCubeGroundAttack::TransCubeGroundAttack()
{
}

TransCubeGroundAttack::~TransCubeGroundAttack()
{
	delete BulletGround_;
	delete Bullet_;
	
}

void TransCubeGroundAttack::Initialize(Vector3 position)
{
	BulletGround_ =new Model();
	Bullet_ = new Model();

	BulletGround_->CreateSphere();
	Bullet_->CreateSphere();

	worldTransformBulletGround_.matWorld_ = MakeIdentity4x4();
	worldTransformBullet_.matWorld_ = MakeIdentity4x4();
	worldTransformBulletGround_.scale_ = { 5,0.3f,5 };
	worldTransformBullet_.scale_ = {2.0f ,8.0f,2.0f };

	
	worldTransformBulletGround_.parent_ = worldTransformBullet_.parent_;

	worldTransformBulletGround_.translation_ = position;
	worldTransformBullet_.translation_ = position;
	
}

void TransCubeGroundAttack::Update()
{

	worldTransformBulletGround_.UpdateMatrix();
	worldTransformBullet_.UpdateMatrix();
}

void TransCubeGroundAttack::Draw(ViewProjection view)
{
	BulletGround_->Draw(worldTransformBulletGround_, view);
	Bullet_->Draw(worldTransformBullet_, view);

}
