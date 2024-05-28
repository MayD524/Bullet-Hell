#include <bh_lua_bindings.h>

Color bh_lua_color_unpack(lua_State* L, int index) {
    if (!lua_istable(L, index)) { 
        return WHITE;
    }
    
    lua_getfield(L, index, "r"); // Get the field "r" from the table
    int color_r = (int)lua_tonumber(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, index, "g"); // Get the field "g" from the table
    int color_g = (int)lua_tonumber(L, -1);
    lua_pop(L, 1); 

    lua_getfield(L, index, "b"); // Get the field "b" from the table
    int color_b = (int)lua_tonumber(L, -1);
    lua_pop(L, 1); 

    lua_getfield(L, index, "a"); // Get the field "a" from the table
    int color_a = lua_isnil(L, -1) ? 255 : (int)lua_tonumber(L, -1);
    lua_pop(L, 1);

    return (Color){color_r, color_g, color_b, color_a};
}

Vector2 bh_lua_vec2_unpack(lua_State* L, int index) {
    if (!lua_istable(L, index)) { 
        return VEC2_ZERO;
    }

    Vector2 vec2;

    lua_getfield(L, index, "x"); // Get the field "x" from the table
    vec2.x = (float)lua_tonumber(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, index, "y"); // Get the field "y" from the table
    vec2.y = (float)lua_tonumber(L, -1);
    lua_pop(L, 1);

    return vec2;
}

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
    if (entity == NULL) { return 0; }
    lua_pushboolean(L, !entity || entity->is_active);
    return 1;
}

int lua_get_position(lua_State *L) {
    t_Entity* entity = (t_Entity*)lua_touserdata(L, 1);
    if (entity == NULL) { return 0; }
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
    if (entity == NULL) { return 0; }

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
    if (entity == NULL) { return 0; }
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
    if (entity == NULL) { return 0; }
    float damage = (float)lua_tonumber(L, 2);
    entity->health -= damage;
    return 0;
}

int lua_set_entity_active(lua_State *L) {
    t_Entity* entity = (t_Entity*)lua_touserdata(L, 1);
    if (entity == NULL) { return 0; }
    bool active = lua_toboolean(L, 2);
    entity->is_active = active;
    return 0;
}

int lua_print_tags(lua_State *L) {
    t_Entity* entity = (t_Entity*)lua_touserdata(L, 1);
    if (entity == NULL) { return 0; }
    print_tags(entity);
    return 0;
}

int lua_add_tag(lua_State *L) {
    t_Entity* entity = (t_Entity*)lua_touserdata(L, 1);
    if (entity == NULL) { return 0; }
    const char* tag = lua_tostring(L, 2);
    add_tag(entity, tag);
    return 0;
}

int lua_get_tags(lua_State *L) {
    t_Entity* entity = (t_Entity*)lua_touserdata(L, 1);
    if (entity == NULL) { return 0; }
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
    if (entity == NULL) { return 0; }
    int index = lua_tointeger(L, 2);
    remove_tag_at(entity, index);
    return 0;
}

int lua_remove_tag(lua_State *L) {
    t_Entity* entity = (t_Entity*)lua_touserdata(L, 1);
    if (entity == NULL) { return 0; }
    const char* tag = lua_tostring(L, 2);
    remove_tag(entity, tag);
    return 0;
}

int lua_has_tag(lua_State *L) {
    t_Entity* entity = (t_Entity*)lua_touserdata(L, 1);
    if (entity == NULL) { return 0; }
    const char* tag = lua_tostring(L, 2);
    int result = has_tag(entity, tag);
    lua_pushboolean(L, result != -1);
    return 1;
}

int lua_remove_all_tags(lua_State *L) {
    t_Entity* entity = (t_Entity*)lua_touserdata(L, 1);
    if (entity == NULL) { return 0; }
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
    if (entity == NULL) { return 0; }
    t_EnemyData* enemy_data = (t_EnemyData*)entity->entity_data;
    if (!enemy_data) {
        return 0;
    }
    lua_to_struct(L, 2, enemy_data, EnemyMeta);
    return 0;
}

int lua_get_enemy_data(lua_State *L) {
    t_Entity* entity = (t_Entity*)lua_touserdata(L, 1);
    if (entity == NULL) { return 0; }
    t_EnemyData* enemy_data = (t_EnemyData*)entity->entity_data;
    if (enemy_data) {
        push_struct_to_lua(L, enemy_data, EnemyMeta);
        return 1;
    }
    return 0;
}

int lua_set_enemy_move_target(lua_State *L) {
    t_Entity* entity = (t_Entity*)lua_touserdata(L, 1);
    if (entity == NULL) { return 0; }

    Vector2 position = { (float)lua_tonumber(L, 2), (float)lua_tonumber(L, 3) };
    float wait_time = (float)lua_tonumber(L, 4);
    float speed_mult = (float)lua_tonumber(L, 5);

    add_enemy_move_target(entity, position, wait_time, speed_mult);

    return 0;
}

int lua_get_enemy_move_target(lua_State *L) {
    t_Entity* entity = (t_Entity*)lua_touserdata(L, 1);
    if (entity == NULL) { return 0; }
    t_EnemyData* enemy_data = (t_EnemyData*)entity->entity_data;

    if (enemy_data && enemy_data->move_target) {
        lua_pushnumber(L, enemy_data->move_target->position.x);
        lua_pushnumber(L, enemy_data->move_target->position.y);
        lua_pushnumber(L, enemy_data->move_target->wait_time);
        lua_pushnumber(L, enemy_data->move_target->speed_mult);
        return 4;
    } else {
        // return -1 on all channels if no target is set
        lua_pushnumber(L, -1);
        lua_pushnumber(L, -1);
        lua_pushnumber(L, -1);
        lua_pushnumber(L, -1);
        return 4;
    }
}

int lua_get_player_score(lua_State *L) {
    t_Entity* entity = (t_Entity*)lua_touserdata(L, 1);
    if (entity == NULL) { return 0; }
    t_PlayerData* player_data = (t_PlayerData*)entity->entity_data;

    if (player_data) {
        lua_pushnumber(L, player_data->player_score);
        return 1;
    }

    lua_pushnumber(L, -1);
    return 1;
}

int lua_set_player_score(lua_State *L) {
    t_Entity* entity = (t_Entity*)lua_touserdata(L, 1);
    if (entity == NULL) { return 0; }
    t_PlayerData* player_data = (t_PlayerData*)entity->entity_data;
    
    if (!player_data) {
        return 0;
    }

    long double new_score = lua_tonumber(L, 2);
    player_data->player_score = new_score;
    return 0;
}

int lua_add_player_power(lua_State *L) {
    t_Entity* entity = (t_Entity*)lua_touserdata(L, 1);
    if (entity == NULL) { return 0; }
    t_PlayerData* player_data = (t_PlayerData*)entity->entity_data;
    
    if (!player_data) {
        return 0;
    }
    
    long double amount = (long double)lua_tonumber(L, 2);
    player_data->player_power += amount;
}
int lua_remove_player_power(lua_State *L) {
    t_Entity* entity = (t_Entity*)lua_touserdata(L, 1);
    if (entity == NULL) { return 0; }
    if (entity == NULL) { return 0; }

    t_PlayerData* player_data = (t_PlayerData*)entity->entity_data;
    
    if (!player_data) {
        return 0;
    }
    
    long amount = (long)lua_tonumber(L, 2);
    player_data->player_power -= amount;
}
int lua_get_player_power(lua_State *L) {
    t_Entity* entity = (t_Entity*)lua_touserdata(L, 1);
    if (entity == NULL) { return 0; }
    if (entity == NULL) { return 0; }

    t_PlayerData* player_data = (t_PlayerData*)entity->entity_data;
    
    if (!player_data) {
        return 0;
    }

    lua_pushnumber(L, player_data->player_power);
    return 1;
}

int lua_add_player_life(lua_State *L) {
    t_Entity* entity = (t_Entity*)lua_touserdata(L, 1);
    if (entity == NULL) { return 0; }
    if (entity == NULL) { return 0; }

    t_PlayerData* player_data = (t_PlayerData*)entity->entity_data;
    
    if (!player_data) {
        return 0;
    }

    int amount = (int)lua_tonumber(L, 2);
    player_data->lives_left += amount;

    return 0;
}
int lua_remove_player_life(lua_State *L) {
    t_Entity* entity = (t_Entity*)lua_touserdata(L, 1);
    if (entity == NULL) { return 0; }
    if (entity == NULL) { return 0; }

    t_PlayerData* player_data = (t_PlayerData*)entity->entity_data;
    
    if (!player_data) {
        return 0;
    }

    int amount = (int)lua_tonumber(L, 2);
    player_data->lives_left -= amount;
    return 0;
}
int lua_get_player_lives(lua_State *L) {
    t_Entity* entity = (t_Entity*)lua_touserdata(L, 1);
    if (entity == NULL) { return 0; }
    if (entity == NULL) { return 0; }

    t_PlayerData* player_data = (t_PlayerData*)entity->entity_data;
    
    if (!player_data) {
        return 0;
    }

    lua_pushnumber(L, player_data->lives_left);
    return 1;
}

int lua_get_player_class(lua_State *L) {
    t_Entity* entity = (t_Entity*)lua_touserdata(L, 1);
    if (entity == NULL) { return 0; }
    if (entity == NULL) { return 0; }
    t_PlayerData* player_data = (t_PlayerData*)entity->entity_data;
    
    if (!player_data) {
        return 0;
    }

    lua_pushnumber(L, player_data->player_class);
    return 1;
}
int lua_set_player_class(lua_State *L) {
    t_Entity* entity = (t_Entity*)lua_touserdata(L, 1);
    if (entity == NULL) { return 0; }
    if (entity == NULL) { return 0; }

    t_PlayerData* player_data = (t_PlayerData*)entity->entity_data;
    
    if (!player_data) {
        return 0;
    }

    PlayerClass new_class = (PlayerClass)lua_tonumber(L, 2);
    player_data->player_class = new_class;

    return 0;
}

int lua_heal_entity(lua_State *L) {
    t_Entity* entity = (t_Entity*)lua_touserdata(L, 1);
    if (entity == NULL) { return 0; }
    if (entity == NULL) { return 0; }

    float amount = (float)lua_tonumber(L, 2);
    entity->health += amount;
    return 0;
}

int lua_get_entity_health(lua_State *L) {
    t_Entity* entity = (t_Entity*)lua_touserdata(L, 1);
    if (entity == NULL) { return 0; }
    if (!entity)
        lua_pushnil(L);
    else
        lua_pushnumber(L, entity->health);
    return 1;
}


// Drawing functions

int lua_draw_text(lua_State *L) {
    const char* text = lua_tostring(L, 1);
    Vector2 pos = bh_lua_vec2_unpack(L, 2);
    float scale = (float)lua_tonumber(L, 3);

    Color color = bh_lua_color_unpack(L, 4);
    DrawText(text, pos.x, pos.y, scale, color);
    return 0;
}

int lua_draw_texture(lua_State *L) {
    Texture2D* texture = (Texture2D*)lua_touserdata(L, 1);
    Vector2 position = bh_lua_vec2_unpack(L, 2);
    float rotation = (float)lua_tonumber(L, 3);
    float scale = (float)lua_tonumber(L, 4);
    Color tint = bh_lua_color_unpack(L, 5);

    DrawTextureEx(*texture, position, rotation, scale, tint);
    return 0;
}

int lua_draw_line(lua_State *L) {
    Vector2 start = bh_lua_vec2_unpack(L, 1);
    Vector2 end = bh_lua_vec2_unpack(L, 2);
    Color color = bh_lua_color_unpack(L, 3);
    float thickness = (float)lua_tonumber(L, 4);

    DrawLine(start.x, start.y, end.x, end.y, color);
    return 0;
}

int lua_draw_rectangle(lua_State *L) {
    Vector2 top_left = bh_lua_vec2_unpack(L, 1);
    Vector2 bottom_right = bh_lua_vec2_unpack(L, 2);
    Color color = bh_lua_color_unpack(L, 3);
    float thickness = (float)lua_tonumber(L, 4);

    bool filled = lua_toboolean(L, 5);
    if (filled) {
        DrawRectangle(top_left.x, top_left.y, bottom_right.x - top_left.x, bottom_right.y - top_left.y, color);
    } else {
        DrawRectangleLines(top_left.x, top_left.y, bottom_right.x - top_left.x, bottom_right.y - top_left.y, color);
    }
    return 0;
}

int lua_draw_triangle(lua_State *L) {
    Vector2 v1 = bh_lua_vec2_unpack(L, 1);
    Vector2 v2 = bh_lua_vec2_unpack(L, 2);
    Vector2 v3 = bh_lua_vec2_unpack(L, 3);
    Color color = bh_lua_color_unpack(L, 4);
    float thickness = (float)lua_tonumber(L, 5);
    bool filled = lua_toboolean(L, 6);

    if (filled) {
        DrawTriangle(v1, v2, v3, color);
    } else {
        DrawTriangleLines(v1, v2, v3, color);
    }
    return 0;
}

int lua_draw_circle(lua_State *L) {
    Vector2 center = bh_lua_vec2_unpack(L, 1);
    float radius = (float)lua_tonumber(L, 2);
    Color color = bh_lua_color_unpack(L, 3);
    float thickness = (float)lua_tonumber(L, 4);
    bool filled = lua_toboolean(L, 5);
    
    if (filled) {
        DrawCircle(center.x, center.y, radius, color);
    } else {
        DrawCircleLines(center.x, center.y, radius, color);
    }
    return 0;
}

int lua_set_entity_rotation(lua_State *L){ 
    t_Entity* entity = (t_Entity*)lua_touserdata(L, 1);
    if (entity == NULL) { return 0; }
    if (entity == NULL) { return 0; }

    float rotation = (float)lua_tonumber(L, 2);
    entity->rotation = rotation;
    return 0;
}

int lua_get_entity_rotation(lua_State *L){ 
    t_Entity* entity = (t_Entity*)lua_touserdata(L, 1);
    if (entity == NULL) { return 0; }
    if (!entity)
        lua_pushnil(L);
    else
        lua_pushnumber(L, entity->rotation);
    return 1;
}



int lua_create_bullet(lua_State *L) {
    const char* texture_name = lua_tostring(L, 1);
    t_BulletOwner bullet_owner = (t_BulletOwner)lua_tonumber(L, 2);
    Vector2 position = bh_lua_vec2_unpack(L, 3);
    Vector2 velocity = bh_lua_vec2_unpack(L, 4);
    float health = (float)lua_tonumber(L, 5);
    float size = (float)lua_tonumber(L, 6);
    float damage = (float)lua_tonumber(L, 7);
    float rotation = (float)lua_tonumber(L, 8);

    Bullet* bullet = create_bullet((char*)texture_name, bullet_owner, position, velocity, health, size, damage, rotation);

    lua_pushlightuserdata(L, bullet);
    return 1;
}


int lua_get_bullet_data(lua_State *L) {
    t_Entity* entity = (t_Entity*)lua_touserdata(L, 1);
    if (entity == NULL) { return 0; }
    t_BulletData* bullet_data = (t_BulletData*)entity->entity_data;
    
    if (!bullet_data || !entity) {
        lua_pushnil(L);
        return 1;
    }

    push_struct_to_lua(L, bullet_data, BulletMeta);
    return 1;
}

int lua_set_scale(lua_State *L) {
    t_Entity* entity = (t_Entity*)lua_touserdata(L, 1);
    if (entity == NULL) { return 0; }
    if (entity == NULL) { return 0; }

    float scale = (float)lua_tonumber(L, 2);
    entity->scale = scale;
    return 0;
}
int lua_set_velocity(lua_State *L) {
    t_Entity* entity = (t_Entity*)lua_touserdata(L, 1);
    if (entity == NULL) { return 0; }
    if (entity == NULL) { return 0; }

    Vector2 velocity = bh_lua_vec2_unpack(L, 2);
    entity->velocity = velocity;
    return 0;
}

int lua_get_velocity(lua_State *L) {
    t_Entity* entity = (t_Entity*)lua_touserdata(L, 1);
    if (entity == NULL) { return 0; }
    push_struct_to_lua(L, &entity->velocity, Vector2Meta);
    return 1;
}

int lua_set_rotation(lua_State *L) {
    t_Entity* entity = (t_Entity*)lua_touserdata(L, 1);
    if (entity == NULL) { return 0; }
    if (entity == NULL) { return 0; }
    float rotation = (float)lua_tonumber(L, 2);
    entity->rotation = rotation;
    return 0;
}

int lua_get_rotation(lua_State *L) {
    t_Entity* entity = (t_Entity*)lua_touserdata(L, 1);
    if (entity == NULL) { return 0; }
    lua_pushnumber(L, entity->rotation);
    return 1;
}

int lua_get_entity_table(lua_State *L) {
    t_Entity* entity = (t_Entity*)lua_touserdata(L, 1);
    if (entity == NULL) { return 0; }
    if (entity != NULL && entity->is_active) 
        push_struct_to_lua(L, entity, EntityMeta);
    else
        lua_pushnil(L);
    return 1;
}

int lua_get_screen_size(lua_State *L) {
    Vector2 size = { .x = GetScreenWidth(),.y = GetScreenHeight() };
    push_struct_to_lua(L, &size, Vector2Meta);
    return 1;
}