#pragma once
#include "Sprite/Sprite.h"
#include "TransformationMatrix/WorldTransform.h"
#include "TransformationMatrix/ViewProjection.h"
#include "../GameObject/Player/Player.h"

class FollowCamera {
public:
	/// <summary>
	/// 更新
	/// </summary>
	void Update(Behavior behavior);

	/// <summary>
	/// 追従対象を設定
	/// </summary>
	/// <param name="target"></param>
	void SetTarget(const WorldTransform* target);

	/// <summary>
	/// ビュープロジェクションを取得
	/// </summary>
	/// <returns></returns>
	const ViewProjection& GetViewProjection() { return viewProjection_; };

private:
	/// <summary>
	/// オフセット計算
	/// </summary>
	/// <returns></returns>
	Vector3 Offset();

	/// <summary>
	/// リセット
	/// </summary>
	void Reset();

private:
	//ビュープロジェクション
	ViewProjection viewProjection_{};
	//追従対象
	const WorldTransform* target_{};
	//目標角度
	float destinationAngleX_ = 0.0f;
	float destinationAngleY_ = 0.0f;
	//追従対象の残像座標
	Vector3 interTarget_{};
	//ロックオンターゲット
	const WorldTransform* rockonTarget_{};
	//オフセット
	Vector3 offset_{ 0.0f,2.0f,-30.0f };
	//角度制限
	float minRotateX = 0.0f;
	float maxRotateX = 0.5f;
	//自キャラの状態
	Behavior playerBehavior_ = Behavior::kRoot;
};

