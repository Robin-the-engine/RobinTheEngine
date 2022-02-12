
BlackBoard:set("bar", 50)
BlackBoard:set("SoundPosUpdated", 0)

function mul(a, b)
    Log.info("mul: " .. a*b)
    return TickResult.SUCCESS
end

function bark(times)
    times = times or 1
    for i=1,times do
        Log.info("bark!")
    end
    return TickResult.RUNNING
end

function OnUpdate()
--    t = bark(3)
--    Log.info("res: " .. t)
end

function onEvent(stimulus)
    sound = getStimulusAsSound(stimulus)
    newpos = sound:getPos()
    --print(newpos)
    BlackBoard:set("SoundPos", newpos)
    BlackBoard:set("SoundPosUpdated", 1)
    --print(BlackBoard:get("SoundPosUpdated"))
end

function onSound()
    topos = BlackBoard:get("SoundPos")
    --print(topos)
    selfAI:requestMove(topos)
    BlackBoard:set("SoundPosUpdated", 0)
    --print(BlackBoard:get("SoundPosUpdated"))
    return TickResult.SUCCESS
end

function abort()
    Log.info("\n\n\n\n\nACTION ABORTED\n\n\n\n\n")
end
