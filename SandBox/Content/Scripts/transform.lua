
angle = 0

function OnUpdate()
    Log.trace("simulation sped is: " .. simulationSpeed)
    dt = timer:DeltaTime()
    angle = angle + dt * 200 * simulationSpeed;
    Log.trace("angle is: " .. angle)
    cosangle = math.cos( angle)
    sinangle = math.sin( angle)
    Log.trace("cos angle is " .. cosangle)
    Log.trace("sin angle is " .. sinangle)

    ogre1:SetPosition(5 *  cosangle, 5 * sinangle , 0);
    ogre1:SetLookAtPos(XMFLOAT3.new(0, 0, 0));
    ogre1:AdjustRotation(0, 2 * dt, 0);
    ball1:SetPosition(-5 * cosangle, 0, 5 *  sinangle);
    lightPos = ball1:GetPositionFloat3();

    ball2:SetPosition(0, -5 * cosangle, -5 * sinangle);

    amogus:SetPosition(6 * sinangle, 6 * cosangle , 0);
    amogus:SetLookAtPos(XMFLOAT3.new(0, 0, 0));

    amogus2:SetPosition(5.5 * sinangle, 0, 5.5 * cosangle);
    amogus2:SetLookAtPos(XMFLOAT3.new(0, 0, 0));

    amogus1:SetRotation(XMFLOAT3.new(0,angle/3,0));
    ball:SetRotation(XMFLOAT3.new(angle/3, angle/5, 0));
    
    camPos = camera:GetPositionFloat3()
    ogre:SetLookAtPos(XMFLOAT3.new(camPos.x * -1, camPos.y * -1, camPos.z *-1));
end
