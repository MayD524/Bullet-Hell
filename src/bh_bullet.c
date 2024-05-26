#include <bh_bullet.h>
#include <bh_entity.h>

t_BulletData* create_bullet_data(t_BulletOwner owner, float damage) {
    t_BulletData* data = malloc(sizeof(t_BulletData));
    data->owner = owner;
    data->damage = damage;
    return data;
}

Bullet* create_bullet(char* texture, t_BulletOwner owner, Vector2 start_position, Vector2 velocity, float health, float size, float damage){
    char name[16];
    sprintf_s(name, sizeof(name), "bullet_%d_%d\0", (int)owner, GetRandomValue(0, 100000));

    PriorityRank priority = NONE;

    switch (owner) {
        case BULLET_PLAYER:
            priority = HIGH;
            break;
        case BULLET_ENEMY:
            priority = MEDIUM;
            break;
        default:
            priority = LOW;
            break;
    }

    Bullet* bullet = create_entity(g_entity_system, name, texture, start_position, velocity, priority, DEFAULT_BULLET_LIFETIME, size, health); 
    strcpy_s(bullet->entity_name, sizeof(bullet->entity_name), name);
    bullet->entity_data = create_bullet_data(owner, damage);
    add_tag(bullet, BULLET_TAG);
    
    return bullet;
    
}

void destroy_bullet(Bullet* bullet) {
    free(bullet->entity_data);
    destroy_entity(bullet);
}