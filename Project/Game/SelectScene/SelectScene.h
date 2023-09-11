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

class SelectScene : public IScene {
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	SelectScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~SelectScene();

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

	WorldTransform worldTransform_{};




	//ステージ選択専用
	int32_t stageNumber_ = 0;

	float WIDTH_INTERVAL_;

	//タイトル画面の画像
	Sprite* sprite_ = nullptr;
	uint32_t textureHandle_ = 0u;

	//カーソル
	Sprite* cursorSprite_ = nullptr;
	uint32_t cursorTexture_ = 0u;
	Vector2 cursorPosition = {};

	//StageIcon
	Sprite* stageIconSprite_[2] = { nullptr };
	uint32_t stageIconTexture_[2] = { 0u };
	Vector2 stageIconPosition[2] = {};


	//タイトルに戻るためのアイコン
	Sprite* backToTitleSprite_ = nullptr;
	uint32_t backToTitleTexture_ = 0u;
	Vector2 backToTitlePosition_ = {};
	
	//黒色の背景
	Sprite* backSprite_ = nullptr;
	uint32_t backTextureHandle_ = 0u;

	//Trigger専用のタイマー
	int32_t triggerButtonBTime_ = 0;
	int32_t triggerButtonLeftTime_ = 0;
	int32_t triggerButtonRightTime_ = 0;

	float_t fadeInTime_ = 0.0f;
	float_t fadeOutTime_ = 0.0f;

	bool isTriggerLeft_ = false;
	bool isTriggerRight_ = false;



	float COLOR_BIND = 1.0f;
	Vector4 transparency_ = { 0.0f,0.0f,0.0f,0.0f };


	//FadeIn用
	bool isFadeInMode_ = false;

	//FadeOut用
	bool isFadeOutMode_ = false;
	float fadeInterval_ = 0.0f;

	const int SECOND_ = 60;
	int32_t loadingTime = 0;


};