#pragma once
#include "Model/Model.h"
#include "TransformationMatrix/WorldTransform.h"
#include <optional>

/// <summary>
/// プレイヤー
/// </summary>
class Player{
public:
	enum class Behavior {
		kRoot,//通常状態
		kDash,//ダッシュ状態
		kJump,//ジャンプ状態
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
	/// 通常行動初期化
	/// </summary>
	void BehaviorRootInitialize();

	/// <summary>
	/// 通常行動更新
	/// </summary>
	void BehaviorRootUpdate();

	/// <summary>
	/// ダッシュ行動初期化
	/// </summary>
	void BehaviorDashInitialize();

	/// <summary>
	/// ダッシュ行動更新
	/// </summary>
	void BehaviorDashUpdate();

	/// <summary>
	/// ジャンプ状態初期化
	/// </summary>
	void BehaviorJumpInitialize();

	/// <summary>
	/// ジャンプ状態更新
	/// </summary>
	void BehaviorJumpUpdate();

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
};

