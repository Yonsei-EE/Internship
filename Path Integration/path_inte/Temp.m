clc;
clear all;
close all;
%% Initialize

% Robot
x = 250; y = 250; %initial positiob
head = 0; %initial head angle, radian
r = 55/2; % robot radius
L = 55/2; % distance of two wheels
SafeRange = 1.4*r;

% Obstacle
rr=[ 50 40 90 30 ];  
xx=[ 80 400 150 450 ];
yy=[ 350 100 100 450 ];
onum = size(rr,2);

% Path integration
Vx=0;  Vy=0;
PathData = [Vx Vy];


% Flags
avoid = 0;
avoid_cnt = 0;

PathRecovery=0;
Path_cnt = 3;
stop = 0;


% ETC
count = 400;
dt = 3/8;
t = 0:0.1:2*pi+0.2;


for n = 1:1:count,
    XY = [x y];    
%D = IRsensor_reading(head,XY,xx,yy,rr,onum);

    %% Robot Moving control
    for i = 1:onum
        obstacle_dis(i) = sqrt((xx(i)-x)^2 + (yy(i)-y)^2)-rr(i)-r;
        obstacle_arg(i) = atan2((yy(i)-y),(xx(i)-x));
        
        if (obstacle_arg(i) <0 )
            obstacle_arg(i) = 2*pi + obstacle_arg(i) ;
        end
        
        if (obstacle_dis(i)<SafeRange & avoid ==0 )
            avoid = 1;
        elseif (x<(10+SafeRange) | y<(10+SafeRange) | (490-x)<SafeRange | (490-y)<SafeRange)
            avoid = 4;
        end
    end
   
    %Avoiding Obstacle
    if (avoid == 1)
        avoid_num = find(obstacle_dis==min(obstacle_dis));
        
            argdiff = head - obstacle_arg(avoid_num); 
            if argdiff > pi
                argdiff = argdiff - 2*pi;
            elseif argdiff <-pi
                argdiff = argdiff + 2*pi;
            end
            
            if (argdiff > 0)
               VL = -10; VR = 10;
            else
               VL = 10; VR = -10;      
            end
        avoid =2;
        
    elseif (avoid ==2)
        argdiff = head - obstacle_arg(avoid_num); 
        if argdiff > pi
            argdiff = argdiff - 2*pi;
        elseif argdiff <-pi
            argdiff = argdiff + 2*pi;
        end
        
        if((argdiff>(pi/2)) | (argdiff < (-pi/2)))
            avoid = 3;
        end
    elseif avoid ==3
        VL= 7; VR = 7;
        avoid_cnt = avoid_cnt +1;
        if (avoid_cnt ==2)
            avoid =0;
            avoid_cnt = 0;
            warn_min_dis = 1000000;%
        end
       
    %Avoiding Wall
    elseif avoid ==4
        argdiff = head - atan2(250-y,250-x);
        if argdiff > pi
            argdiff = argdiff - 2*pi;
        elseif argdiff <-pi
            argdiff = argdiff + 2*pi;
        end
        
        if argdiff > pi/10
            VL = 10; VR =-10;
        elseif argdiff < -pi/10
            VL = -10; VR = 10;
        else
            VL = 10; VR = 10;
            avoid =0;
        end
        
        
    %Random moving & Path Integration
    else
        if PathRecovery == 1
            argdiff = head - atan2(Goal(2)-y,Goal(1)-x);
            if argdiff > pi
                argdiff = argdiff - 2*pi;
            elseif argdiff <-pi
                argdiff = argdiff + 2*pi;
            end
            if Path_cnt ==3
                
                if argdiff > pi/10
                    VL = 10; VR =-10;
                elseif argdiff < -pi/10
                    VL = -10; VR = 10;
                else
                    Path_cnt =0;
                end
            else
                Path_cnt = Path_cnt + 1;
                VL = 15; VR = 15;
            end
            if sqrt((Goal(1)-x)^2 + (Goal(2)-y)^2) < 10
                VL = 0; VR = 0;
                stop = 1;
            else 
                plot (Goal(1),Goal(2),'*');
                hold on;
            end
        else
            VL= rand*80;VR=rand*80;
        end
    end
    
    
    %% Moving sequence
    if VL==VR
        x = x + VR*cos(head);
        y = y + VR*sin(head);
    else
        x = x + (VR+VL)/(VR-VL)*r*(cos(head + (VR-VL)*dt/2/r - pi/2)-cos(head- pi/2));
        y = y + (VR+VL)/(VR-VL)*r*(sin(head + (VR-VL)*dt/2/r- pi/2)-sin(head- pi/2));
        head = head + (VR-VL)*dt/2/r;
    end

    if(head > 2 * pi)
        head = head - 2 * pi;
    elseif(head < 0)
        head = head + 2 * pi;
    end

    %% Path integration
    if PathRecovery ==0
        PathData = PathData + [x y] - XY;
    end
    
    if n == (count -80)
        PathRecovery = 1;
        Goal = [x y] - PathData;
    end
    
    
    %% Drawing sequence
    mapenv(rr,xx,yy,onum);
    rx = x + r * cos(t); ry = y + r * sin(t);   % robot drawing
    hold on;
    plot(rx,ry);                                % draw robot body
    line([x x+r*cos(head)], [y y+r*sin(head)])  % mark head

    hold off;
    
    pause(0.1);
 
    if (x>500 || y>500 || x<0||y<0 ||stop) break;end
end;
