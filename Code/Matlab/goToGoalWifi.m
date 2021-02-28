clear;
clc;

figure
grid on
hold on
plot(0,0);
hold on
plot(300,300);
hold on
title('Position of the Robot moving from (0,50) to (300,50)');
hold on
xlabel('X direction(cm)');
hold on
ylabel('Y direction(cm)');  
hold on
plot(0,50,'or');
hold on;
plot(300,50,'or');
hold on;
counter = 1;
webread('http://192.168.1.1/startAuto');
pause(2);
while counter <= 150
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