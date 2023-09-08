#pragma once
#include"Model/Model.h"
#include"state/TransCubeRandBullet/TransCubeRandBulletState.h"
#include"Input/Input.h"
#include"../GameObject/Player/Player.h"
#include "CollisionManager/Collider.h"
struct TransCubeReticle
{
	Vector3 Left = {};
	Vector3 Right = {};
	Vector3 Front = {};
	Vector3 Back = {};
	WorldTransform LworldTransform = {};
	WorldTransform RworldTransform = {};
	WorldTransform FworldTransform = {};
	WorldTransform BworldTransform = {};
};


class Player;

class TransCube : public Collider
{
public:
	TransCube();
	~TransCube();

	void Initialize();

	void Update();

	void Draw(ViewProjection view);

	void ChangeRandBulletState();

	Vector3 GetWorldPosition()override;
	void OnCollision()override;
	TransCubeReticle GetReticlePos() { return DirectionReticlePos_; }
	WorldTransform& GetWorldTransform() { return worldTransform; }
	Player* GetPlayer() { return player_; }

	void SetWorldTransform(WorldTransform w) { worldTransform = w; }
	void SetPlayer(Player* player) { player_ = player; }

	
private:

	void ReticlePosFanc();

	Model*model_;
	WorldTransform worldTransform = {};


	std::unique_ptr<ITransCubeState>state_ = nullptr;

	TransCubeReticle DirectionReticlePos_ = {};

	const float DirectionReticleSpace = 5.0f;

	Input* input = nullptr;
	Player *player_ = nullptr;
};
