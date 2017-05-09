function [ histeqRGB, histeqHSV ] = valueEq( HSVimg )
% Returns modified image after equalizing the Value histogram
% Get channels
H = HSVimg(:,:,1);
S = HSVimg(:,:,2);
V = HSVimg(:,:,3);
% Perform equalization and create new image matrices
newV = histeq(V);
histeqHSV = cat(3, H, S, newV);
histeqRGB = uint8(hsv2rgb(histeqHSV).*255);
end