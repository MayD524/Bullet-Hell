#pragma once

#ifndef BH_BULLET_H
#define BH_BULLET_H

#include <bh_entity.h>

#define PLAYER_PROJECTILE_TAG       "PLAYER_BULLET"
#define BULLET_TAG                  "PROJECTILE"
#define DEFAULT_BULLET_LIFETIME     10.0f

typedef t_Entity Bullet; 

typedef enum e_BulletOwner {
    BULLET_NONE,
    BULLET_ENEMY,
    BULLET_PLAYER,
} t_BulletOwner;

typedef struct s_BulletData {
    t_BulletOwner owner;
    float damage;
} t_BulletData;

t_BulletData* create_bullet_data(t_BulletOwner owner, float damage);

Bullet* create_bullet(char* texture, t_BulletOwner owner, Vector2 start_position, Vector2 velocity, float health, float size, float damage, float rotation);
void destroy_bullet(Bullet* bullet);

#endif