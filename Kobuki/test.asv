%% initialize
robot = serial( 'COM4', 'Baudrate',115200);
fopen(robot);
pause(2);

cclockwise = [1 0];
clockwise = [255 255];
straight = [0 128];
R = 200;

%% straight
[velocity, radius] = getvr(200,R);
control(robot,velocity,straight);
pause(1);
control(robot,[0 0],[0 0]);

%% backwards
[velocity, radius] = getvr(-200,R);
control(robot,velocity,straight);
pause(1);
control(robot,[0 0],[0 0]);
