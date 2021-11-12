
print("hello from script")
angle = 0

function update()
    print("in update function")
    print(simulationSpeed)
    angle = timer:DeltaTime() * 200 * simulationSpeed;
    print("angle is " .. angle)
    ogre1:SetPosition(5 *  cos( angle), 5 * sin( angle) , 0);
    ogre1:SetLookAtPos(XMFLOAT3.new(0, 0, 0));
    ogre1:AdjustRotation(0, 2 * timer:DeltaTime(), 0);
    ball1:SetPosition(-5 *   cos(angle), 0, 5 *  sin( angle));
    lightPos = ball1:GetPositionFloat3();

    ball2:SetPosition(0, -5 *   cos( angle), -5 *  sin( angle));


    amogus:SetPosition(6 *   sin( angle), 6 *  cos( angle) , 0);
    amogus:SetLookAtPos(XMFLOAT3.new(0, 0, 0));

    amogus2:SetPosition(5.5 *  sin( angle),0, 5.5 *  cos( angle) );  
    amogus2:SetLookAtPos(XMFLOAT3.new(0, 0, 0));

    amogus1:SetRotation(XMFLOAT3.new(0,angle/3,0));
    ball:SetRotation(XMFLOAT3.new(angle/3, angle/5, 0));
    
    camPos = camera:GetPositionFloat3()
    ogre:SetLookAtPos(XMFLOAT3.new(camPos.x * -1, camPos.y * -1, camPos.z *-1));
end
