#pragma once
#include <cstdint>

//プレイヤー陣営
const uint32_t kCollisionAttributePlayer       = 0b00001;
const uint32_t kCollisionAttributePlayerBullet = 0b00010;
const uint32_t kCollisionAttributeEnemy        = 0b00100;
const uint32_t kCollisionAttributeEnemyBullet  = 0b01000;


const uint32_t kCollisionMaskPlayer            = 0b11110;
const uint32_t kCollisionMaskPlayerBullet      = 0b10100;
const uint32_t kCollisionMaskEnemy             = 0b11011;
const uint32_t kCollisionMaskEnemyBullet       = 0b10001;



