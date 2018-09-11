function [velocity,radius] = getvr(velocity,radius)
%% get velocity
if(velocity<0)
    velocity = dec2hex(16^4+velocity);
else
    velocity = dec2hex(velocity);
end 
for i = 1:(4-length(velocity))
    velocity = strcat('0',velocity);
end
velocity = [hex2dec(velocity(3:4)) hex2dec(velocity(1:2))];

%% get radius
if(radius<0)
    radius = dec2hex(16^4+radius);
else
    radius = dec2hex(radius);
end   
for i = 1:(4-length(radius))
    radius = strcat('0',radius);
end
radius = [hex2dec(radius(3:4)) hex2dec(radius(1:2))];

end

