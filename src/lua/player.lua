package.path = package.path .. ';D:/bullet_hell/build/scripts/?.lua'

local bh = require("bh")

bh.register_function(bh.FunctionType.ON_START, function ()

end)

bh.register_function(bh.FunctionType.POST_RENDER, function()

end)

bh.register_function(bh.FunctionType.UPDATE, function (delta_time) 
    local enemy_bullets = bh.get_all_entities_with_tag("TEST_BULLET")
    for _, ent in pairs(enemy_bullets) do
        if bh.are_entities_colliding(ent, _G.player) then
            local b_data = bh.get_bullet_data(ent)
            if not b_data then return end

            bh.deal_damage(_G.player, b_data['damage'])

            bh.set_entity_active(ent, false)
        end
    end
end)
