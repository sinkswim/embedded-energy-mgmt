function [ hungryBlueRGB, hungryBlueLAB] = hungryBlue( RGBimg, amount )
% Performs a "hungry blue" distortion by  specified amount
R = RGBimg(:,:,1);
G = RGBimg(:,:,2);
B = RGBimg(:,:,3);
newB = B - amount;
hungryBlueRGB = cat(3, R, G, newB);
[newL, newa, newb] = rgb2lab(hungryBlueRGB);
hungryBlueLAB = cat(3, newL, newa, newb);
end

