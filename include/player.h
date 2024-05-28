#pragma once

#ifndef PLAYER_H
#define PLAYER_H

#include <bh_entity.h>
#include <bh_bullet.h>
#include <stdlib.h>
#include <stdio.h>
#include <raylib.h>
#include <util.h>

typedef t_Entity Player;

#define PLAYER_SHOOT_DELAY         5
#define PLAYER_MOVE_SPEED          250.0f
#define PLAYER_DEFAULT_HEALTH      100.0f
#define PLAYER_TAG                 "PLAYER"
#define PLAYER_LOCAL_NAME          "LOCAL_PLAYER"
#define PLAYER_TEXTURE_PATH        "D:\\bullet_hell\\resources\\log_t.png"

#define PLAYER_PROJECTILE_SCALE    0.5f
#define PLAYER_PROJECTILE_DAMAGE   10.0f
#define PLAYER_PROJECTILE_HEALTH   100.0f
#define PLAYER_PROJECTILE_LIFETIME 10.0f
#define PLAYER_PROJECTILE_PATH     "D:\\bullet_hell\\resources\\bullet.png"

static const Vector2 PLAYER_PROJECTILE_SPEED = { 0.0f, -500.0f };
static const Vector2 PLAYER_START_POSITION   = { 450.0f, 800.0f };


typedef enum {
    DEFAULT
} PlayerClass;

#define PLAYER_SCORE_PER_LEVEL      1000
#define PLAYER_SCORE_PER_SECOND     10
#define PLAYER_SCORE_PER_KILL       100
#define PLAYER_SCORE_PER_DEATH      -100


#define POWER_MULTIPLIER            10
#define POWER_PER_LEVEL             100
#define DEFAULT_PLAYER_CLASS        DEFAULT
#define PROJECTILES_PER_POWER_LEVEL 10
#define STARTING_SCORE              0
#define STARTING_LIVES              3
#define STARTING_POWER              1

typedef struct s_PlayerData {
    long double player_score;
    long player_power;
    int lives_left;
    int bullet_cooldown;
    PlayerClass player_class;
    Vector2 bullet_speed;
} t_PlayerData;

void on_player_destroy(Player* p);
void update_player( Player* p, float dt);
Player* create_player(t_EntitySystem* es, Vector2 start_position);

#endif