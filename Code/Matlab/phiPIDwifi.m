clear;
clc;

figure
hold on
title('Orientation of  the robot while moving from (0,0) to (150,150)');
hold on
xlabel('time(milli Second)');
hold on
ylabel('theta(radian)');
hold on;
counter = 1;
webread('http://192.168.1.1/startAuto');
pause(2);
while counter < 100
    str = webread('http://192.168.1.1/pose');
   time = str.time;
   phi = str.theta;
   if(isnumeric(phi))
        if(phi >= -pi && phi <= pi)
            counter = counter + 1;
            plot(time,phi,'r*-');
            pause(0.001);
            hold on
        end
   end
end
hold off
clear;