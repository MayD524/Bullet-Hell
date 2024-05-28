package.path = package.path .. ';D:/bullet_hell/build/scripts/?.lua'

local bh = require("bh")

bh.register_function(bh.FunctionType.ON_START, function ()

end)

bh.register_function(bh.FunctionType.POST_RENDER, function()

end)

bh.register_function(bh.FunctionType.UPDATE, function (delta_time) 
    local base_enemies = bh.get_all_entities_with_tag("BASE_ENEMY")

    if (bh.tablelen(base_enemies) == 0 and math.fmod(_G.timer, 10) < 0.02) then
        print("new enemy")
        local entity = bh.create_enemy("D:\\bullet_hell\\resources\\log_t.png", 100, 100, 10, 1, 10, 10)
        bh.add_tag(entity, "BASE_ENEMY")
        bh.print_tags(entity)

        bh.set_enemy_move_target(entity, math.random(0, _G.screen['x']), 
                                math.random(0, _G.screen['y']), 5, 250)
        return
    elseif bh.tablelen(base_enemies) == 0 then
        return -- no enemies to loop through
    end
    
    for _, ent in pairs(base_enemies) do
        local enemy_data = bh.get_enemy_data(ent)
        local ent_data   = bh.get_entity_data(ent)

        if enemy_data ~= nil and enemy_data['shoot_cooldown'] <= 0 and ent_data ~=nil and _G.ttest == 0 then
            local new_data = bh.create_enemy_data(  enemy_data['total_targets'], 
                                                    enemy_data['move_cooldown'], 
                                                    0.5, 
                                                    enemy_data['damage'])
            bh.set_enemy_data(ent, new_data)

            local p_data = bh.get_entity_data(_G.player)
            if not p_data then return end

            local start_top = bh.create_vec2(ent_data['position']['x'] - (ent_data['texture']['width'] / 2), ent_data['position']['y'] - (ent_data['texture']['height'] / 2))
            local start_btm = bh.create_vec2(10, 0)
            local speed = bh.create_vec2(1,1)

            local true_pos = bh.create_vec2(p_data['position']['x'] - (p_data['texture']['width'] / 2), p_data['position']['y'] - (p_data['texture']['height'] / 2))

            local dir_vec = bh.get_direction_vector(true_pos, start_top, speed)

            local texture = bh.create_texture_str(bh.ShapeType.CIRCLE, start_top, start_btm, bh.VEC2_ZERO, bh.Colors.RED, 10, true)
            local b = bh.create_bullet(texture, bh.BulletOwner.BULLET_ENEMY, start_top, dir_vec, 10, 1, 10, 0)
            bh.add_tag(b, "TEST_BULLET");
            

        end
    end
end)

bh.register_function(bh.FunctionType.ON_DESTROY, function ()

end)