#pragma once
#include <cstdint>

//プレイヤー陣営
const uint32_t kCollisionAttributePlayer       = 0b00001;
const uint32_t kCollisionAttributePlayerBullet = 0b00010;
const uint32_t kCollisionAttributeEnemy        = 0b00100;
const uint32_t kCollisionAttributeEnemyBullet  = 0b01000;


const uint32_t kCollisionmaskPlayer            = 0b11110;
const uint32_t kCollisionmaskPlayerBullet      = 0b10100;
const uint32_t kCollisionmaskEnemy             = 0b11011;
const uint32_t kCollisionmaskEnemyBullet       = 0b10001;


//int PbA = 0b00010;
//int EbM = 0b10111;



//int EbA = 0b01100;
//int PbM = 0b10101;
