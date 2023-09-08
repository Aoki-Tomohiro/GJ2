#pragma once
#include"Model/Model.h"
#include"state/TransCubeRandBullet/TransCubeRandBulletState.h"
#include"state/TransCubeGroundAttak/TransCubeGroundAttackState.h"
#include"Input/Input.h"
#include"../GameObject/Player/Player.h"
#include "CollisionManager/Collider.h"
#include"../GameObject/Enemy/TransCube/GroundAttack/TransCubeGroundAttack.h"

#include"ImGuiManager/ImGuiManager.h"
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

	void ChangeState(ITransCubeState* state);

	Vector3 GetWorldPosition()override;
	void OnCollision()override;
	
	TransCubeReticle GetReticlePos() { return DirectionReticlePos_; }
	WorldTransform& GetWorldTransform() { return worldTransform; }
	Player* GetPlayer() { return player_; }

	std::list<TransCubeBullet*>Getbullets() { return bullets_; }
	std::list<TransCubeGroundAttack*>&GetGroundBullets() { return GroundBullets_; }

	void SetWorldTransform(WorldTransform w) { worldTransform = w; }
	void SetPlayer(Player* player) { player_ = player; }
	void Setbullet(std::list<TransCubeBullet*>bullets) { bullets_ = bullets; }
	void SetGroundBullet(std::list<TransCubeGroundAttack*>groundBullet) { GroundBullets_ = groundBullet; }

	void PushBackBullet(Vector3 velocity, Vector3 pos);

	void PushBackGroundBullet(Vector3 pos);

private:

	void ReticlePosFanc();
	void BulletKill();

	Model*model_;
	WorldTransform worldTransform = {};


	ITransCubeState*state_ = nullptr;

	TransCubeReticle DirectionReticlePos_ = {};

	const float DirectionReticleSpace = 5.0f;

	//stateRandBulletの本体
	std::list<TransCubeBullet*>bullets_;

	std::list<TransCubeGroundAttack*>GroundBullets_;

	Input* input = nullptr;
	Player *player_ = nullptr;

	bool stateFlag = false;

};
