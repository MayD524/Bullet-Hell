#include <bh_entity.h>

#pragma region Shape drawing

ShapeType getShapeType(const char *shapeStr) {
    if (strcmp(shapeStr, "RECT") == 0) return RECTANGLE;
    if (strcmp(shapeStr, "CIRC") == 0) return CIRCLE;
    if (strcmp(shapeStr, "TRI") == 0) return TRIANGLE;
    return UNKNOWN;
}

Shape parseShapeString(const char *str) {
    Shape shape;
    char shapeStr[5];
    char filledStr[6];
    int r, g, b, a;

    sscanf(str, "SHAPE:%4[^:]:%f,%f:%f,%f:%f,%f:%d,%d,%d,%d:%d:%5s",
           shapeStr, &shape.position1.x, &shape.position1.y,
           &shape.position2.x, &shape.position2.y, &shape.position3.x,
           &shape.position3.y, &r, &g, &b, &a, &shape.thickness, filledStr);

    // Assign shape type
    shape.type = getShapeType(shapeStr);

    // Assign color
    shape.color = (Color){ r, g, b, a };

    // Handle boolean parsing
    shape.filled = (strcmp(filledStr, "true") == 0);

    // Default values for optional parameters based on shape
    if (shape.type == RECTANGLE) {
        shape.position3 = (Vector2){0, 0};
    } else if (shape.type == CIRCLE) {
        shape.position2 = (Vector2){shape.position2.x, 0};
        shape.position3 = (Vector2){0, 0};
    }

    return shape;
}

void printShape(const Shape *shape) {
    const char *shapeTypeStr = (shape->type == RECTANGLE) ? "RECTANGLE" :
                                (shape->type == CIRCLE) ? "CIRCLE" :
                                (shape->type == TRIANGLE) ? "TRIANGLE" : "UNKNOWN";

    printf("--------------------------\n");
    printf("Shape: %s\n", shapeTypeStr);
    printf("Position 1: (%.2f, %.2f)\n", shape->position1.x, shape->position1.y);
    if (shape->type == RECTANGLE || shape->type == TRIANGLE) {
        printf("Position 2: (%.2f, %.2f)\n", shape->position2.x, shape->position2.y);
    } else {
        printf("Radius %.2f\n", shape->position2.x);
    }
    if (shape->type == TRIANGLE) {
        printf("Position 3: (%.2f, %.2f)\n", shape->position3.x, shape->position3.y);
    }
    printf("Color: (%d, %d, %d, %d)\n", shape->color.r, shape->color.g, shape->color.b, shape->color.a);
    printf("Thickness: %d\n", shape->thickness);
    printf("Filled: %s\n", shape->filled ? "true" : "false");
    printf("--------------------------\n");
}

void drawShape(t_Entity* ent, Shape *shape) {

    switch (shape->type) {
        case RECTANGLE: {
            Vector2 pos1 = { ent->position.x + shape->position1.x * ent->scale, ent->position.y + shape->position1.y * ent->scale };
            if (shape->filled) {
                DrawRectangle(pos1.x, pos1.y, (shape->position2.x - shape->position1.x) * ent->scale, (shape->position2.y - shape->position1.y) * ent->scale, shape->color);
            } else {
                DrawRectangleLinesEx((Rectangle){ pos1.x, pos1.y, (shape->position2.x - shape->position1.x) * ent->scale, (shape->position2.y - shape->position1.y) * ent->scale },
                                        shape->thickness, shape->color);
            }
            break;
        }
        case CIRCLE: {
            if (shape->filled) {
                DrawCircleV(ent->position, shape->position2.x * ent->scale, shape->color);
            } else {
                DrawCircleLines(ent->position.x, ent->position.y, shape->position2.x * ent->scale, shape->color);
            }
            break;
        }
        case TRIANGLE: {
            Vector2 pos1 = { ent->position.x + shape->position1.x * ent->scale, ent->position.y + shape->position1.y * ent->scale };
            Vector2 pos2 = { ent->position.x + shape->position2.x * ent->scale, ent->position.y + shape->position2.y * ent->scale };
            Vector2 pos3 = { ent->position.x + shape->position3.x * ent->scale, ent->position.y + shape->position3.y * ent->scale };
            if (shape->filled) {
                DrawTriangle(pos1, pos2, pos3, shape->color);
            } else {
                DrawTriangleLines(pos1, pos2, pos3, shape->color);
            }
            break;
        }
        default:
            break;
    }
}
#pragma endregion


int are_floats_equal(float a, float b) {
    return fabs(a - b) < FLT_EPSILON;
}

#pragma region Public methods for entity system
t_EntitySystem* create_entity_system() {
    t_EntitySystem* entity_system = malloc(sizeof(t_EntitySystem));

    entity_system->num_entities = 0;
    entity_system->num_textures = 0;

    return entity_system;
}

int next_free_entity_slot(const t_EntitySystem* entity_system, const PriorityRank* priority_rank) {
    int start = entity_system->num_entities-1;
    int i = entity_system->num_entities;
    while (i != start) {
        if (i > MAX_ENTITIES-1) {
            i = 0;
        }

        const t_Entity* entity = &entity_system->entities[i];
        if (entity == NULL || entity->priority_rank < *priority_rank || !entity->is_active) {
            return i;
        }

        ++i;
    }

    return -1;
}

#pragma region Update
void update(t_EntitySystem* entity_system) {
    float delta_time = GetFrameTime();
    for (int i = 0; i < entity_system->num_entities; ++i) {
        t_Entity* e = get_entity(entity_system, i);
        if (e == NULL || e->entity_name == NULL) { continue; }

        e->current_life_time += delta_time;
        if (e->priority_rank != PLAYER && (!e->is_active || (e->current_life_time > e->max_life_time && e->max_life_time >= 0))) {
            e->current_life_time = 0;
            e->is_active = false;
            if (e->update != NULL) {
                e->update(e,delta_time);
            }
            continue;
        }

        if (e->health <= 0) {
            printf("%s has died\n", e->entity_name);
            e->is_active = false;
            continue;
        }

        if (e->update != NULL) {
            e->update(e,delta_time);
        }
        
        if (!are_floats_equal(e->velocity.x, 0.0f) || !are_floats_equal(e->velocity.y, 0.0f)) {
            e->position = Vector2Add(e->position, Vector2Scale(e->velocity, delta_time));

            e->velocity.x -= (e->friction * e->mass) * delta_time;
            e->velocity.y -= (e->friction * e->mass) * delta_time;
        }
    }
}
#pragma endregion

#pragma region Rendering
void render(t_EntitySystem* entity_system) {
    char FPS_string[100];
    sprintf_s(FPS_string, 100, "FPS: %d", GetFPS());
    DrawText(FPS_string, 10, 10, 20, GRAY);

    // char MEM_used_string[100];
    // sprintf_s(MEM_used_string, 100, "MEM: %d kb", get_memory_usage());
    // DrawText(MEM_used_string, 10, 40, 20, GRAY);

    for (int i = 0; i < entity_system->num_entities; ++i){

        t_Entity* e = get_entity(entity_system, i);
        // don't attempt to draw inactive entities, or entities that don't exist. 
        if (e->entity_name == NULL || !e->is_active || has_tag(e, "HIDDEN") != -1) { continue; } 
        
        if (e->texture.height != 0 && e->texture.width != 0) {
            Texture2D texture = e->texture;
            
            Rectangle destRec = { e->position.x, e->position.y, texture.width * e->scale, texture.height* e->scale };

            DrawTexturePro(texture, e->source, destRec, e->sprite_origin, 0, WHITE);
        } else {
            drawShape(e, &e->shape);
        }
    }
}
#pragma endregion

#pragma endregion

#ifdef WIN32
#include <io.h>
#define F_OK 0
#define access _access
#endif

#pragma region Methods for base entity
t_Entity* create_entity(t_EntitySystem* es, char* entity_name, char* texturePath, Vector2 start_position, Vector2 start_velocity, PriorityRank priority_rank, float max_life_time, float scale, float health) {
    int slot = next_free_entity_slot(es, &priority_rank);

    if (slot == -1) {
        printf("Failed to allocate memory for new entity. \n");
        printf("This entity will not be created. \n");
        return NULL;
    }
    
    t_Entity entity;

    entity.position = start_position;
    entity.velocity = start_velocity;
    entity.priority_rank = priority_rank;
    entity.max_life_time = max_life_time;
    entity.entity_name = entity_name;
    entity.is_active = true;
    entity.max_life_time = max_life_time;
    entity.current_life_time = 0.0f;
    entity.mass = 1.0f;
    entity.friction = 0.0f;
    entity.speed = 1.0f;
    entity.update = NULL;
    entity.entity_data = NULL;
    entity.health = health;

    entity.scale = scale;

    if (access(texturePath, F_OK) == 0) {
        int textureid = load_texture(es, texturePath);
        Texture2D texture = get_texture(es, textureid);
        entity.texture = texture;

        int frameWidth = texture.width;
        int frameHeight = texture.height;

        Vector2 sp = { (float)frameWidth, (float)frameHeight };
        entity.sprite_origin = sp;

        Rectangle source = { 0.0f, 0.0f, (float)frameWidth, (float)frameHeight };
        entity.source = source;
        entity.shape = (Shape){ 0 };
    } else {
        entity.texture = (Texture){ 0 };
        entity.source = (Rectangle){ 0 };
        entity.sprite_origin = VEC2_ZERO;
    }
    
    if (strncmp("SHAPE", texturePath, strlen(texturePath)))
        entity.shape = parseShapeString(texturePath);

    t_Tag* root = malloc(sizeof(t_Tag));
    root->tag_id = 0;
    root->next = NULL;
    root->prev = NULL;
    root->tag = "ENTITY_ROOT";

    entity.root_tag = root;
    entity.num_tags = 0;



    es->entities[slot] = entity;
    es->num_entities++;
    return &es->entities[slot];
}

Rectangle get_entity_source(const t_Entity* entity) {
    return entity->source;
}

t_Entity* get_entity(t_EntitySystem* es, int index) {
    return &es->entities[index];
}

t_Entity* get_entity_by_name(t_EntitySystem* es, char* entity_name) {\
    for (int i = 0; i < es->num_entities; ++i) {
        t_Entity* e = get_entity(es, i);
        if (e == NULL || e->entity_name == NULL) { continue; }
        if (strcmp(e->entity_name, entity_name) == 0) {
            return e;
        }
    }
    return NULL;
}

bool are_entities_colliding(const t_Entity* e1, const t_Entity* e2) {
    // TODO: make the hitboxes smaller
    if (e1 == NULL || e2 == NULL || !e1->is_active ||!e2->is_active) {
        //fprintf(stderr, "One of the entities is NULL or inactive\n");
        return false;
    }

    Rectangle rect1 = {
        e1->position.x - e1->sprite_origin.x, 
        e1->position.y - e1->sprite_origin.y, 
        e1->texture.width * e1->scale, 
        e1->texture.height * e1->scale
    };

    Rectangle rect2 = {
        e2->position.x - e2->sprite_origin.x, 
        e2->position.y - e2->sprite_origin.y, 
        e2->texture.width * e2->scale, 
        e2->texture.height * e2->scale
    };

    DrawRectangleLinesEx(rect1, 2, RED);
    DrawRectangleLinesEx(rect2, 2, RED);


    // Check if the rectangles collide
    return CheckCollisionRecs(rect1, rect2);

}

void print_movement(const t_Entity* entity) {
    printf("movement info %s: \n", entity->entity_name);
    printf("\tPosition: %f, %f \n", entity->position.x, entity->position.y);
    printf("\tVelocity: %f, %f \n", entity->velocity.x, entity->velocity.y);
    printf("\tMass: %f \n", entity->mass);
    printf("\tFriction: %f \n", entity->friction);
}

#pragma region Entity tag system
void print_tags(const t_Entity* entity) {
    const char** tags = get_tags(entity);
    if (tags == NULL) {
        fprintf(stderr, "ERROR: print_tags: Failed to get tags\n");
        return;
    }
    for (int i = 0; i < entity->num_tags; ++i) {
        if (tags[i] == NULL) { return; }
        printf("TAG: %s \n", tags[i]);
    }

    free(tags);
}

const char** get_tags(const t_Entity* entity) {
    if (!entity || !entity->root_tag) {
        fprintf(stderr, "Entity or root_tag is NULL\n");
        return NULL;
    }

    t_Tag* current = entity->root_tag->next;
    if (!current) {
        fprintf(stderr, "root_tag->next is NULL\n");
        return NULL;
    }
    printf("num_tags: %d \n", entity->num_tags);
    printf("memory allocated for %lld bytes\n", sizeof(char*) * entity->num_tags+1);
    const char** tags = malloc(sizeof(char*) * entity->num_tags+1);
    if (tags == NULL) {
        fprintf(stderr, "ERROR: get_tags: Memory allocation failed\n");
        return NULL;
    }

    int i = 0;
    while (current != entity->root_tag) {
        if (i >= entity->num_tags) {
            fprintf(stderr, "Too many tags for entity %s\n", entity->entity_name);
            break;
        }

        if (!current) {
            fprintf(stderr, "Current tag pointer is NULL\n");
            free(tags);
            return NULL;
        }

        if (!current->tag) {
            fprintf(stderr, "Current tag is NULL\n");
            free(tags);
            return NULL;
        }

        tags[i] = current->tag;
        current = current->next;
        ++i;
    }

    return tags;
}

void add_tag(t_Entity* entity, const char* tag) {
    t_Tag* new_tag = malloc(sizeof(t_Tag));
    new_tag->tag_id = entity->num_tags+1;
    new_tag->tag = tag;
    
    new_tag->next = entity->root_tag;

    if (entity->root_tag->prev != NULL) {
        // whatever comes after the root tag will now point to the new tag
        new_tag->prev = entity->root_tag->prev;
        entity->root_tag->prev->next = new_tag;
        entity->root_tag->prev = new_tag;

    } else { // there is no tag before the root
        new_tag->prev = entity->root_tag;
        entity->root_tag->next = new_tag;
    }

    // the new tag will now be the new end (circular linked list)
    entity->root_tag->prev = new_tag;
    entity->num_tags++;
}

int has_tag(const t_Entity* entity, const char* tag) {
    // entity has no tags
    if (entity->root_tag->next == NULL) {
        return -1;
    }

    t_Tag* current = entity->root_tag->next;
    while (current != entity->root_tag) {
        if (strcmp(current->tag, tag) == 0) {
            return current->tag_id;
        }
        current = current->next;
    }
    return -1;

}

void remove_tag_at(t_Entity* entity, int index) {
    t_Tag* current = entity->root_tag->next;
    int i = 0;
    while (current != entity->root_tag) {
        if (i == index) {
            current->prev->next = current->next;
            current->next->prev = current->prev;
            free(current);
            entity->num_tags--;
            return;
        }
        current = current->next;
        ++i;
    }
    // we didn't find the tag to remove
}

void remove_tag(t_Entity* entity, const char* tag) {
    int index = has_tag(entity, tag);
    if (index == -1) {
        return;
    }

    remove_tag_at(entity, index);
}

void remove_all_tags(t_Entity* entity) {
    t_Tag* current = entity->root_tag->next;
    while (current != entity->root_tag) {
        t_Tag* next = current->next;
        free(current);
        current = next;
    }
    entity->root_tag->next = NULL;
    entity->root_tag->prev = NULL;
    entity->num_tags = 0;
}
#pragma endregion // Entity tag system
#pragma endregion // functions for base entity

#pragma region Texture handling

Texture2D get_texture(t_EntitySystem* es, int id) {
    return es->textures[id].texture;
}

// Wait why don't we just return the t_Texture?
// Not sure why we don't lol
// oh well it works for now
int load_texture(t_EntitySystem* es, char* texture_path) {
    if (es->num_textures >= MAX_TEXTURES) {
        fprintf(stderr, "Too many textures\n");
        return -1;
    }
    
    for (int i = 0; i < es->num_textures; ++i) {
        if (strcmp(es->textures[i].texture_name, texture_path) == 0) {
            return i;
        }
    }

    Texture2D texture = LoadTexture(texture_path);
    t_Texture res = { .texture_name = texture_path, .texture = texture };

    es->textures[es->num_textures] = res;
    es->num_textures++;
    return es->num_textures - 1;
}

#pragma endregion

#pragma region Deconstruction
void destroy_entity(t_Entity* entity) {
    printf("Destroying entity %s \n", entity->entity_name);
    entity->is_active = false;
    
    if (entity->on_destroy != NULL) {
        entity->on_destroy(entity);
    }

    if (entity->entity_data != NULL) {
        free(entity->entity_data);
    }
}

void destroy_entity_system(t_EntitySystem* es) {
    for (int i = 0; i < es->num_entities; ++i) {
        destroy_entity(&es->entities[i]);
    }

    free(es);
}

#pragma endregion