function [ RGBorig, R, G, B, LABorig, L, a, b, HSVorig ] = getImgData( imgFile )
% Gets image file (path) and returns a set of data to be used later for
% image processing.
RGBorig = imread(imgFile);
R = RGBorig(:,:,1);
G = RGBorig(:,:,2);
B = RGBorig(:,:,3);
[L, a, b] = rgb2lab(RGBorig);  
LABorig = cat(3, L, a, b);
HSVorig = rgb2hsv(double(RGBorig)./255);  % [0, 255] scaled to [0, 1]
end

