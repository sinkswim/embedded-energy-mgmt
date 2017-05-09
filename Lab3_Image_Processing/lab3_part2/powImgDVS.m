function [PowPanel, Icell] = powImgDVS( RGBimg, Vdd )
% Computes power necessary by the OLED panel for displaying the image
scaledRGB = (double(RGBimg)./255);
Icell = 4.251e-05*Vdd*scaledRGB - 3.029e-04*scaledRGB + 3.024e-05;
PowPanel = 0;
IR = Icell(:,:,1);
IG = Icell(:,:,2);
IB = Icell(:,:,3);
for i= 1:numel(IR)
    PowPanel = PowPanel + IR(i) + IG(i) + IB(i);
end
PowPanel = PowPanel * Vdd;

end

