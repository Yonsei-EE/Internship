function [ D ] = ob( x, y, r )

hold on;
for i = 1:9;
    j = 0:0.1:2*pi;
    rx = x(i) + r(i)*cos(j); ry = y(i) + r(i)*sin(j);
    fill(rx,ry,'r');
end
x = [1 1000 1000 1 1];
y = [1 1 10 10 1];
fill(x,y,'b');
x = [1 1000 1000 1 1];
y = [990 990 1000 1000 990];
fill(x,y,'b');
x = [1 1 10 10 1];
y = [1 1000 1000 1 1];
fill(x,y,'b');
x = [990 990 1000 1000 990];
y = [1 1000 1000 1 1];
fill(x,y,'b');
axis([0 1000 0 1000])
hold off;
D = 0;
end

