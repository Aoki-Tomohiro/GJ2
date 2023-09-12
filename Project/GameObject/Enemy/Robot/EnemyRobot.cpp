#include "EnemyRobot.h"
#include "GameSceneRobot/GameSceneRobot.h"

EnemyRobot::EnemyRobot()
{
}

EnemyRobot::~EnemyRobot()
{
	Bullets_.remove_if([](RobotBullet* bullet) {
		
			delete bullet;
			return true;
	});

	punch_.remove_if([](RobotPunch* bullet) {

		delete bullet;
		return true;
	});
}

void EnemyRobot::Initialize()
{
	enemy_.BodyModel = std::make_unique<Model>();
	enemy_.BodyModel->CreateFromOBJ("Resources/EnemyObj/EnemyRobot/Body", "EnemyRobotBody.obj");
	enemy_.BodyWorldTransform.matWorld_ = MakeIdentity4x4();
	enemy_.BodyWorldTransform.scale_ = { 28,28,28 };
	enemy_.BodyWorldTransform.translation_ = { 0,20,90 };
	//EnemyCore->CreateFromOBJ();
	HeadArmInit();
	UpdateMatrixs();
	state = std::make_unique<EnemeyRobotPunchState>();
	state->Initialize(this);
	MoveFlag = true;


	StartRTWorPos = enemy_.RarmWorldTransform.rotation_;
	EndRTWorPos = { 0,0,0.5 };
	StartRBWorPos = enemy_.RarmBoWorldTransform.rotation_;
	EndRBWorPos = { 0,0,2.0 };

	StartLTWorPos = enemy_.LarmWorldTransform.rotation_;
	EndLTWorPos = { 0,0,-0.5 };
	StartLBWorPos = enemy_.LarmBoWorldTransform.rotation_;
	EndLBWorPos = { 0,0,-2.0 };
	CoreModel_ = std::make_unique<Model>();
	CoreModel_->CreateFromOBJ("Resources/EnemyObj/EnemyRobot/Core", "EnemyRobotCore.obj");
	CoreWorldTransform.translation_.z = 73;
	CoreWorldTransform.translation_.y = 30;
	CoreWorldTransform.scale_ = { 3.0f,3.0f,3.0f };

	//衝突属性を設定
	SetCollisionAttribute(kCollisionAttributeEnemy);
	//衝突対象を自分の属性以外に設定
	SetCollisionMask(kCollisionMaskEnemy);
	SetRadius(3.5f);

	//スプライトの作成
	textureHandleHP1_ = TextureManager::GetInstance()->Load("Resources/TransCubeHP1.png");
	textureHandleHP2_ = TextureManager::GetInstance()->Load("Resources/TransCubeHP2.png");
	spriteHP1_ = std::make_unique<Sprite>();
	spriteHP1_->Create(textureHandleHP1_, { 320.0f,50.0f });
	spriteHP2_ = std::make_unique<Sprite>();
	spriteHP2_->Create(textureHandleHP2_, { 322.0f,52.0f });
}

void EnemyRobot::Update()
{

	if (MoveFlag&&isStateEndFlag)
	{		
		Flame++;
		enemy_.RarmWorldTransform.rotation_=EasingFanc(StartRTWorPos, EndRTWorPos, Flame, EndFlame);
		enemy_.RarmBoWorldTransform.rotation_ = EasingFanc(StartRBWorPos, EndRBWorPos, Flame, EndFlame);
		enemy_.LarmWorldTransform.rotation_ = EasingFanc(StartLTWorPos, EndLTWorPos, Flame, EndFlame);
		enemy_.LarmBoWorldTransform.rotation_ = EasingFanc(StartLBWorPos, EndLBWorPos, Flame, EndFlame);

		if (Flame==EndFlame)
		{
			MoveFlag = false;
			
			int r = std::rand() % 2;

			if (r == 1)
			{
				state.release();
				state = std::make_unique<EnemyRobotRandBulletState>();
				state->Initialize(this);
			}
			if (r==0)
			{
				state.release();
				state = std::make_unique<EnemeyRobotPunchState>();
				state->Initialize(this);
			}
			MoveFlag = false;
			isStateEndFlag = false;
			Flame = 0;
		}
	}

	BulletUp();
	punch_.remove_if([](RobotPunch* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});
	for (RobotPunch* bullet : punch_)
	{
		bullet->Update();
		if (bullet->GetWorldTransform().translation_.z<=-100)
		{
			bullet->SetIsDead(true);
		}
	}

	state->Update(this);
	UpdateMatrixs();
	CoreWorldTransform.UpdateMatrix();

	//体力ゲージの処理
	spriteScale_.x = Lerp(spriteScale_.x, life_ / 300.0f, 0.1f);
	spriteHP2_->SetScale(spriteScale_);
}

void EnemyRobot::Draw(ViewProjection view)
{
	state->Draw(this,view);
	CoreModel_->Draw(CoreWorldTransform, view);
	for (RobotBullet* bullet : Bullets_)
	{
		bullet->Draw(view);

	}
	for (RobotPunch* bullet : punch_)
	{
		bullet->Draw(view);
	}
	enemy_.HeadModel->Draw(enemy_.HeadWorldTransform, view);
	enemy_.BodyModel->Draw(enemy_.BodyWorldTransform, view);
	enemy_.LarmTopModel->Draw(enemy_.LarmWorldTransform, view);
	enemy_.LarmBoModel->Draw(enemy_.LarmBoWorldTransform, view);
	enemy_.RarmTopModel->Draw(enemy_.RarmWorldTransform, view);
	enemy_.RarmBoModel->Draw(enemy_.RarmBoWorldTransform, view);
}

void EnemyRobot::DrawUI() {
	spriteHP1_->Draw();
	spriteHP2_->Draw();
}

Vector3 EnemyRobot::GetWorldPosition()
{
	Vector3 result;

	result.x = CoreWorldTransform.matWorld_.m[3][0];
	result.y = CoreWorldTransform.matWorld_.m[3][1];
	result.z = CoreWorldTransform.matWorld_.m[3][2];

	return result;
}

void EnemyRobot::OnCollision()
{
	//パーティクルを発生させる
	ParticleEmitter* particleEmitter = new ParticleEmitter();
	particleEmitter->Initialize(EnemyRobot::GetWorldPosition());
	gameSceneRobot_->AddParticleEmitter(particleEmitter);
	life_--;
}

void EnemyRobot::BulletPushBack(Vector3 velocity,Vector3 pos)
{
	RobotBullet* bullet = new RobotBullet();
	bullet->Initialize(velocity, pos);
	Bullets_.push_back(bullet);

}

void EnemyRobot::PunchPushBack(Vector3 pos)
{
	RobotPunch* bullet = new RobotPunch();
	bullet->Initialize(pos);
	punch_.push_back(bullet);

}

float EnemyRobot::LerpMove(float pos)
{

	float result;

	result = pos < 0.5 ? 4 * pos * pos * pos : 1 - std::powf(-2.0f * pos + 2, 3) / 2.0f;

	return result;

}
Vector3 EnemyRobot::EasingFanc( Vector3 startv, Vector3 Endv, float &Flame, float EndFlame)
{
	Vector3 result;

	result.x = startv.x + (Endv.x - startv.x) * LerpMove(Flame / EndFlame);
	result.y = startv.y+ (Endv.y - startv.y)* LerpMove(Flame / EndFlame);
	result.z = startv.z + (Endv.z - startv.z) * LerpMove(Flame / EndFlame);
	return result;
}



void EnemyRobot::UpdateMatrixs()
{
	enemy_.BodyWorldTransform.UpdateMatrix();

	enemy_.HeadWorldTransform.UpdateMatrix();
	enemy_.LarmWorldTransform.UpdateMatrix();
	enemy_.LarmBoWorldTransform.UpdateMatrix();
	enemy_.RarmWorldTransform.UpdateMatrix();
	enemy_.RarmBoWorldTransform.UpdateMatrix();

}

void EnemyRobot::HeadArmInit()
{
	enemy_.HeadModel = std::make_unique<Model>();
	enemy_.HeadModel->CreateFromOBJ("Resources/EnemyObj/EnemyRobot/Head", "EnemyRobotHead.obj");
	enemy_.HeadWorldTransform.parent_ = &enemy_.BodyWorldTransform;

	enemy_.LarmTopModel = std::make_unique<Model>();
	enemy_.LarmTopModel->CreateFromOBJ("Resources/EnemyObj/EnemyRobot/LTopArm", "EnemyRobotLArm.obj");
	enemy_.LarmWorldTransform.parent_= &enemy_.BodyWorldTransform;
	enemy_.LarmWorldTransform.translation_.x = enemy_.LarmWorldTransform.translation_.x + 0.6f;
	enemy_.LarmWorldTransform.translation_.y = enemy_.LarmWorldTransform.translation_.y + 0.4f;
	enemy_.LarmBoModel = std::make_unique <Model>();
	enemy_.LarmBoModel->CreateFromOBJ("Resources/EnemyObj/EnemyRobot/LBoArm", "LBoArm.obj");
	enemy_.LarmBoWorldTransform.parent_ = &enemy_.LarmWorldTransform;
	enemy_.LarmBoWorldTransform.translation_.x = enemy_.LarmBoWorldTransform.translation_.x + 0.90f;
    enemy_.LarmBoWorldTransform.translation_.y = enemy_.LarmBoWorldTransform.translation_.y + 0.1f;
	enemy_.LarmBoWorldTransform.translation_.z = enemy_.LarmBoWorldTransform.translation_.z - 0.1f;

	enemy_.RarmTopModel = std::make_unique<Model>();
	enemy_.RarmTopModel->CreateFromOBJ("Resources/EnemyObj/EnemyRobot/RTopArm", "RTopArm.obj");
	enemy_.RarmWorldTransform.parent_ = &enemy_.BodyWorldTransform;
	enemy_.RarmWorldTransform.translation_.x = enemy_.RarmWorldTransform.translation_.x - 0.5f;
	enemy_.RarmWorldTransform.translation_.y = enemy_.RarmWorldTransform.translation_.y + 0.4f;
	enemy_.RarmBoModel = std::make_unique <Model>();
	enemy_.RarmBoModel->CreateFromOBJ("Resources/EnemyObj/EnemyRobot/RBoArm", "EnemyRBoArm.obj");
	enemy_.RarmBoWorldTransform.parent_ = &enemy_.RarmWorldTransform;
	enemy_.RarmBoWorldTransform.translation_.x = enemy_.RarmBoWorldTransform.translation_.x -1.0f;
	enemy_.RarmBoWorldTransform.translation_.z = enemy_.RarmBoWorldTransform.translation_.z - 0.3f;
	enemy_.RarmBoWorldTransform.translation_.y = enemy_.RarmBoWorldTransform.translation_.y+0.1f;


}

void EnemyRobot::BulletUp()
{
	Bullets_.remove_if([](RobotBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
		});



	for (RobotBullet* bullet : Bullets_) {

		if (bullet->GetPosition().x >= 80 || bullet->GetPosition().x <= -80)
		{
			bullet->SetIsDead(true);

		}
		if (bullet->GetPosition().z >= 80 || bullet->GetPosition().z <= -80)
		{
			bullet->SetIsDead(true);
		}

	}
	for (RobotBullet* bullet : Bullets_)
	{
		bullet->Update();
	}
}
