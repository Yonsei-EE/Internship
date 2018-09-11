%% to do
% obstacle avoidance during exporation and returing home

%% initalization
head = rand*pi; %initial head angle
r = 55/2; % robot radius
L = 50;
or = [4, 6, 8, 6, 8, 9, 10, 5, 7]*10;
ox = [100, 70, 60, 80, 8, 90, 10, 40, 10]*10;
oy = [25, 75, 100, 100, 20, 10, 100, 10, 60]*10;
danger = true;
while(danger)
    x = rand * 500 + 250; y = rand * 500 + 250; %initial position
    x0 = x; y0 = y;
    dx = 0; dy = 0;
    head = pi/4; %initial head angle
    ir = irsensor( x, y, L, head, ox, oy, or);
    danger = ir(1)<15||ir(2)<10||ir(8)<10||ir(7)<3||ir(3)<3;
end

%% random movement
for j = 1:100
    Lm=rand*10;
    Rm=rand*10;
    
    for i = 1:10
        t = 0:0.1:2*pi+0.2;
        
        % irsensor
        ir = irsensor( x, y, L, head, ox, oy, or);
        if(ir(1)<15||ir(2)<10||ir(8)<10||ir(7)<3||ir(3)<3)
            diri = randi([1,2],1,1);
            dir = [-1 ,1];
            Lm = dir(diri)*8;
            Rm = -dir(diri)*8;
        end
        
        % kinematics
        if(Lm<Rm)
            R = L/((Rm/Lm)-1)+L/2; % radius of rotation
            w = Lm/(R-L/2); % angular velocity
            o = [x + R*-sin(head), y + R*cos(head)]; % center of rotation
        elseif(Lm>Rm)
            R = L/((Lm/Rm)-1)+L/2;
            w = -Rm/(R-L/2);
            o = [x + R*sin(head), y + R*-cos(head)];
        else
            R = Inf;
            w = 0;
            o = [0,0];
            x = x + Lm*cos(head);
            y = y + Lm*sin(head);
        end
        
        % rotate the position along the center of rotation
        first = [...
            1 0 -o(1);
            0 1 -o(2);
            0 0 1];
        third = [...
            1 0 o(1);
            0 1 o(2);
            0 0 1];
        second = [...
            cos(w) -sin(w) 0;
            sin(w) cos(w) 0;
            0 0 1];
        rotated = third*second*first*[x,y,1]'; 
        x = rotated(1);
        y = rotated(2);

        % path integration
        if(Lm<Rm)
            dx = dx + 2*R*sin(w/2)*cos(head+w/2); % path integration for x
            dy = dy + 2*R*sin(w/2)*sin(head+w/2); % path integration for y
        elseif(Lm>Rm)
            dx = dx + 2*R*sin(-w/2)*cos(head+w/2);
            dy = dy + 2*R*sin(-w/2)*sin(head+w/2);
        else
            dx = dx + Lm*cos(head);
            dy = dy + Lm*sin(head);
        end
        disp([dx dy head]);
        % plot simulation
        head = head + w; % head = theta
        rx = x + r * cos(t); ry = y + r * sin(t); % robot drawing
        plot(rx,ry); % draw robot body
        hold on;
        plot(o(1),o(2),'ro'); % plot the center of rotation
        plot(x0,y0,'bx');
        line([x x+r*cos(head)], [y y+r*sin(head)]) % mark head
        axis([0 1000 0 1000])
        k = ob(ox,oy,or);
        pause(0.01); %you can control the speed of animation with this
    end
end

%% face home
home = atan2(-dy,-dx);
dh = (home - head)/10;
for i = 1:10
    head = head + dh; % head = theta
    rx = x + r * cos(t); ry = y + r * sin(t); % robot drawing
    plot(rx,ry); % draw robot body
    hold on;
    plot(x0,y0,'bx');
    line([x x+r*cos(head)], [y y+r*sin(head)]) % mark head
    axis([0 1000 0 1000])
    k = ob(ox,oy,or);
    pause(0.01); %you can control the speed of animation with this
end

%% return to home
v = rand*5 + 5;
for i = 1:sqrt(dx^2+dy^2)/v
    x = x + v*cos(head);
    y = y + v*sin(head);
    rx = x + r * cos(t); ry = y + r * sin(t); % robot drawing
    plot(rx,ry); % draw robot body
    hold on;
    plot(x0,y0,'bx');
    line([x x+r*cos(head)], [y y+r*sin(head)]) % mark head
    axis([0 1000 0 1000])
    k = ob(ox,oy,or);
    pause(0.01); %you can control the speed of animation with this
end