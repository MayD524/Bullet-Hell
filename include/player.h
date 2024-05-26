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

typedef struct s_PlayerData {
    int bullet_cooldown;
} t_PlayerData;

void on_player_destroy(Player* p);
void update_player( Player* p, float dt);
Player* create_player(t_EntitySystem* es, Vector2 start_position);

#endif