clear;
clc;

delete(instrfind({'Port'},{'COM9'}));
serial_port = serial('COM9', 'Terminator', 'CR/LF');
port_serial.Baudrate = 9600;
warning('off','MATLAB:serial:fscanf:unsuccessfulread');

fopen(serial_port);

figure
title('Position of the Robot');
xlabel('X axis(cm)');
ylabel('Y axis(cm)');
plot(0,0,'or');
hold on;
plot(100,100,'or');
hold on;
plot(110,110);
hold on;
counter = 1;
while counter <= 100
   str = fscanf(serial_port);
    values = strsplit(str, ',');
    len = size(values);
    len = len(2);
    if len == 4
         x = str2double(values{1});
         y = str2double(values{2});
         if isnan(x) || isnan(y)
             continue;
         end
         counter = counter + 1;
         plot(x, y, '*')
         pause(0.01);
         hold on
        % [phi, Phid, error]
     end
end
hold off;
fclose(serial_port);
clear;