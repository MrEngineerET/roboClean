clear;
clc;

figure
plot(0,0,'or');
hold on;
plot(100,100,'or');
hold on;
plot(110,110);
hold on;
counter = 1;
while counter <= 100
    str = webread('http://192.168.1.1/pose');
    x = str.x;
    y = str.y;
    plot(x, y, '*')
    pause(0.01);
    hold on
    counter = counter + 1;

end
hold off;
clear;