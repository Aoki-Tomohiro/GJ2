#pragma once
#include "TransformationMatrix/WorldTransform.h"
#include "TransformationMatrix/ViewProjection.h"

class FollowCamera{
public:
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// オフセット計算
	/// </summary>
	/// <returns></returns>
	Vector3 Offset()const;

	/// <summary>
	/// リセット
	/// </summary>
	void Reset();

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
	//ビュープロジェクション
	ViewProjection viewProjection_{};
	//追従対象
	const WorldTransform* target_{};
	//目標角度
	float destinationAngleX_ = 0.0f;
	float destinationAngleY_ = 0.0f;
	//追従対象の残像座標
	Vector3 interTarget_{};
};

