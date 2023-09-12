#include "Doom.h"

void Doom::Initialize() {
	model_ = std::make_unique<Model>();

	model_.get()->CreateFromOBJ("Resources/Doom", "Doom.obj");
	worldTransform.matWorld_ = MakeIdentity4x4();
	
	worldTransform.scale_ = { 35,35,35 };
	worldTransform.UpdateMatrix();
}

void Doom::Draw(ViewProjection view) {
	model_.get()->Draw(worldTransform, view);

}



