#include <bh_lua_bindings.h>

int lua_create_entity(lua_State *L) {
    const char* entity_name = lua_tostring(L, 1);
    const char* texturePath = lua_tostring(L, 2);
    Vector2 start_position = { (float)lua_tonumber(L, 3), (float)lua_tonumber(L, 4) };
    Vector2 start_velocity = { (float)lua_tonumber(L, 5), (float)lua_tonumber(L, 6) };
    int priority_rank = lua_tointeger(L, 7);
    float max_life_time = (float)lua_tonumber(L, 8);
    float scale = (float)lua_tonumber(L, 9);
    float health = (float)lua_tonumber(L, 10);

    t_Entity* entity = create_entity(g_entity_system, (char*)entity_name, (char*)texturePath, start_position, start_velocity, priority_rank, max_life_time, scale, health);
    lua_pushlightuserdata(L, entity);
    return 1;
}

int lua_is_entity_active(lua_State *L) {
    t_Entity* entity = (t_Entity*)lua_touserdata(L, 1);
    lua_pushboolean(L, !entity || entity->is_active);
    return 1;
}

int lua_get_position(lua_State *L) {
    t_Entity* entity = (t_Entity*)lua_touserdata(L, 1);
    if (!entity) {
        panic("lua_get_position: entity is null");
    }
    Vector2 position = entity->position;
    lua_pushnumber(L, position.x);
    lua_pushnumber(L, position.y);
    return 2;
}

int lua_set_position(lua_State *L) {
    t_Entity* entity = (t_Entity*)lua_touserdata(L, 1);

    if (!entity) {
        panic("lua_get_position: entity is null");
    }

    float x = (float)lua_tonumber(L, 2);
    float y = (float)lua_tonumber(L, 3);
    Vector2 position = { x, y };

    entity->position = position;
    return 0;
}

int lua_get_entity_by_name(lua_State *L) {
    const char* entity_name = lua_tostring(L, 1);
    t_Entity* entity = get_entity_by_name(g_entity_system, (char*)entity_name);
    lua_pushlightuserdata(L, entity);
    return 1;
}

int lua_get_entity_count(lua_State *L) {
    int count = g_entity_system->num_entities;
    lua_pushinteger(L, count);
    return 1;
}

int lua_get_entity(lua_State *L) {
    int index = lua_tointeger(L, 1);
    t_Entity* entity = get_entity(g_entity_system, index);
    lua_pushlightuserdata(L, entity);
    return 1;
}

int lua_print_movement(lua_State *L) {
    t_Entity* entity = (t_Entity*)lua_touserdata(L, 1);
    print_movement(entity);
    return 0;
}

int lua_are_entities_colliding(lua_State *L) {
    t_Entity* entity_a = (t_Entity*)lua_touserdata(L, 1);
    t_Entity* entity_b = (t_Entity*)lua_touserdata(L, 2);
    bool result = are_entities_colliding(entity_a, entity_b);
    lua_pushboolean(L, result);
    return 1;
}

int lua_deal_damage(lua_State *L) {
    t_Entity* entity = (t_Entity*)lua_touserdata(L, 1);
    float damage = (float)lua_tonumber(L, 2);
    entity->health -= damage;
    return 0;
}

int lua_set_entity_active(lua_State *L) {
    t_Entity* entity = (t_Entity*)lua_touserdata(L, 1);
    bool active = lua_toboolean(L, 2);
    entity->is_active = active;
    return 0;
}

int lua_print_tags(lua_State *L) {
    t_Entity* entity = (t_Entity*)lua_touserdata(L, 1);
    print_tags(entity);
    return 0;
}

int lua_add_tag(lua_State *L) {
    t_Entity* entity = (t_Entity*)lua_touserdata(L, 1);
    const char* tag = lua_tostring(L, 2);
    add_tag(entity, tag);
    return 0;
}

int lua_get_tags(lua_State *L) {
    t_Entity* entity = (t_Entity*)lua_touserdata(L, 1);
    const char** tags = get_tags(entity);

    lua_newtable(L);
    for (int i = 0; tags[i] != NULL; i++) {
        lua_pushstring(L, tags[i]);
        lua_rawseti(L, -2, i + 1);
    }
    return 1;
}

int lua_remove_tag_at(lua_State *L) {
    t_Entity* entity = (t_Entity*)lua_touserdata(L, 1);
    int index = lua_tointeger(L, 2);
    remove_tag_at(entity, index);
    return 0;
}

int lua_remove_tag(lua_State *L) {
    t_Entity* entity = (t_Entity*)lua_touserdata(L, 1);
    const char* tag = lua_tostring(L, 2);
    remove_tag(entity, tag);
    return 0;
}

int lua_has_tag(lua_State *L) {
    t_Entity* entity = (t_Entity*)lua_touserdata(L, 1);
    const char* tag = lua_tostring(L, 2);
    int result = has_tag(entity, tag);
    lua_pushboolean(L, result != -1);
    return 1;
}

int lua_remove_all_tags(lua_State *L) {
    t_Entity* entity = (t_Entity*)lua_touserdata(L, 1);
    remove_all_tags(entity);
    return 0;
}

// Texture functions

int lua_load_texture(lua_State *L) {
    const char* texture_path = lua_tostring(L, 1);
    int texture_id = load_texture(g_entity_system, (char*)texture_path);
    lua_pushinteger(L, texture_id);
    return 1;
}

int lua_get_texture(lua_State *L) {
    int texture_id = lua_tointeger(L, 1);
    Texture2D texture = get_texture(g_entity_system, texture_id);
    lua_pushlightuserdata(L, &texture);
    return 1;
}

// Enemy data functions

int lua_create_enemy(lua_State *L) {
    const char* entity_texture_path = lua_tostring(L, 1);
    float x = (float)lua_tonumber(L, 2);
    float y = (float)lua_tonumber(L, 3);
    float health = (float)lua_tonumber(L, 4);
    float size = (float)lua_tonumber(L, 5);
    float damage = (float)lua_tonumber(L, 6);
    float base_speed = (float)lua_tonumber(L, 7);

    Vector2 position = { x, y };

    t_Entity* entity = create_enemy((char*) entity_texture_path, position, health, size, damage, base_speed);
    lua_pushlightuserdata(L, entity);
    return 1;
}

int lua_set_enemy_data(lua_State *L) {
    t_Entity* entity = (t_Entity*)lua_touserdata(L, 1);
    t_EnemyData* enemy_data = (t_EnemyData*)entity->entity_data;
    if (!enemy_data) {
        return -1;
    }
    enemy_data->move_cooldown = (float)lua_tonumber(L, 2);
    enemy_data->shoot_cooldown = (float)lua_tonumber(L, 3);
    enemy_data->damage = (float)lua_tonumber(L, 4);
    return 0;
}

int lua_get_enemy_data(lua_State *L) {
    t_Entity* entity = (t_Entity*)lua_touserdata(L, 1);
    t_EnemyData* enemy_data = (t_EnemyData*)entity->entity_data;
    if (enemy_data) {
        lua_pushnumber(L, enemy_data->move_cooldown);
        lua_pushnumber(L, enemy_data->shoot_cooldown);
        lua_pushnumber(L, enemy_data->damage);
        return 3;
    }
    return 0;
}

int lua_set_enemy_move_target(lua_State *L) {
    t_Entity* entity = (t_Entity*)lua_touserdata(L, 1);

    Vector2 position = { (float)lua_tonumber(L, 2), (float)lua_tonumber(L, 3) };
    float wait_time = (float)lua_tonumber(L, 4);
    float speed_mult = (float)lua_tonumber(L, 5);

    add_enemy_move_target(entity, position, wait_time, speed_mult);

    return 0;
}

int lua_get_enemy_move_target(lua_State *L) {
    t_Entity* entity = (t_Entity*)lua_touserdata(L, 1);
    t_EnemyData* enemy_data = (t_EnemyData*)entity->entity_data;

    if (enemy_data && enemy_data->move_target) {
        lua_pushnumber(L, enemy_data->move_target->position.x);
        lua_pushnumber(L, enemy_data->move_target->position.y);
        lua_pushnumber(L, enemy_data->move_target->wait_time);
        lua_pushnumber(L, enemy_data->move_target->speed_mult);
        return 4;
    }
    return 0;
}
