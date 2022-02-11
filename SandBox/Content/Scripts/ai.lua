
BlackBoard:set("bar", 50)

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

function onEvent(Pstimulus)
    sound = Pstimulus:getAsSound()
    BlackBoard:set("SoundPos", sound:pos)
    BlackBoard:set("SoundPosUpdated", 1)
end

function onSound()
    ai:requestMove(BlackBoard:get("SoundPos"))
    BlackBoard:set("SoundPosUpdated", 0)
end

function abort()
    Log.info("\n\n\n\n\nACTION ABORTED\n\n\n\n\n")
end
