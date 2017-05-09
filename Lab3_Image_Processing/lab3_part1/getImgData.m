function [ RGBimg, R, G, B, LABimg, L, a, b, HSVimg ] = getImgData( imgFile )
% Gets image file (path) and returns a set of data to be used later for
% image processing.
RGBimg = imread(imgFile);
R = RGBimg(:,:,1);
G = RGBimg(:,:,2);
B = RGBimg(:,:,3);
[L, a, b] = rgb2lab(RGBimg);  
LABimg = cat(3, L, a, b);
HSVimg = rgb2hsv(double(RGBimg)./255);  % [0, 255] scaled to [0, 1]
end

