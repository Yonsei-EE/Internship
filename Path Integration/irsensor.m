function [ ir ] = irsensor( x, y, L, head, ox, oy, or)
ir = zeros(1,8);
dangle = 2*pi/8;
flag = [true, true, true, true, true, true, true, true];
angle = [head, head - dangle, head - dangle*2, head - dangle*3, head - dangle*4, head - dangle*5, head - dangle*6, head - dangle*7];
for i = 1:1024
    for j = 1:9
        for k = 1:8
            if(flag(k))
                if((x+(L/2+i)*cos(angle(k)) - ox(j))^2 + (y + (L/2+i)*sin(angle(k))-oy(j))^2<or(j)^2)
                    ir(k) = i;
                    flag(k) = false;
                end
                if(x+(L/2+i)*cos(angle(k))>990)
                    ir(k) = i;
                    flag(k) = false;
                end
                if(x+(L/2+i)*cos(angle(k))<10)
                    ir(k) = i;
                    flag(k) = false;
                end
                if(y + (L/2+i)*sin(angle(k))>990)
                    ir(k) = i;
                    flag(k) = false;
                end
                if(y + (L/2+i)*sin(angle(k))<10)
                    ir(k) = i;
                    flag(k) = false;
                end
            end
        end
    end
end
end

