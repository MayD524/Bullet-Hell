-- example lua script for bullet hell

package.path = package.path .. ';D:/bullet_hell/build/scripts/?.lua'

local bindings = require("bh")

bindings.register_function(bindings.FunctionType.ON_START, function ()
    local entity = bindings.create_enemy("D:\\bullet_hell\\resources\\log_t.png", 100, 100, 10, 1, 10, 10)
    bindings.add_tag(entity, "hello")
    bindings.print_tags(entity)

    bindings.set_enemy_move_target(entity, 200, 200, 5, 50)
    -- bindings.set_enemy_move_target(entity, 500, 500, 5, 50)
end)

bindings.register_function(bindings.FunctionType.POST_RENDER, function()
    local center = bindings.create_vec2(100, 100)
    bindings.draw_circle(center, 10, bindings.Colors.RED, 10, true)
end)

bindings.register_function(bindings.FunctionType.UPDATE, function(delta_time)
    local entity = bindings.get_entity_by_tag("hello")

    if entity == nil then
        return
    end

    -- if bindings.enemy_has_move_target(entity) then
    --     print("has move target...")
    -- else
    --     print("No move target")
    -- end

    -- if entity == nil then
    --     print("No entity with tag hello")
    -- end

    -- print(entity)

    
    -- local e = bindings.get_entity_by_name("test")    
    -- if bindings.is_entity_active(e) == false then
    --     return
    -- end
    
    -- local x, y = bindings.get_position(e)
    
    -- x = x + (10 * delta_time)
    -- y = y + (10 * delta_time)

    -- bindings.set_position(e, x, y)

    -- local entity_count = bindings.get_entity_count()
    -- local index = 0

    -- while index < entity_count do
    --     local new_entity = bindings.get_entity(index)
    --     if bindings.has_tag(new_entity, "PROJECTILE") then
    --         if (bindings.are_entities_colliding(e, new_entity)) then
    --             bindings.deal_damage(e, 10)
    --         end
    --     end
    --     index = index + 1
    -- end

end)