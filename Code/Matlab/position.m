close all;
clc;
y = zeros(1,1000);

delete(instrfind({'Port'},{'COM9'}));
serial_port = serial('COM9', 'Terminator', 'CR/LF');
port_serial.Baudrate = 9600;
warning('off','MATLAB:serial:fscanf:unsuccessfulread');

fopen(serial_port);

sample_counter = 1;
while sample_counter <= 800
   potentiometer_value = fscanf(serial_port);
    C = strsplit(potentiometer_value, ',');
    len = size(C);
    len = len(2);
    if len == 2
         X = str2num(C{1});
         Y = str2num(C{2});
    % phi = str2num (C {3});
    % Phid = str2num (C {4});
    % error = str2num (C {5});
    % Rdistance = str2num (C {6});
    % Ldistance = str2num (C {7});
         sample_counter = sample_counter + 1;
         plot(X, Y, '*')
         pause(0.01);
         hold on
        % [phi, Phid, error]
     end
end

fclose(serial_port);