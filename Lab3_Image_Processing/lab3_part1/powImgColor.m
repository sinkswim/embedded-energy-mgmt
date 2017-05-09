function [ Pimage ] = powImgColor( RGBimg )
% Computes image power given an RGBimg following the equation in the slides
% Extract channels
RGBimgScaled = double(RGBimg)./255;
R = RGBimgScaled(:,:,1);
G = RGBimgScaled(:,:,2);
B = RGBimgScaled(:,:,3);
Pimage = 7.362;     % C, static power
for i= 1:numel(R)
    Ppixel = 9.080*R(i).^3 - 2.225*R(i).^2 + 2.92*R(i) - 0.072 + 5.706*G(i).^3 + 2.38*G(i).^2 + 1.63*G(i) - 0.003 + 18.66*B(i).^3 - 2.46*B(i).^2 + 6.71*B(i) - 0.11;
    Pimage = Pimage + Ppixel;
end
end

