close all;
clc;

divider = 50/600;
x = 1:divider:20;

delete(instrfind({'Port'},{'COM9'}));
serial_port = serial('COM9', 'Terminator', 'CR/LF');
port_serial.Baudrate = 9600;
warning('off','MATLAB:serial:fscanf:unsuccessfulread');

fopen(serial_port);

counter = 1;
outCounter = 1;
while counter <= 600
   value = str2num(fscanf(serial_port));
    if(value > -2 * pi && value < 2 * pi)
        counter += 1;
        plot(x(counter),value);
        pause(0.01);
        hold on
    end
    outCounter += 1;
    if (outCounter == 1000);
        counter = 600;
    end
end

fclose(serial_port);