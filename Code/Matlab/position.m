clear;
clc;

delete(instrfind({'Port'},{'COM9'}));
serial_port = serial('COM9', 'Terminator', 'CR/LF');
port_serial.Baudrate = 9600;
warning('off','MATLAB:serial:fscanf:unsuccessfulread');

fopen(serial_port);

figure
plot(0,0,'or');
plot(300,300,'or');
hold on;
counter = 1;
while counter <= 200
   str = fscanf(serial_port);
    values = strsplit(str, ',');
    len = size(values);
    len = len(2);
    if len == 4
         x = str2num(values{1});
         y = str2num(values{2});
    % phi = str2num (values {3});
    % Phid = str2num (values {4});
    % error = str2num (values {5});
    % Rdistance = str2num (values {6});
    % Ldistance = str2num (values {7});
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