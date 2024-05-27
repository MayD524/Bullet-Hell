#pragma once

#ifndef BH_METATABLES_H
#define BH_METATABLES_H

#include <player.h>
#include <bh_entity.h>
#include <bh_bullet.h>
#include <stddef.h>

#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>

#include <util.h>

typedef enum {
    TYPE_STRING,
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_BOOL,
    TYPE_DOUBLE,
    TYPE_CHAR,
    TYPE_FUNC_PTR,
    TYPE_ENUM,
    TYPE_VOID_PTR,
    TYPE_STRUCT,
    TYPE_UINT,
    TYPE_UCHAR,
    TYPE_LINKED_LIST
} FieldType;

typedef struct FieldMeta {
    const char* name;
    FieldType type;
    size_t offset;
    struct FieldMeta* sub_meta;  // For nested structs
    const char** enum_strings;   // For enum types
    struct FieldMeta* list_meta; // For linked lists
    size_t next_offset;          // Offset of the next pointer for linked lists
    size_t prev_offset;          // Offset of the prev pointer for doubly linked lists
} FieldMeta;

typedef void (*callback_t)(void);

void push_struct_to_lua(lua_State* L, void* struct_ptr, FieldMeta* meta);
int lua_to_enum(lua_State* L, int index, const char** enum_strings);
void lua_to_struct(lua_State* L, int index, void* struct_ptr, FieldMeta* meta);


#define FIELD_STRING(struct_type, field) { #field, TYPE_STRING, offsetof(struct_type, field), NULL, NULL, NULL, 0, 0 }
#define FIELD_INT(struct_type, field) { #field, TYPE_INT, offsetof(struct_type, field), NULL, NULL, NULL, 0, 0 }
#define FIELD_FLOAT(struct_type, field) { #field, TYPE_FLOAT, offsetof(struct_type, field), NULL, NULL, NULL, 0, 0 }
#define FIELD_BOOL(struct_type, field) { #field, TYPE_BOOL, offsetof(struct_type, field), NULL, NULL, NULL, 0, 0 }
#define FIELD_DOUBLE(struct_type, field) { #field, TYPE_DOUBLE, offsetof(struct_type, field), NULL, NULL, NULL, 0, 0 }
#define FIELD_CHAR(struct_type, field) { #field, TYPE_CHAR, offsetof(struct_type, field), NULL, NULL, NULL, 0, 0 }
#define FIELD_FUNC_PTR(struct_type, field) { #field, TYPE_FUNC_PTR, offsetof(struct_type, field), NULL, NULL, NULL, 0, 0 }
#define FIELD_ENUM(struct_type, field, enum_strings) { #field, TYPE_ENUM, offsetof(struct_type, field), NULL, enum_strings, NULL, 0, 0 }
#define FIELD_VOID_PTR(struct_type, field) { #field, TYPE_VOID_PTR, offsetof(struct_type, field), NULL, NULL, NULL, 0, 0 }
#define FIELD_STRUCT(struct_type, field, sub_meta) { #field, TYPE_STRUCT, offsetof(struct_type, field), sub_meta, NULL, NULL, 0, 0 }
#define FIELD_UINT(struct_type, field) { #field, TYPE_UINT, offsetof(struct_type, field), NULL, NULL, NULL, 0, 0 }
#define FIELD_UCHAR(struct_type, field) { #field, TYPE_UCHAR, offsetof(struct_type, field), NULL, NULL, NULL, 0, 0 }
#define FIELD_LINKED_LIST(struct_type, field, sub_meta, next_field, prev_field) { #field, TYPE_LINKED_LIST, offsetof(struct_type, field), NULL, NULL, sub_meta, offsetof(struct_type, next_field), offsetof(struct_type, prev_field) }

static FieldMeta Vector2Meta[] = {
    FIELD_FLOAT(Vector2, x),
    FIELD_FLOAT(Vector2, y),
    { NULL, 0, 0, NULL, NULL, NULL, 0, 0 }
};

static FieldMeta TagMeta[] = {
    FIELD_STRING(t_Tag, tag),
    FIELD_UINT(t_Tag, tag_id),
    { NULL, 0, 0, NULL, NULL, NULL, 0, 0 }
};

static FieldMeta Texture2DMeta[] = {
    FIELD_UINT(Texture2D, id),
    FIELD_INT(Texture2D, width),
    FIELD_INT(Texture2D, height),
    FIELD_INT(Texture2D, mipmaps),
    FIELD_INT(Texture2D, format),
    { NULL, 0, 0, NULL, NULL, NULL, 0, 0 }
};

static FieldMeta ColorMeta[] = {
    FIELD_INT(Color, r),
    FIELD_INT(Color, g),
    FIELD_INT(Color, b),
    FIELD_INT(Color, a),
    { NULL, 0, 0, NULL, NULL, NULL, 0, 0 }
};

static FieldMeta RectangleMeta[] = {
    FIELD_FLOAT(Rectangle, x),
    FIELD_FLOAT(Rectangle, y),
    FIELD_FLOAT(Rectangle, width),
    FIELD_FLOAT(Rectangle, height),
    { NULL, 0, 0, NULL, NULL, NULL, 0, 0 }
};

static const char* PriorityRankStrings[] = {
    "NONE",
    "LOW",
    "MEDIUM",
    "HIGH",    
    "CRITICAL",
    "PLAYER",
    NULL
};

static FieldMeta EntityMeta[] = {
    FIELD_STRING(t_Entity, entity_name),
    FIELD_UINT(t_Entity, num_tags),
    FIELD_INT(t_Entity, layer),

    FIELD_VOID_PTR(t_Entity, entity_data),

    FIELD_STRUCT(t_Entity, position, Vector2Meta),
    FIELD_STRUCT(t_Entity, velocity, Vector2Meta),
    FIELD_STRUCT(t_Entity, sprite_origin, Vector2Meta),

    FIELD_STRUCT(t_Entity, texture, Texture2DMeta),
    FIELD_STRUCT(t_Entity, source, RectangleMeta),
    FIELD_FLOAT(t_Entity, rotation),
    FIELD_FLOAT(t_Entity, scale),

    FIELD_BOOL(t_Entity, is_active),
    FIELD_FLOAT(t_Entity, health),
    FIELD_FLOAT(t_Entity, friction),
    FIELD_FLOAT(t_Entity, mass),
    FIELD_FLOAT(t_Entity, speed),

    FIELD_ENUM(t_Entity, priority_rank, PriorityRankStrings),
    
    FIELD_FLOAT(t_Entity, max_life_time),
    FIELD_FLOAT(t_Entity, current_life_time),

    { NULL, 0, 0, NULL, NULL, NULL, 0, 0 }
};

static const char* BulletOwnerStrings[] = {
    "BULLET_NONE",
    "BULLET_ENEMY",
    "BULLET_PLAYER",
    NULL
};

static FieldMeta BulletMeta[] = {
    FIELD_ENUM(t_BulletData, owner, BulletOwnerStrings),
    FIELD_FLOAT(t_BulletData, damage),
    FIELD_STRUCT(t_BulletData, owner_entity, EntityMeta),
    { NULL, 0, 0, NULL, NULL, NULL, 0, 0 }
};

#endif // BH_METATABLES_H