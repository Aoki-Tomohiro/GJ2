#pragma once

#include"Model/Model.h"
#include"../GameObject/Enemy/TransCube/state/ITransCubeState.h"
#include"../GameObject/Enemy/TransCube/RandBullet/TransCubeBullet.h"
#include"../GameObject/Enemy/TransCube/TransCube.h"
#include"Input/Input.h"


class TransCubeRandBulletState :public ITransCubeState
{
public:
	TransCubeRandBulletState();
	~TransCubeRandBulletState();



	void Initialize(TransCube* state) override;

	void Update(TransCube* state) override;

	void Draw(TransCube* state, ViewProjection view)override;

	void SetParent(const WorldTransform* parent)override;

	void Deleate()override;

private:

	enum BulletMode
	{
		boost,
		Shot,
		death
	};
	void Fire(TransCube *state);

	Vector3 VelocityFanc(Vector3 v1, Vector3 v2);
	void BulletPushBack(Vector3 v,Vector3 pos);

	std::list<TransCubeBullet*> bullets_;

	int32_t CoolTime = 0;
	Input* input = nullptr;

	int32_t ModeTimer = 0;

	float rotateSpeed = 0.0f;

	BulletMode mode_ = {};
};



