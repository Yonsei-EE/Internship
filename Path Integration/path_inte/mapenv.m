function [xx, yy, rr] = mapenv(rr,xx,yy,onum)
%obstacle ��
t=0:0.1:2*pi+0.1;
%
%rr=[ 10 20 100 ];
%xx=[ 80 300 150 ];
%yy=[ 298 100 100];
%onum = 3;

for i = 1:onum,
    x=rr(i)*cos(t) + xx(i); y=rr(i)*sin(t) + yy(i);
    fill(x,y,'r');
    hold on
end

%�Ʒ���
hold on
x = [1 1 500 500 1];
y = [1 10 10 1 1];
fill(x, y, 'b*')

%����
hold on
x = [0 0 500 500 0];
y = [490 500 500 490 490];
fill(x, y, 'b*')
hold on

%�����ʺ�
hold on
x = [490 490 500 500 490];
y = [10 490 490 10 10];
fill(x, y, 'b*')

%���ʺ�
x = [0 0 10 10 0];
y = [10 490 490 10 10];
fill(x, y, 'b*')

axis([0 500 0 500])
grid on