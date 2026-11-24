%% Real-time 3-D plot of MSP432 accelerometer data
% Reads lines of the form "X: <x> Y: <y> Z: <z>" from the MSP432 serial port
% and plots each sample as a point in 3-D space.
%
% Usage: set MSP432_PORT to your board's port, run, press Ctrl+C to stop.
%   macOS  : '/dev/cu.usbmodemXXXXXXXX'
%   Windows: 'COM3'
%   Linux  : '/dev/ttyACM0'

clear; clc; close all;

MSP432_PORT = '/dev/cu.usbmodemM43210051';
BAUDRATE    = 9600;

fprintf('Connecting to MSP432 on %s ...\n', MSP432_PORT);
try
    s = serialport(MSP432_PORT, BAUDRATE);
    s.Timeout = 5;
    flush(s);
    fprintf('Connected! Reading & plotting data...\n\n');
catch ME
    fprintf('Connection failed: %s\n', ME.message);
    return;
end

figure('Name', 'MSP432 Accelerometer XYZ Plot');
grid on;
xlabel('X'); ylabel('Y'); zlabel('Z');
title('Real-Time Accelerometer Data (MSP432)');
view(3);

fprintf('Press Ctrl+C to stop.\n');
fprintf('----------------------\n');

%% Read & plot loop
while true
    try
        data_line = readline(s);
        data_str  = strtrim(char(data_line));
        tokens = regexp(data_str, ...
            'X:\s*(-?\d+)\s*Y:\s*(-?\d+)\s*Z:\s*(-?\d+)', 'tokens');

        if ~isempty(tokens)
            vals = str2double(tokens{1});
            x = vals(1); y = vals(2); z = vals(3);

            plot3(x, y, z, 'o', 'MarkerSize', 10, 'MarkerFaceColor', 'b');
            xlim([-1000 1000]); ylim([-1000 1000]); zlim([-1000 1000]);
            xlabel('X'); ylabel('Y'); zlabel('Z');
            grid on;
            drawnow;
        else
            disp(['Invalid line: ' data_str]);
        end
    catch ME
        fprintf('Serial read error: %s\n', ME.message);
        break;
    end
end

%% Cleanup
try
    clear s;
    fprintf('Serial port closed.\n');
catch
end
