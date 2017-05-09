function [ LABver] = getLAB( RGBimg )
% Returns LAB version of RGB image and its 3 separate channels
R = RGBimg(:,:,1);
G = RGBimg(:,:,2);
B = RGBimg(:,:,3);
[Lchan, achan, bchan] = rgb2lab(RGBimg);  
LABver = cat(3, Lchan, achan, bchan);

end

