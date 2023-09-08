#include"TransCube.h"
#include "CollisionManager/CollisionConfig.h"

TransCube::TransCube()
{
}

TransCube::~TransCube()
{
	state_.get()->Deleate(this);
	state_.release();
	delete model_;
}

void TransCube::Initialize()
{
	model_ = new Model();
	worldTransform.matWorld_ = MakeIdentity4x4();
	worldTransform.scale_ = { 5,5,5 };
	worldTransform.translation_ = { 0,0,0 };
	model_->CreateFromOBJ("Project/Resources/EnemyObj/TransCube", "TransCube.obj");
	
	state_ =std::make_unique<TransCubeGroundAttackState>();
	state_->Initialize(this);
	
	input = Input::GetInstance();

	//衝突属性を設定
	SetCollisionAttribute(kCollisionAttributeEnemy);
	//衝突対象を自分の属性以外に設定
	SetCollisionMask(kCollisionMaskEnemy);

}

void TransCube::Update()
{
	if (input->IsPushKey(DIK_F))
	{
		worldTransform.translation_.x -= 1.00f;
	}
	if (input->IsPushKey(DIK_T))
	{
		worldTransform.translation_.z += 1.05f;
	}
	if (input->IsPushKey(DIK_G))
	{
		worldTransform.translation_.x += 1.05f;
	}
	if (input->IsPushKey(DIK_V))
	{
		worldTransform.translation_.z -= 1.05f;
	}
	ReticlePosFanc();

	worldTransform.UpdateMatrix();
	state_->Update(this);

	bullets_.remove_if([](TransCubeBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});
	for (TransCubeBullet* bullet : bullets_)
	{
		bullet->Update();
	}
	for (TransCubeGroundAttack* bullet : Ground_)
	{
		bullet->Update();
	}

}

void TransCube::Draw(ViewProjection view)
{
	for (TransCubeBullet* bullet : bullets_)
	{
		bullet->Draw(view);
	}
	for (TransCubeGroundAttack* bullet : Ground_)
	{
		bullet->Draw(view);
	}


	state_->Draw(this, view);
	model_->Draw(worldTransform, view);

}

void TransCube::ChangeRandBulletState()
{

	state_.release();
	state_ = std::make_unique<TransCubeRandBulletState>();
	state_.get()->Initialize(this);
}

Vector3 TransCube::GetWorldPosition()
{
	Vector3 result = {};

	result.x = worldTransform.matWorld_.m[3][0];
	result.y = worldTransform.matWorld_.m[3][1];
	result.z = worldTransform.matWorld_.m[3][2];
	return result;
}

void TransCube::OnCollision() {

}



void TransCube::PushBackBullet(Vector3 velocity,Vector3 pos)
{
	TransCubeBullet* bullet = new TransCubeBullet();
	bullet->Initialize(velocity, pos);
	bullets_.push_back(bullet);
}

void TransCube::PushBackGroundBullet(Vector3 pos)
{
	TransCubeGroundAttack* bullet = new TransCubeGroundAttack();
	bullet->Initialize(pos);
	Ground_.push_back(bullet);

}

void TransCube::ReticlePosFanc()
{

	const float kDistancePlayerTo3DReticle = 500.0f;
	Vector3 Foffset = { 0, 0, 1.0f };

	Vector3 Fpos = {};
	Fpos = GetWorldPosition();
	
	Foffset = TransformNormal(Foffset, worldTransform.matWorld_);
	Foffset = Normalize(Foffset);

	Foffset.x *= kDistancePlayerTo3DReticle;
	Foffset.y *= kDistancePlayerTo3DReticle;
	Foffset.z *= kDistancePlayerTo3DReticle;
	
	DirectionReticlePos_.FworldTransform.translation_.x = Foffset.x + Fpos.x;
	DirectionReticlePos_.FworldTransform.translation_.y = Foffset.y + Fpos.y;
	DirectionReticlePos_.FworldTransform.translation_.z = Foffset.z + Fpos.z;
	DirectionReticlePos_.FworldTransform.UpdateMatrix();


#pragma region B
	Vector3 Boffset = { 0, 0, -1.0f };
	Vector3 Bpos = {};
	Bpos = GetWorldPosition();
	Boffset = TransformNormal(Boffset, worldTransform.matWorld_);
	Boffset = Normalize(Boffset);

	Boffset.x *= kDistancePlayerTo3DReticle;
	Boffset.y *= kDistancePlayerTo3DReticle;
	Boffset.z *= kDistancePlayerTo3DReticle;

	DirectionReticlePos_.BworldTransform.translation_.x = Boffset.x + Bpos.x;
	DirectionReticlePos_.BworldTransform.translation_.y = Boffset.y + Bpos.y;
	DirectionReticlePos_.BworldTransform.translation_.z = Boffset.z + Bpos.z;
	DirectionReticlePos_.BworldTransform.UpdateMatrix();
#pragma endregion

	//
	Vector3 Roffset = { 1.0f, 0.0f, 0.0f };
	Vector3 Rpos = {};
    Rpos = GetWorldPosition();
	Roffset = TransformNormal(Roffset,worldTransform.matWorld_);
	Roffset = Normalize(Roffset);
	
	Roffset.x *= kDistancePlayerTo3DReticle;
	Roffset.y *= kDistancePlayerTo3DReticle;
	Roffset.z *= kDistancePlayerTo3DReticle;

	DirectionReticlePos_.RworldTransform.translation_.x = Roffset.x + Rpos.x;
	DirectionReticlePos_.RworldTransform.translation_.y = Roffset.y + Rpos.y;
	DirectionReticlePos_.RworldTransform.translation_.z = Roffset.z + Rpos.z;
	DirectionReticlePos_.RworldTransform.UpdateMatrix();

	//
	Vector3 Loffset = { -1.0f, 0.0f, 0.0f };

	Vector3 Lpos = {};
	Lpos = GetWorldPosition();
	Loffset = TransformNormal(Loffset, worldTransform.matWorld_);
	Loffset = Normalize(Loffset);

	Loffset.x *= kDistancePlayerTo3DReticle;
	Loffset.y *= kDistancePlayerTo3DReticle;
	Loffset.z *= kDistancePlayerTo3DReticle;

	DirectionReticlePos_.LworldTransform.translation_.x = Loffset.x + Lpos.x;
	DirectionReticlePos_.LworldTransform.translation_.y = Loffset.y + Lpos.y;
	DirectionReticlePos_.LworldTransform.translation_.z = Loffset.z + Lpos.z;

	DirectionReticlePos_.LworldTransform.UpdateMatrix();

}