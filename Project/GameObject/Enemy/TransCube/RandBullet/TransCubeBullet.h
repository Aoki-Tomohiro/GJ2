#pragma once


#include"Model/Model.h"

using std::unique_ptr;



class TransCubeBullet
{
public:
	TransCubeBullet();
	~TransCubeBullet();


	void Initialize(Vector3 &velocity,Vector3 pos);

	void Update();

	bool IsDead() const{ return isDead; }
	void SetIsDead(bool flag) {  isDead=flag; }
	Vector3 GetPosition() { return GetWorldPosition(); }

	void Draw(ViewProjection view);

	
private:
	Vector3 GetWorldPosition();


	Model* model_ = nullptr;
	WorldTransform worldTransform_ = {};

	Vector3 velocity_ = {};

	bool isDead = false;
	int32_t deathTimer_ = 10;


};



