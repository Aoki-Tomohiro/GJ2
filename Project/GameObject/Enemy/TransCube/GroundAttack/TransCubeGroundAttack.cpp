#include "TransCubeGroundAttack.h"

TransCubeGroundAttack::TransCubeGroundAttack()
{
}

TransCubeGroundAttack::~TransCubeGroundAttack()
{
	
}

void TransCubeGroundAttack::Initialize(Vector3 position)
{
	BulletGround_ = std::make_unique<Model>();
	Bullet_ = std::make_unique<Model>();

	worldTransformBulletGround_.matWorld_ = MakeIdentity4x4();
	worldTransformBullet_.matWorld_ = MakeIdentity4x4();
	worldTransformBulletGround_.parent_ = worldTransformBullet_.parent_;


}

void TransCubeGroundAttack::Update()
{
}

void TransCubeGroundAttack::Draw(ViewProjection view)
{
}
