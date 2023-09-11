#pragma once

#include "GameManager/IScene.h"
#include <TextureManager/TextureManager.h>
#include <Audio/Audio.h>
#include <Input/Input.h>
#include <PostProcess/PostProcess.h>
#include <CollisionManager/CollisionManager.h>
#include <DebugCamera/DebugCamera.h>
#include "TransformationMatrix/ViewProjection.h"
#include <Model/Model.h>

#include <Sprite/Sprite.h>

//StatePatternの時は前方宣言を忘れずに
class GameManager;

class TitleScene : public IScene {
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	TitleScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~TitleScene();

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

	//Skydome
	WorldTransform worldTransform_{};


	//黒色の背景
	Sprite* backSprite_ = nullptr;
	uint32_t backTextureHandle_ = 0u;


	//タイトル画面の画像
	Sprite* sprite_ = nullptr;

	//テクスチャ用
	uint32_t textureHandle_ = 0u;

	//位置
	Vector2 position_ = {};

	//Trigger専用のタイマー
	int32_t triggerButtonBTime_ = 0;
	float_t fadeOutTime_ = 0;

	//FadeOut用
	bool isFadeOutMode_ = false;
	float COLOR_BIND = 1.0f;
	Vector4 transparency_ = { 0.0f,0.0f,0.0f,0.0f };
	float fadeInterval_ = 0.0f;

	const int SECOND_ = 60;
	int32_t loadingTime = 0;


	//BGM
	Audio* bgmAudio_ = nullptr;
	uint32_t bgmHandle_ = 0u;

	//SE
	Audio* startSEAudio_ = nullptr;
	uint32_t startSEHandle_ = 0u;



};