x, y, z = ...
move = function (transform, time)
	x = math.sin(time)
	y = math.cos(time)
	z = math.sin(time) * math.cos(time)
	return x, y, z
end
