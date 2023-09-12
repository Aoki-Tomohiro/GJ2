#pragma once
#include "GameManager/IScene.h"
#include "TextureManager/TextureManager.h"
#include "Audio/Audio.h"
#include "Input/Input.h"
#include "PostProcess/PostProcess.h"
#include "DebugCamera/DebugCamera.h"
#include "ImGuiManager/ImGuiManager.h"
#include "TransformationMatrix/WorldTransform.h"
#include "TransformationMatrix/ViewProjection.h"
#include "Model/Model.h"
#include "Sprite/Sprite.h"
#include "CollisionManager/CollisionManager.h"
#include "../GameObject/Player/Player.h"
#include "../GameObject/Player/PlayerBullet.h"
#include "../GameObject/FollowCamera/FollowCamera.h"
#include "../GameObject/Ground/TestGround.h"
#include"../GameObject/Enemy/Robot/EnemyRobot.h"
#include "../GameObject/Box/BoxManager.h"
#include "../GameObject/Particle/ParticleEmitter.h"
#include "CollisionManager/CollisionManager.h"
#include <list>

class GameSceneRobot : public IScene{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameSceneRobot();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameSceneRobot();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(GameManager* gameManager)override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update(GameManager* gameManager)override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(GameManager* gameManager)override;

	/// <summary>
	/// 自機の弾を追加する
	/// </summary>
	void AddPlayerBullet(PlayerBullet* playerBullet);

	/// <summary>
	/// パーティクルを追加
	/// </summary>
	/// <param name="particleEmitter"></param>
	void AddParticleEmitter(ParticleEmitter* particleEmitter);

private:
	/// <summary>
	/// コライダーをセット
	/// </summary>
	void SetCollisions();

private:
	//TextureManager
	TextureManager* textureManager_ = nullptr;
	//Audio
	Audio* audio_ = nullptr;
	//Input
	Input* input_ = nullptr;
	//PostProcess
	PostProcess* postProcess_ = nullptr;
	//衝突マネージャー
	std::unique_ptr<CollisionManager> collisionManager_ = nullptr;
	//DebugCamera
	std::unique_ptr<DebugCamera> debugCamera_ = nullptr;
	//DebugCameraの切り替え
	bool isDebugCameraActive_ = false;
	//ビュープロジェクション
	ViewProjection viewProjection_{};

	//自キャラ
	std::vector<Model*> playerModels_{};
	std::unique_ptr<Model> modelPlayerHead_ = nullptr;
	std::unique_ptr<Model> modelPlayerBody_ = nullptr;
	std::unique_ptr<Model> modelPlayerL_arm_ = nullptr;
	std::unique_ptr<Model> modelPlayerR_arm_ = nullptr;
	std::unique_ptr<Player>player_ = nullptr;
	//自弾
	std::unique_ptr<Model> modelPlayerBullet_ = nullptr;
	std::list<std::unique_ptr<PlayerBullet>> playerBullets_{};

	//敵キャラ
	std::unique_ptr<EnemyRobot> EnemyRobot_;

	//追従カメラ
	std::unique_ptr<FollowCamera> followCamera_ = nullptr;

	//地面
	std::unique_ptr<TestGround> ground_ = nullptr;

	//箱
	std::unique_ptr<BoxManager> boxManager_ = nullptr;

	//パーティクル
	std::list<std::unique_ptr<ParticleEmitter>> particleEmitters_{};

	//BGM
	Audio* bgmAudio_ = nullptr;
	uint32_t bgmHandle_ = 0u;


	//SE
	Audio* enemyDamagedAudio_ = nullptr;
	uint32_t enemyDamagedHandle_ = 0u;


	//SceneChange用のフラグ
	bool changeToWin_ = false;
	bool changeToLose_ = false;



};