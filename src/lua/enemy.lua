package.path = package.path .. ';D:/bullet_hell/build/scripts/?.lua'

local bh = require("bh")

bh.register_function(bh.FunctionType.ON_START, function ()

end)

bh.register_function(bh.FunctionType.POST_RENDER, function()

end)

bh.register_function(bh.FunctionType.UPDATE, function (delta_time) 
    local base_enemies = bh.get_all_entities_with_tag("BASE_ENEMY")

    for _, ent in pairs(base_enemies) do

    end
end)

bh.register_function(bh.FunctionType.ON_DESTROY, function ()

end)