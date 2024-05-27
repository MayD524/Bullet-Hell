#pragma once

#ifndef BH_ENEMY_H
#define BH_ENEMY_H

#include <bh_entity.h>
#include <bh_bullet.h>
#include <player.h>
#include <stdlib.h>
#include <stdio.h>
#include <raylib.h>
#include <raymath.h>
#include <util.h>

typedef t_Entity Enemy;

#define BASE_ENEMY_HEALTH         10
#define BASE_ENEMY_DAMAGE         1
#define BASE_ENEMY_SPEED          250.0f
#define BASE_ENEMY_SHOOT_COOLDOWN 1
#define BASE_ENEMY_SIZE           1.0f
#define ENEMY_TAG                 "ENEMY"
#define ENEMY_TEXTURE_PATH        "D:\\bullet_hell\\resources\\enemy.png"

typedef struct s_EnemyMoveTarget {
    Vector2 position;
    float wait_time;    // wait before going to the next target
                        // this time doesn't start till the enemy is in position
    float speed_mult;   // how fast to move to the next target
    struct s_EnemyMoveTarget* next;
} t_EnemyMoveTarget;

typedef struct s_EnemyData {
    t_EnemyMoveTarget* move_target;
    float total_targets;
    float move_cooldown;
    float shoot_cooldown;

    float damage;
} t_EnemyData;

static const Vector2 ENEMY_START_POSITION = (Vector2) { 400.0f, 400.0f };

Enemy* create_enemy(char* texture_path, Vector2 start_position, float health, float size, float damage, float base_speed);
void destroy_enemy(Enemy* enemy);
void add_enemy_move_target(Enemy* enemy, Vector2 position, float wait_time, float speed_mult);


void update_enemy(Enemy* enemy, float delta_time);
void on_enemy_collision(t_Entity* other, Enemy* enemy);
void destroy_enemy(Enemy* enemy);

#endif