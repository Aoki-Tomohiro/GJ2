#pragma once
#include "Model/Model.h"
#include "TransformationMatrix/WorldTransform.h"

class Particle{
public:
	static const int kAliveTime = 60;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const Vector3& position);

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
	/// デスフラグを取得
	/// </summary>
	/// <returns></returns>
	bool isDead() { return isDead_; };

private:
	//モデル
	std::unique_ptr<Model> model_ = nullptr;
	//ワールドトランスフォーム
	WorldTransform worldTransform_{};
	//速度
	Vector3 velocity_{};
	//デスタイマー
	int32_t dethTimer_ = kAliveTime;
	//デスフラグ
	bool isDead_{};
};

