#pragma once
#include "CollisionManager/Collider.h"
#include"Model/Model.h"
#include"CollisionManager/CollisionConfig.h"
using std::unique_ptr;



class TransCubeBullet:public Collider
{
public:
	TransCubeBullet();
	~TransCubeBullet();


	void Initialize(Vector3 &velocity,Vector3 pos);
	void Update();
	void Draw(ViewProjection view);

	Vector3 GetWorldPosition()override;
	void OnCollision()override;

	bool IsDead() const{ return isDead; }
	void SetIsDead(bool flag) {  isDead=flag; }
	Vector3 GetPosition() { return GetWorldPosition(); }
	
	

	
private:
	//Vector3 GetWorldPosition();


	Model* model_ = nullptr;
	WorldTransform worldTransform_ = {};

	Vector3 velocity_ = {};

	bool isDead = false;
	int32_t deathTimer_ = 10;


};



