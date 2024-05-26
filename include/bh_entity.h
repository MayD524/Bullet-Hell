#pragma once

#ifndef ENTITY_H
#define ENTITY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <raylib.h>
#include <raymath.h>
#include <util.h>
#include <math.h>
#include <float.h>

static const Vector2 VEC2_ZERO = { 0.0f, 0.0f };
static const Vector2 VEC2_ONE = { 1.0f, 1.0f };

#define MAX_ENTITIES 100000
#define MAX_TEXTURES 100000

// allows for us to have different types of entities.
// and if we run out of entity slots, we can remove
// other entities based on their priority rank.
// anything HIGH->Player cannot be killed via time
typedef enum {
    NONE,
    LOW,
    MEDIUM,
    HIGH,    
    CRITICAL,
    PLAYER,
} PriorityRank;

typedef struct s_Tag {
    const char* tag;
    int tag_id;

    struct s_Tag* next;
    struct s_Tag* prev;
} t_Tag;

typedef struct s_Entity {
    char* entity_name;
    unsigned int num_tags;
    int layer;
    
    t_Tag* root_tag;

    void* entity_data;

    Vector2 position;
    Vector2 velocity;
    Vector2 sprite_origin;

    Texture2D texture;
    Rectangle source;
    float rotation;
    float scale;

    bool is_active;
    float health;

    float friction;
    float mass;
    float speed;

    PriorityRank priority_rank;
    float max_life_time;
    float current_life_time;

    void (*update)(struct s_Entity* entity, float delta_time);
    // other update functions will call on_collision
    void (*on_collision)(struct s_Entity* other, struct s_Entity* entity);
    void (*on_destroy)(struct s_Entity* entity);

} t_Entity;

typedef struct s_Texture {
    char* texture_name;
    Texture2D texture;
} t_Texture;

typedef struct s_EntitySystem {
    t_Entity entities[MAX_ENTITIES];
    
    // Number of entities in the entity system.
    int num_entities;

    t_Texture textures[MAX_TEXTURES];
    int num_textures;
    
} t_EntitySystem;

extern t_EntitySystem* g_entity_system;

// Entity system functions.
t_EntitySystem* create_entity_system();
void destroy_entity_system(t_EntitySystem* entity_system);
int next_free_entity_slot(const t_EntitySystem* entity_system, const PriorityRank* priority_rank);
void update(t_EntitySystem* entity_system);
void render(t_EntitySystem* entity_system);

int load_texture(t_EntitySystem* entity_system, char* texture_path);
Texture2D get_texture(t_EntitySystem* entity_system, int id);

//Entity functions.
t_Entity* create_entity(t_EntitySystem* es, char* entity_name, char* texturePath, Vector2 start_position, Vector2 start_velocity, PriorityRank priority_rank, float max_life_time, float scale, float health);
t_Entity* get_entity(t_EntitySystem* es, int index);
t_Entity* get_entity_by_name(t_EntitySystem* es, char* entity_name);

void print_movement(const t_Entity* entity);
bool are_entities_colliding(const t_Entity* entity_a, const t_Entity* entity_b);

void print_tags(const t_Entity* entity);
void add_tag(t_Entity* entity, const char* tag);
const char** get_tags(const t_Entity* entity);
void remove_tag_at(t_Entity* entity, int index);
void remove_tag(t_Entity* entity, const char* tag);
int has_tag(const t_Entity* entity, const char* tag);
void remove_all_tags(t_Entity* entity);

Rectangle get_entity_source(const t_Entity* entity);
Rectangle get_entity_destination(const t_Entity* entity);

void destroy_entity(t_Entity* entity);


#endif // ENTITY_H