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
#include "Player/Player.h"
#include "FollowCamera/FollowCamera.h"

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
	CollisionManager* collisionManager_ = nullptr;
	//DebugCamera
	DebugCamera* debugCamera_ = nullptr;
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

	//追従カメラ
	std::unique_ptr<FollowCamera> followCamera_ = nullptr;

	//Skydome
	std::unique_ptr<Model> modelSkydome_ = nullptr;
	WorldTransform worldTransform_{};
};