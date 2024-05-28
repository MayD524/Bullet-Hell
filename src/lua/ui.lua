package.path = package.path .. ';D:/bullet_hell/build/scripts/?.lua'

local bh = require("bh")

bh.register_function(bh.FunctionType.POST_RENDER, function()
    local score = bh.get_player_score(_G.player)
    
    local player_data = bh.get_entity_data(_G.player)
    if not player_data then return end
    
    local position = string.format("Player pos: %f,%f", player_data['position']['x'], player_data['position']['y'])
    local pos = bh.create_vec2(10, 30)
    bh.draw_text(position, pos, 20, bh.Colors.GRAY)

    -- Create a 20 digit long string (left pad the score)
    local score_string = string.format("Score: \n%020d", math.floor(score * 100))
    local pos = bh.create_vec2(_G.screen['x']-250, 10)

    -- Draw the score at the top right corner
    bh.draw_text(score_string, pos, 20, bh.Colors.GRAY)

    local power = bh.get_player_power(_G.player)
    local health = player_data['health']
    if not power or not health then return end

    local power_string = string.format("Power: %d", math.floor(power))
    pos = bh.create_vec2(_G.screen['x']-250, 50)
    bh.draw_text(power_string, pos, 20, bh.Colors.GRAY)
    
    local health_string = string.format("Health: %d", math.floor(health))
    pos = bh.create_vec2(_G.screen['x']-250, 70)
    bh.draw_text(health_string, pos, 20, bh.Colors.GRAY)

end)