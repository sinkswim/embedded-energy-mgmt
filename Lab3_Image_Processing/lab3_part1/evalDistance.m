function [ distance ] = evalDistance( img1, img2 )
% Given two input LAB space images computes the distance b/n them
% Extract channels
L1 = img1(:,:,1);
L2 = img2(:,:,1);
a1 = img1(:,:,2);
a2 = img2(:,:,2);
b1 = img1(:,:,3);
b2 = img2(:,:,3);
distance = 0.0;
for i= 1:numel(L1)
    distance = distance + sqrt((L1(i) - L2(i)).^2+(a1(i) - a2(i)).^2+(b1(i) - b2(i)).^2);
end
end

