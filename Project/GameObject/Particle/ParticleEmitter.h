#pragma once
#include "Particle.h"
#include "TransformationMatrix/ViewProjection.h"
#include <list>

class ParticleEmitter{
public:
	static const int kParticleMax = 10;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="position"></param>
	void Initialize(const Vector3& position);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// デスフラグを取得
	/// </summary>
	/// <returns></returns>
	bool isDead() { return isDead_; };

private:
	//パーティクルのリスト
	std::list<std::unique_ptr<Particle>> particles_{};
	//デスフラグ
	bool isDead_ = false;
};

