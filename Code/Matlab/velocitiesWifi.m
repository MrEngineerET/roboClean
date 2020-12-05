clear;
clc;

%figure
counter = 1;
while counter <= 10
    str = webread('http://192.168.1.1/velocities')
    %Vr = str.Vr;
    %Vl = str.Vl;
    
    %V = str.V;
    %w = str.w;
    %t = str.t;
    %plot(Vr, t, '*')
    pause(0.2);
    %hold on
    counter = counter + 1;

end
%hold off;
clear;