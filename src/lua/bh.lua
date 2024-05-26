local M = {}

-- Development flag
local is_development = false -- Set to true for development, false for production

-- Functiont types
M.FunctionType = {
    UPDATE = "update",
    PRE_RENDER = "pre-render",
    POST_RENDER = "post-render",
    ON_AWAKE = "on-awake",
    ON_DESTROY = "on-destroy",
    ON_START = "on-start"
}

-- Dummy functions for development (intellisense support)
local function dummy_register_function(eventType, func) end
local function dummy_get_entity_count() end
local function dummy_create_entity(entityName, texturePath, x, y, vx, vy, priority, maxLifeTime, scale, health) end
local function dummy_get_entity(index) end
local function dummy_print_movement(entity) end
local function dummy_are_entities_colliding(entityA, entityB) end
local function dummy_print_tags(entity) end
local function dummy_add_tag(entity, tag) end
local function dummy_get_tags(entity) end
local function dummy_remove_tag_at(entity, index) end
local function dummy_remove_tag(entity, tag) end
local function dummy_has_tag(entity, tag) end
local function dummy_remove_all_tags(entity) end
local function dummy_load_texture(texturePath) end
local function dummy_get_texture(textureID) end
local function dummy_set_enemy_data(entity, moveCooldown, shootCooldown, damage) end
local function dummy_get_enemy_data(entity) end
local function dummy_set_enemy_move_target(entity, x, y, waitTime, speedMult) end
local function dummy_get_enemy_move_target(entity) end
local function dummy_get_position(entity) end
local function dummy_set_position(entity, x, y) end
local function dummy_get_entity_by_name(name) end
local function dummy_deal_damage(entity, damage) end
local function dummy_set_entity_active(entity, state) end
local function dummy_is_entity_active(entity) end
local function dummy_create_enemy(entity_texture_path, x, y, health, size, damage, base_speed) end

if is_development then
    M.get_entity_count          = dummy_get_entity_count
    M.register_function         = dummy_register_function
    M.create_entity             = dummy_create_entity
    M.get_entity                = dummy_get_entity
    M.print_movement            = dummy_print_movement
    M.are_entities_colliding    = dummy_are_entities_colliding
    M.print_tags                = dummy_print_tags
    M.add_tag                   = dummy_add_tag
    M.get_tags                  = dummy_get_tags
    M.remove_tag_at             = dummy_remove_tag_at
    M.remove_tag                = dummy_remove_tag
    M.has_tag                   = dummy_has_tag
    M.remove_all_tags           = dummy_remove_all_tags
    M.load_texture              = dummy_load_texture
    M.get_texture               = dummy_get_texture
    M.set_enemy_data            = dummy_set_enemy_data
    M.get_enemy_data            = dummy_get_enemy_data
    M.set_enemy_move_target     = dummy_set_enemy_move_target
    M.get_enemy_move_target     = dummy_get_enemy_move_target
    M.get_position              = dummy_get_position
    M.set_position              = dummy_set_position
    M.get_entity_by_name        = dummy_get_entity_by_name
    M.deal_damage               = dummy_deal_damage
    M.set_entity_active         = dummy_set_entity_active
    M.is_entity_active          = dummy_is_entity_active
    M.create_enemy              = dummy_create_enemy
else
    -- Reference the global environment (actual functions registered by C)
    M.register_function         = _G.register_function
    M.create_entity             = _G.create_entity
    M.get_entity                = _G.get_entity
    M.print_movement            = _G.print_movement
    M.are_entities_colliding    = _G.are_entities_colliding
    M.print_tags                = _G.print_tags
    M.add_tag                   = _G.add_tag
    M.get_tags                  = _G.get_tags
    M.remove_tag_at             = _G.remove_tag_at
    M.remove_tag                = _G.remove_tag
    M.has_tag                   = _G.has_tag
    M.remove_all_tags           = _G.remove_all_tags
    M.load_texture              = _G.load_texture
    M.get_texture               = _G.get_texture
    M.set_enemy_data            = _G.set_enemy_data
    M.get_enemy_data            = _G.get_enemy_data
    M.set_enemy_move_target     = _G.set_enemy_move_target
    M.get_enemy_move_target     = _G.get_enemy_move_target
    M.get_entity_count          = _G.get_entity_count
    M.get_position              = _G.get_position
    M.set_position              = _G.set_position
    M.get_entity_by_name        = _G.get_entity_by_name
    M.deal_damage               = _G.deal_damage
    M.set_entity_active         = _G.set_entity_active
    M.is_entity_active          = _G.is_entity_active
    M.create_enemy              = _G.create_enemy
end

return M