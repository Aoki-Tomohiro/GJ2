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
#include "../GameObject/Enemy/TransCube/TransCube.h"
#include "../GameObject/Box/BoxManager.h"
#include "CollisionManager/CollisionManager.h"
#include <list>
#include"../GameObject/Enemy/Robot/EnemyRobot.h"

class GameScene : public IScene {
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

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

private:

	void SetCollisions();

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
	std::unique_ptr<Model> modelTransCube_;
	std::unique_ptr<TransCube> transCube_;
	std::unique_ptr<EnemyRobot> EnemyRobot_;

	//追従カメラ
	std::unique_ptr<FollowCamera> followCamera_ = nullptr;
	//地面
	std::unique_ptr<TestGround> ground_ = nullptr;

	//箱
	std::unique_ptr<BoxManager> boxManager_ = nullptr;

};