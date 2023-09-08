#pragma once
#include <cstdint>

//プレイヤー陣営
const uint32_t kCollisionAttributePlayer = 0b1;
//プレイヤー球
const uint32_t kCollisionAttributePlayerBullet = 0b11;


//敵本隊
const uint32_t kCollisionAttributeEnemy = 0b100;
//敵の球
const uint32_t kCollisionAttributeEnemyBullet = 0b101;