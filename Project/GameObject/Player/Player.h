#pragma once
#include "Model/Model.h"
#include "Sprite/Sprite.h"
#include "TransformationMatrix/WorldTransform.h"
#include "CollisionManager/Collider.h"
#include <optional>

class GameScene;
class TransCube;

/// <summary>
/// プレイヤー
/// </summary>
class Player : public Collider{
public:
	//強化状態の時間
	static const int kEnhancedStateTime = 60 * 10;

	enum class Behavior {
		kRoot,//通常状態
		kAttack,//攻撃状態
		kDash,//ダッシュ状態
		kBoxPush,//箱を押してる状態
	};

	//ダッシュ用ワーク
	struct WorkDash {
		//ダッシュ用の媒介変数
		uint32_t dashParameter_ = 0;
		uint32_t coolTime = 300;
	};

	//ジャンプ用ワーク
	struct WorkJump {
		//ジャンプ用の媒介変数
		bool flag = false;
		float power = 0;
	};

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	void Initialize(const std::vector<Model*> models);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection"></param>
	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// 衝突判定
	/// </summary>
	/// <returns></returns>
	void OnCollision()override;

	/// <summary>
	/// ワールドポジションを取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition()override;

	/// <summary>
	/// 箱との衝突判定
	/// </summary>
	void OnCollisionBox();

	/// <summary>
	/// 移動
	/// </summary>
	/// <param name="velocity"></param>
	void Move(const Vector3& velocity) { worldTransformBase_.translation_ = Add(worldTransformBase_.translation_, velocity); };

	/// <summary>
	/// 強化状態にする
	/// </summary>
	void SetEnhanced() { 
		isEnhancedState_ = true;
		enhancedStateTimer_ = kEnhancedStateTime;
	};

	/// <summary>
	/// 強化状態のフラグを取得
	/// </summary>
	/// <returns></returns>
	bool GetIsEnhancedState() { return isEnhancedState_; };

	/// <summary>
	/// ワールドトランスフォームの取得
	/// </summary>
	/// <returns></returns>
	const WorldTransform& GetWorldTransform() { return worldTransformBase_; };

	/// <summary>
	/// ビュープロジェクションをセット
	/// </summary>
	/// <param name="viewProjection"></param>
	void SetViewProjection(const ViewProjection* viewProjection) { viewProjection_ = viewProjection; };

	/// <summary>
	/// ゲームシーンをセット
	/// </summary>
	/// <param name="gameScene"></param>
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; };

	/// <summary>
	/// 敵キャラをセット
	/// </summary>
	/// <param name="transCube"></param>
	void SetTrancCube(TransCube* transCube) { transCube_ = transCube; };

	/// <summary>
	/// 移動量を取得
	/// </summary>
	/// <returns></returns>
	const Vector3& GetVelocity() { return velocity_; };

	/// <summary>
	/// ボックスを押しているかのフラグを取得
	/// </summary>
	/// <returns></returns>
	bool GetIsBoxPush() { return isBoxPush_; };

private:
	/// <summary>
	/// 通常行動初期化
	/// </summary>
	void BehaviorRootInitialize();

	/// <summary>
	/// 通常行動更新
	/// </summary>
	void BehaviorRootUpdate();

	/// <summary>
	/// 攻撃行動初期化
	/// </summary>
	void BehaviorAttackInitialize();

	/// <summary>
	/// 攻撃行動更新
	/// </summary>
	void BehaviorAttackUpdate();

	/// <summary>
	/// ダッシュ行動初期化
	/// </summary>
	void BehaviorDashInitialize();

	/// <summary>
	/// ダッシュ行動更新
	/// </summary>
	void BehaviorDashUpdate();

	/// <summary>
	/// 箱を押す状態の初期化
	/// </summary>
	void BehaviorBoxPushInitialize();

	/// <summary>
	/// 箱を押す状態の更新
	/// </summary>
	void BehaviorBoxPushUpdate();

	/// <summary>
	/// 発射処理
	/// </summary>
	/// <returns></returns>
	void Fire();

private:
	//モデル
	std::vector<Model*> models_{};
	//ワールドトランスフォーム
	WorldTransform worldTransformBase_{};
	WorldTransform worldTransformHead_{};
	WorldTransform worldTransformBody_{};
	WorldTransform worldTransformL_arm_{};
	WorldTransform worldTransformR_arm_{};
	//カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;
	//目標角度
	float destinationAngleY_ = 0.0f;
	//振る舞い
	Behavior behavior_ = Behavior::kRoot;
	//次の振る舞いのリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;
	//ダッシュ用の変数
	WorkDash workDash_;
	//ジャンプ用の変数
	WorkJump workJump_;
	//弾の発射タイマー
	int32_t bulletTimer_ = 0;
	//ゲームシーン
	GameScene* gameScene_ = nullptr;
	//敵キャラ
	TransCube* transCube_ = nullptr;
	//速度
	Vector3 velocity_{};
	//箱に触れているか
	bool isBoxPush_ = false;
	//強化状態
	bool isEnhancedState_ = false;
	int32_t enhancedStateTimer_ = 0;
};

