#include <player.h>

Player* create_player(t_EntitySystem* es, Vector2 start_position) {
    Player* p = create_entity(es, 
                                PLAYER_LOCAL_NAME, 
                                PLAYER_TEXTURE_PATH, 
                                start_position, 
                                VEC2_ZERO,
                                PLAYER, 
                                -1, 
                                1.0f, PLAYER_DEFAULT_HEALTH);
    if (p == NULL) {
        printf("Error creating player...\n");
        panic("Error creating player...");
    }

    add_tag(p, PLAYER_TAG);
    p->health = 100.0f;

    t_PlayerData* data = malloc(sizeof(t_PlayerData));
    data->bullet_cooldown = 0.0f;
    p->entity_data = data;

    p->update = update_player;
    p->on_destroy = on_player_destroy;
    p->speed = PLAYER_MOVE_SPEED;

    return p;
}

void on_player_destroy(Player* p) {
    printf("Player destroyed!\n");
}

void update_player(Player* p, float dt) {
    if (p->entity_data == NULL) {
        printf("Player has no entity data!\n");
        return;
    }

    t_PlayerData* data = (t_PlayerData*)p->entity_data;
    data->bullet_cooldown -= dt;

    if ((char)IsKeyDown(KEY_W)) {
        p->position.y -= p->speed * dt;
    }

    if ((char)IsKeyDown(KEY_A)) {
        p->position.x -= p->speed * dt;
    } 

    if ((char)IsKeyDown(KEY_S)) {
        p->position.y += p->speed * dt;
    }

    if ((char)IsKeyDown(KEY_D)) {
        p->position.x += p->speed * dt;
    }

    Texture2D texture = p->texture;

    // ensure player stays on screen
    if (p->position.x > SCREEN_WIDTH) {
        p->position.x = SCREEN_WIDTH;
    } else if (p->position.x - texture.width < 0) {
        p->position.x = texture.width;
    }

    if (p->position.y > SCREEN_HEIGHT) {
        p->position.y = SCREEN_HEIGHT;
    } else if (p->position.y - texture.height < 0) {
        p->position.y = texture.height;
    }

    if ((char)IsKeyDown(KEY_SPACE)) {
        // shoot bullet
        if (data->bullet_cooldown <= 0.0f) {

            Vector2 start_position = p->position;
            start_position.y += 25.0f;
            start_position.x += 25.0f;
            
            Bullet* b = create_bullet(PLAYER_PROJECTILE_PATH, 
                                        BULLET_PLAYER, 
                                        start_position, 
                                        PLAYER_PROJECTILE_SPEED, 
                                        100, 
                                        PLAYER_PROJECTILE_SCALE, 
                                        PLAYER_PROJECTILE_DAMAGE);
            data->bullet_cooldown = PLAYER_SHOOT_DELAY;
            b->max_life_time = PLAYER_PROJECTILE_LIFETIME;

            add_tag(b, PLAYER_PROJECTILE_TAG);
            print_tags(b);
        }
    }
    for (int i = 0; i < g_entity_system->num_entities; ++i) {
        t_Entity* e = get_entity(g_entity_system, i);
        if (e == NULL || !e->is_active || 
            has_tag(e, PLAYER_TAG) != -1 || has_tag(e, PLAYER_PROJECTILE_TAG)!= -1) {
            continue;
        }

        if (are_entities_colliding(p, e)) {
            // printf("Collision! between %s and %s\n", p->entity_name, e->entity_name);
        }
    }
}