local M = {}

-- Development flag
local is_development = false -- Set to true for development, false for production

--#region enum types
-- Functiont types
M.FunctionType = {
    UPDATE = "update",
    PRE_RENDER = "pre-render",
    POST_RENDER = "post-render",
    ON_DESTROY = "on-destroy",
    ON_START = "on-start",
}

M.PriorityRank = {
    NONE = 0,
    LOW = 1,
    MEDIUM = 2,
    HIGH = 3,
    CRITICAL = 4,
    PLAYER = 5,
}

M.PlayerClass = {
    DEFAULT = 0
}
--#endregion

--#region Constants
M.PLAYER_SCORE_PER_LEVEL      = 1000
M.PLAYER_SCORE_PER_SECOND     = 0.5
M.PLAYER_SCORE_PER_KILL       = 100
M.PLAYER_SCORE_PER_DEATH      = -100
M.POWER_MULTIPLIER            = 10
M.POWER_PER_LEVEL             = 100
M.DEFAULT_PLAYER_CLASS        = M.PlayerClass.DEFAULT
M.PROJECTILES_PER_POWER_LEVEL = 10
M.STARTING_SCORE              = 0
M.STARTING_LIVES              = 3
M.STARTING_POWER              = 1
M.ENEMY_TAG                   = "ENEMY"
M.PLAYER_TAG                  = "PLAYER"
M.PLAYER_LOCAL_NAME           = "LOCAL_PLAYER"
M.PLAYER_PROJECTILE_TAG       = "PLAYER_BULLET"
M.BULLET_TAG                  = "PROJECTILE"
M.DEFAULT_BULLET_LIFETIME     = 10

--#endregion

--#region dummy functions

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
local function dummy_set_velocity(entity, velocity) end
local function dummy_get_velocity(entity) end
local function dummy_get_entity_by_name(name) end
local function dummy_deal_damage(entity, damage) end
local function dummy_set_entity_active(entity, state) end
local function dummy_is_entity_active(entity) end
local function dummy_create_enemy(entity_texture_path, x, y, health, size, damage, base_speed) end
local function dummy_get_player_score(entity) end
local function dummy_set_player_score(entity, new_score) end
local function dummy_add_player_power(entity, amount) end
local function dummy_remove_player_power(entity, amount) end
local function dummy_get_player_power(entity) end
local function dummy_add_player_life(entity, amount) end
local function dummy_remove_player_life(entity, amount) end
local function dummy_get_player_lives(entity) end
local function dummy_get_player_class(entity) end
local function dummy_set_player_class(entity, new_class) end
local function dummy_heal_entity(entity, amount) end
local function dummy_get_entity_health(entity) end
local function dummy_set_entity_rotation(entity, rotation) end
local function dummy_get_entity_rotation(entity) end
local function dummy_get_entity_data(entity) end
local function dummy_create_bullet(texture, owner, start_pos, velocity, health, size, damage, rotation) end
local function dummy_get_bullet_data(entity) end
local function dummy_get_screen_size() end

-- local function dummy_draw_sprite(lua_State *L) end
-- local function dummy_draw_sprite_batch(lua_State *L) end
-- local function dummy_draw_texture_batch(lua_State *L) end

-- drawing functions
local function dummy_draw_text(text, pos, scale, color) end
local function dummy_draw_texture(texture, pos, rotation, scale, tint) end
local function dummy_draw_line(v_start, v_end, color, thickness) end
local function dummy_draw_rectangle(top_left, bottom_right, color, thickness, filled) end
local function dummy_draw_triangle(v1, v2, v3, color, thickness, filled) end
local function dummy_draw_circle(center, radius, color, thickness, filled) end

--#endregion

--#region Module functions
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
    M.get_player_score          = dummy_get_player_score
    M.set_player_score          = dummy_set_player_score
    M.add_player_power          = dummy_add_player_power
    M.add_player_power          = dummy_add_player_power
    M.remove_player_power       = dummy_remove_player_power
    M.get_player_power          = dummy_get_player_power
    M.add_player_life           = dummy_add_player_life
    M.remove_player_life        = dummy_remove_player_life
    M.get_player_lives          = dummy_get_player_lives
    M.get_player_class          = dummy_get_player_class
    M.set_player_class          = dummy_set_player_class
    M.heal_entity               = dummy_heal_entity
    M.get_entity_health         = dummy_get_entity_health
    M.draw_text                 = dummy_draw_text
    M.draw_texture              = dummy_draw_texture
    M.draw_line                 = dummy_draw_line
    M.draw_rectangle            = dummy_draw_rectangle
    M.draw_triangle             = dummy_draw_triangle
    M.draw_circle               = dummy_draw_circle
    M.set_velocity              = dummy_set_velocity
    M.get_velocity              = dummy_get_velocity
    M.set_entity_rotation       = dummy_set_entity_rotation
    M.get_entity_rotation       = dummy_get_entity_rotation
    M.get_entity_data           = dummy_get_entity_data
    M.create_bullet             = dummy_create_bullet
    M.get_bullet_data           = dummy_get_bullet_data
    M.get_screen_size           = dummy_get_screen_size
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
    M.get_player_score          = _G.get_player_score                    
    M.set_player_score          = _G.set_player_score                    
    M.add_player_power          = _G.add_player_power                    
    M.add_player_power          = _G.add_player_power                    
    M.remove_player_power       = _G.remove_player_power              
    M.get_player_power          = _G.get_player_power                    
    M.add_player_life           = _G.add_player_life                      
    M.remove_player_life        = _G.remove_player_life                
    M.get_player_lives          = _G.get_player_lives                    
    M.get_player_class          = _G.get_player_class                    
    M.set_player_class          = _G.set_player_class                    
    M.heal_entity               = _G.heal_entity                              
    M.get_entity_health         = _G.get_entity_health      
    M.draw_text                 = _G.draw_text
    M.draw_texture              = _G.draw_texture
    M.draw_line                 = _G.draw_line
    M.draw_rectangle            = _G.draw_rectangle
    M.draw_triangle             = _G.draw_triangle
    M.draw_circle               = _G.draw_circle            
    M.set_velocity              = _G.set_velocity
    M.get_velocity              = _G.get_velocity
    M.set_entity_rotation       = _G.set_entity_rotation
    M.get_entity_rotation       = _G.get_entity_rotation
    M.get_entity_data           = _G.get_entity_data
    M.create_bullet             = _G.create_bullet
    M.get_bullet_data           = _G.get_bullet_data
    M.get_screen_size           = _G.get_screen_size
end
--#endregion

--#region Entity controls
---Grant the player a single life
---@param entity userdata
M.grant_player_life = function(entity)
    M.add_player_life(entity, 1)
end

---Get the first entity with a tag
---@param tag string
---@return nil | userdata
M.get_entity_by_tag = function(tag)
    local num_entities = M.get_entity_count()
    for i = 0, num_entities - 1 do
        local entity = M.get_entity(i)
        if M.has_tag(entity, tag) and M.is_entity_active(entity) then
            return entity
        end
    end

    return nil
end

---Get an array of all entities with a specific tag
---@param tag string
---@return table
M.get_all_entities_with_tag = function(tag)
    local entity_list = {}
    local num_entities = M.get_entity_count()

    for i = 0, num_entities-1 do
        local entity = M.get_entity(i)
        if M.has_tag(entity, tag) and M.is_entity_active(entity) then
            table.insert(entity_list, entity)
        end
    end
    return entity_list
end


---Check if an enemy has a move taget
---@param entity userdata
---@return boolean
M.enemy_has_move_target = function (entity)
    if not M.is_entity_active(entity) then
        return false
    end
    local x, y, wait, speed_mult = M.get_enemy_move_target(entity)
    return x ~= -1 and y ~= -1 and wait ~= -1 and speed_mult ~= -1
end

--#endregion

--#region colors
---Create a color table
---@param r number
---@param g number
---@param b number
---@param a number|nil
---@return table
M.create_color = function (r, g, b, a)
    a = a or 255
    return {
        r = r,
        g = g,
        b = b,
        a = a
    }
end

M.Colors = {
    GRAY = M.create_color(130, 130, 130),
    RED = M.create_color(255, 0, 0),
    GREEN = M.create_color(0, 255, 0),
    BLUE = M.create_color(0, 0, 255),
    WHITE = M.create_color(255, 255, 255),
    BLACK = M.create_color(0, 0, 0),
}
--#endregion

--#region Vectors
---Create a vector 2
---@param x number
---@param y number
---@return table
M.create_vec2 = function(x, y)
    return {
        x = x, y = y
    }
end

M.VEC2_ZERO = M.create_vec2(0, 0)
M.VEC2_ONE  = M.create_vec2(1, 1)
--#endregion


--#region Drawing shapes
M.draw_rectangle_lines = function (top_left, bottom_right, color, thickness)
    M.draw_rectangle(top_left, bottom_right, color, thickness, false)
end

M.draw_rectangle_fill = function (top_left, bottom_right, color, thickness) 
    M.draw_rectangle(top_left, bottom_right, color, thickness, true)
end

M.draw_circle_lines = function (center, radius, color, thickness) 
    M.draw_circle(center, radius, color, thickness, true)
end

M.draw_circle_fill = function (center, radius, color, thickness) 
    M.draw_circle(center, radius, color, thickness, true)
end

M.draw_triangle_lines = function (v1, v2, v3, color, thickness)
    M.draw_triangle(v1, v2, v3, color, thickness, false)
end

M.draw_triangle_fill = function (v1, v2, v3, color, thickness) 
    M.draw_triangle(v1, v2, v3, color, thickness, true)
end
--#endregion


--#region useful functions
M.dump = function (o, indent)
    indent = indent or 0
    local indent_str = string.rep("  ", indent)
    if type(o) == 'table' then
        local s = '{\n'
        for k, v in pairs(o) do
            if type(k) ~= 'number' then k = '"'..k..'"' end
            s = s .. indent_str .. '  ['..k..'] = ' .. M.dump(v, indent + 1) .. ',\n'
        end
        return s .. indent_str .. '}'
    else
        return tostring(o)
    end
end

M.protect = function(table)
    return setmetatable({}, { 
        __index = table, 
        __newindex = function(table, key, value) 
            print("Error writing to '"..table.."' tried to modify '"..key.."' to '"..value.."' but was unable to due to this table being readonly...")
            error("Attempt to modify read-only table") 
        end, 
        __metatable = false 
    })
end

--#endregion

return M