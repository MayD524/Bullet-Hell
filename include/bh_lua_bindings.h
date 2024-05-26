#pragma once

#ifndef BH_LUA_BINDINGS_H
#define BH_LUA_BINDINGS_H

#include <lua/lua.h>
#include <raylib.h>
#include <bh_entity.h>
#include <bh_enemy.h>

// Lua bindings for entity functions
int lua_create_entity(lua_State *L);
int lua_get_entity(lua_State *L);
int lua_get_entity_count(lua_State *L);
int lua_get_entity_by_name(lua_State *L);
int lua_create_enemy(lua_State *L);
int lua_deal_damage(lua_State *L);
int lua_set_entity_active(lua_State *L);
int lua_is_entity_active(lua_State *L);
int lua_deal_damage(lua_State *L);
int lua_get_position(lua_State *L);
int lua_set_position(lua_State *L);
int lua_print_movement(lua_State *L);
int lua_are_entities_colliding(lua_State *L);
int lua_print_tags(lua_State *L);
int lua_add_tag(lua_State *L);
int lua_get_tags(lua_State *L);
int lua_remove_tag_at(lua_State *L);
int lua_remove_tag(lua_State *L);
int lua_has_tag(lua_State *L);
int lua_remove_all_tags(lua_State *L);

// Lua bindings for texture functions
int lua_load_texture(lua_State *L);
int lua_get_texture(lua_State *L);

// lua bindings for enemy functions
int lua_create_enemy_data(lua_State *L);
int lua_get_enemy_data(lua_State *L);
int lua_set_enemy_data(lua_State *L);
int lua_create_enemy_move_target(lua_State *L);
int lua_get_enemy_move_target(lua_State *L);
int lua_set_enemy_move_target(lua_State *L);

#endif // BH_LUA_BINDINGS_H