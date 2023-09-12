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
	BulletGround_->CreateFromOBJ("Resources/EnemyObj/TransCube/GroundAttack", "StarGroundAttack.obj");
	Bullet_->CreateFromOBJ("Resources/EnemyObj/TransCube/GroundAttack", "GroundAttack.obj");

	worldTransformBulletGround_.matWorld_ = MakeIdentity4x4();
	worldTransformBullet_.matWorld_ = MakeIdentity4x4();
	worldTransformBulletGround_.scale_ = { 5,0.3f,5 };
	worldTransformBullet_.scale_ = {1.0f ,2.0f,1.0f };
	BulletTimer = 0;
	DrawTimer = 0;
	
	worldTransformBulletGround_.parent_ = worldTransformBullet_.parent_;

	worldTransformBulletGround_.translation_ = position;
	worldTransformBullet_.translation_ = position;
	worldTransformBulletGround_.translation_.y = 0.1f;
	worldTransformBullet_.translation_.y = -10.0f;

	//衝突属性を設定
	SetCollisionAttribute(kCollisionAttributeEnemyBullet);
	//衝突対象を自分の属性以外に設定
	SetCollisionMask(kCollisionMaskEnemyBullet);
	AABB AABBBulletSizeSet = { { -1.0f,-4.0f,-1.0f } ,{ 1.0f,4.0f,1.0f } };

	SetAABB(AABBBulletSizeSet);
}

void TransCubeGroundAttack::Update()
{

	if (BulletTimer > 150){
		worldTransformBullet_.translation_.y += 0.2f;
	}
	if (worldTransformBullet_.translation_.y>=3.0f){
		BulletTimer = 0;
		DrawTimer++;
		if (DrawTimer>=120){
			isDead_ = true;
		}
	}
	else{
		BulletTimer++;
	}


	worldTransformBulletGround_.UpdateMatrix();
	worldTransformBullet_.UpdateMatrix();
}

void TransCubeGroundAttack::Draw(ViewProjection view){
	BulletGround_->Draw(worldTransformBulletGround_, view);
	Bullet_->Draw(worldTransformBullet_, view);
}

Vector3 TransCubeGroundAttack::GetWorldPosition()
{
	Vector3 result;
	 
	result.x = worldTransformBullet_.matWorld_.m[3][0];
	result.y = worldTransformBullet_.matWorld_.m[3][1];
	result.z = worldTransformBullet_.matWorld_.m[3][2];
	return result;

}

void TransCubeGroundAttack::OnCollision()
{

}
