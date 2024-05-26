#include <stdio.h>
#include <raylib.h>

#include <bh_entity.h>
#include <bh_enemy.h>
#include <player.h>
#include <util.h>

#include <dirent.h> 
#include <stdio.h> 

#include <bh_lua.h>

t_EntitySystem* g_entity_system;
LuaContext* g_lua_context;

#define LUA_DIR "./build/scripts/"
#define FILE_SEARCH_SIZE 10

typedef struct {
    char** files;
    int file_count;
} t_FileSearch;

t_FileSearch GetFilesInDirectory(const char* path) {
    t_FileSearch file_search;
    char** files = malloc(sizeof(char*) * FILE_SEARCH_SIZE);
    int max = FILE_SEARCH_SIZE;
    int file_count = 0;
    DIR *dir = opendir(path);
    struct dirent *de;

    while((de = readdir(dir))!= NULL) {
        if (file_count >= max) {
            max += FILE_SEARCH_SIZE;
            files = realloc(files, sizeof(char*) * max);
        }
        if (strcmp(de->d_name, ".")!= 0 && strcmp(de->d_name, "..")!= 0 && strcmp(de->d_name, "bh.lua")!= 0) {
            files[file_count] = malloc(strlen(path) + strlen(de->d_name) + 1);
            strcpy(files[file_count], path);
            strcat(files[file_count], de->d_name);
            file_count++;
        }
    }
    closedir(dir);
    file_search.files = files;
    file_search.file_count = file_count;
    return file_search;
}

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, GAME_TITLE);
    SetTargetFPS(TARGET_FPS);

    g_entity_system = create_entity_system();
    g_lua_context = init_lua();

    // preload textures
    load_texture(g_entity_system, PLAYER_TEXTURE_PATH);
    load_texture(g_entity_system, ENEMY_TEXTURE_PATH);
    load_texture(g_entity_system, PLAYER_PROJECTILE_PATH);

    Player* player = create_player(g_entity_system, PLAYER_START_POSITION);
    printf("Player: %p\n", player);
    // print_tags(player);

    printf("func_refs: %p\n", g_lua_context->func_refs);

    t_FileSearch file_search = GetFilesInDirectory(LUA_DIR);
    for (int i = 0; i < file_search.file_count; i++) {
        printf("loading %s...\n", file_search.files[i]);
        execute_lua_file(g_lua_context->lua_state, file_search.files[i]);
    }

    // Enemy* enemy = create_enemy(ENEMY_TEXTURE_PATH, ENEMY_START_POSITION, BASE_ENEMY_HEALTH, BASE_ENEMY_SIZE, BASE_ENEMY_DAMAGE, BASE_ENEMY_SPEED);
    
    // add_enemy_move_target(enemy, Vector2AddValue(ENEMY_START_POSITION, 50.0f), 0 , 50.0f);
    // add_enemy_move_target(enemy, Vector2AddValue(enemy->position     , -100.0f), 0, 50.0f);
    // add_enemy_move_target(enemy, Vector2AddValue(enemy->position     , 150.0f), 0, 50.0f);

    while (!WindowShouldClose()) {
        
        BeginDrawing();
        ClearBackground(RAYWHITE);

        call_functions(g_lua_context, PRE_RENDER);
        render(g_entity_system);
        call_functions(g_lua_context, POST_RENDER);

        update(g_entity_system);
        call_functions(g_lua_context, UPDATE);

        // DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
        EndDrawing();
    }

    call_functions(g_lua_context, ON_DESTROY);

    close_lua(g_lua_context);
    CloseWindow();
    destroy_entity_system(g_entity_system);

    return 0;
}