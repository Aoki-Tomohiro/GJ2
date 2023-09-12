#pragma once

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

class ExplanationScene : public IScene {
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	ExplanationScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ExplanationScene();

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


	//黒色の背景
	Sprite* backSprite_ = nullptr;
	uint32_t backTextureHandle_ = 0u;

	//操作説明
	Sprite* explanationSprite_[3] = { nullptr };
	uint32_t explanationHandle_[3] = { 0u };
	int32_t explanationCurrentNumber_ = 0;

	//Trigger専用のタイマー
	int32_t triggerButtonBTime_ = 0;

	float_t fadeInTime_ = 0.0f;
	float_t fadeOutTime_ = 0.0f;

	bool isTriggerB_ = false;



	float COLOR_BIND = 1.0f;
	Vector4 transparency_ = { 0.0f,0.0f,0.0f,0.0f };


	//FadeIn用
	bool isFadeInMode_ = false;

	//FadeOut用
	bool isFadeOutMode_ = false;
	float fadeInterval_ = 0.0f;

	const int SECOND_ = 60;
	int32_t loadingTime = 0;

	//titleへ
	
	bool explanationChange_ = false;


	Audio* bgmAudio_ = nullptr;
	uint32_t bgmHandle_ = 0u;


	Audio* selectSEAudio_ = nullptr;
	uint32_t returnSEHandle_ = 0u;
	uint32_t explanationSEHandle_ = 0u;


};