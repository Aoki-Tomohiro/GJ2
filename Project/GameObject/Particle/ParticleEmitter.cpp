#include "ParticleEmitter.h"

void ParticleEmitter::Initialize(const Vector3& position) {
	//パーティクルの作成
	for (int i = 0; i < kParticleMax; i++) {
		//パーティクルの作成
		Particle* particle = new Particle();
		particle->Initialize(position);
		//パーティクルをリストに追加する
		particles_.push_back(std::unique_ptr<Particle>(particle));
	}
}

void ParticleEmitter::Update() {
	//デスフラグの立ったパーティクルを削除
	particles_.remove_if([](std::unique_ptr<Particle>& particle) {
		if (particle->isDead()) {
			particle.reset();
			return true;
		}
		return false;
	});

	//パーティクルの更新
	for (const std::unique_ptr<Particle>& particle : particles_) {
		particle->Update();
	}

	//パーティクルがすべて消滅している場合デスフラグを立てる
	if (particles_.size() == 0) {
		isDead_ = true;
	}
}

void ParticleEmitter::Draw(const ViewProjection& viewProjection) {
	//パーティクルの描画
	for (const std::unique_ptr<Particle>& particle : particles_) {
		particle->Draw(viewProjection);
	}
}