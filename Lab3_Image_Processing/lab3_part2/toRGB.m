function [ R, G, B ] = toRGB( image )
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here
R = image(:, :, 1);
G = image(:, :, 2);
B = image(:, :, 3);

end

