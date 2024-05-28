#include <bh_enemy.h>

#pragma region Private helpers
void free_move_targets(Enemy* enemy) {
    t_EnemyData* data = (t_EnemyData*)enemy->entity_data;

    // gotta love allocations!
    t_EnemyMoveTarget* target = data->move_target;
    t_EnemyMoveTarget* next;

    while (target != NULL) {
        next = target->next;
        free(target);
        target = next;
    }
}

Vector2 Vector2FromAngle(float angle) {
    Vector2 result;
    result.x = cosf(angle);
    result.y = sinf(angle);
    return result;
}

// Function to move the entity based on direction, delta time, speed, and speed multiplier
void MoveEntity(Vector2 *entityPosition, Vector2 targetPosition, float deltaTime, float speed, float speedMultiplier) {
    // Get direction vector from angle
    Vector2 direction = { targetPosition.x - entityPosition->x, targetPosition.y - entityPosition->y };
    
    // Normalize direction vector
    direction = Vector2Normalize(direction);


    // Scale direction vector by speed, speed multiplier, and delta time
    // Scale direction vector by speed, speed multiplier, and delta time
    Vector2 scaledVector;
    scaledVector.x = direction.x * speed * speedMultiplier * deltaTime;
    scaledVector.y = direction.y * speed * speedMultiplier * deltaTime;

    // Update entity position
    entityPosition->x += scaledVector.x;
    entityPosition->y += scaledVector.y;
}

#pragma endregion


Enemy* create_enemy(char* texture_path, Vector2 start_position, float health, float size, float damage, float base_speed){
    Enemy* enemy = create_entity(g_entity_system, "ENEMY", texture_path, start_position, VEC2_ZERO, HIGH, -1, size, health);

    if (enemy == NULL) {
        panic("Error creating enemy entity");
    }

    add_tag(enemy, ENEMY_TAG);

    t_EnemyData* data = malloc(sizeof(t_EnemyData));
    data->damage = damage;
    data->shoot_cooldown = BASE_ENEMY_SHOOT_COOLDOWN;
    data->total_targets = 0;

    enemy->entity_data = data;

    enemy->on_collision = on_enemy_collision;
    enemy->update = update_enemy;
    enemy->on_destroy = destroy_enemy;

    return enemy;
}

void destroy_enemy(Enemy* enemy){
    // make sure that the move targets are freed
    free_move_targets(enemy);
    free(enemy->entity_data);
}

void update_enemy(Enemy* enemy, float delta_time){
    // no point updating an inactive enemy
    if (enemy->is_active == false) { return; } 
    t_Entity* player;
    
    // check for collisions
    for (int i = 0; i < g_entity_system->num_entities; ++i) {
        t_Entity* entity = get_entity(g_entity_system, i);
        if (entity == NULL || entity->entity_name == NULL || !entity->is_active) {
            continue;
        }

        if (entity->priority_rank == PLAYER) {
            player = entity;
        }

        if (has_tag(entity, PLAYER_PROJECTILE_TAG) != -1 && are_entities_colliding(entity, enemy)) {
            t_BulletData* data = (t_BulletData*)entity->entity_data;
            enemy->health -= data->damage;
            entity->is_active = false;
            printf("enemy health: %f\n", enemy->health);

            t_PlayerData* player_data = (t_PlayerData*)data->owner_entity->entity_data;
            player_data->player_score += PLAYER_SCORE_PER_KILL;

            return; // enemy dead don't bother doing anything
        }
    }

    t_EnemyData* data = (t_EnemyData*)enemy->entity_data;
    
    // decrease the cooldowns
    data->move_cooldown -= delta_time;
    data->shoot_cooldown -= delta_time;
    
    if (data->move_target!= NULL && data->move_cooldown <= 0) {
        
        MoveEntity(&enemy->position, data->move_target->position, delta_time, enemy->speed, data->move_target->speed_mult);
    
        if (Vector2Distance(enemy->position, data->move_target->position) <= 2) {
            t_EnemyMoveTarget* current_target = data->move_target;
            data->move_target = data->move_target->next;
            data->move_cooldown = current_target->wait_time;
            data->total_targets--;
            free(current_target); // clean up (no leaky leaky)
        }
    }
}

void add_enemy_move_target(Enemy* enemy, Vector2 position, float wait_time, float speed_mult) {
    t_EnemyData* data = (t_EnemyData*)enemy->entity_data;
    if (!data) {
        panic("Error adding enemy move target: enemy data is null");
        return;
    }
    
    t_EnemyMoveTarget* new_target = malloc(sizeof(t_EnemyMoveTarget));
    if (new_target == NULL) {
        panic("Error adding enemy move target: malloc failed");
        return;
    }
    new_target->position = position;
    new_target->wait_time = wait_time;
    new_target->speed_mult = speed_mult;
    new_target->next = NULL;

    if (data->move_target == NULL || data->total_targets == 0) {
        data->move_target = new_target;
    } else {
        t_EnemyMoveTarget* target = data->move_target;

        // find the end of the list
        while (target->next!= NULL) {
            target = target->next;
        }

        target->next = new_target;
    }
    data->total_targets++;
}

void on_enemy_collision(t_Entity* other, Enemy* enemy){
}
