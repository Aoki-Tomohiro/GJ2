#pragma once
#include <cstdint>

//プレイヤー陣営
const uint32_t kCollisionAttributePlayer       = 0b00001;
//プレイヤー球
const uint32_t kCollisionAttributePlayerBullet = 0b11011;


//敵本隊
const uint32_t kCollisionAttributeEnemy        = 0b00100;
//敵の球
const uint32_t kCollisionAttributeEnemyBullet  = 0b11111;