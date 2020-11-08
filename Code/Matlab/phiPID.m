clear;
clc;



delete(instrfind({'Port'},{'COM9'}));
serial_port = serial('COM9', 'Terminator', 'CR/LF');
port_serial.Baudrate = 9600;
warning('off','MATLAB:serial:fscanf:unsuccessfulread');

fopen(serial_port);
figure;
counter = 1;
while counter < 200
   str = fscanf(serial_port);
   values = strsplit(str,',');
   time = str2num(values{3});
   phi = str2num(values{4});
   if(isnumeric(phi))
        if(phi >= -pi && phi <= pi)
            counter = counter + 1;
            plot(time,phi,'r*-');
            pause(0.01);
            hold on
        end
   end
end
hold off
fclose(serial_port);
clear;