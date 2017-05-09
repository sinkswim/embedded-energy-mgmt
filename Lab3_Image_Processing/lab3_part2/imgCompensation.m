function [ compensatedRGBimg, compensatedHSVimg ] = imgCompensation( RGBimg, b, mode )
% Performs brightness or contrast mods
BRIGHTNESS = 1; % V' = v + b
CONTRAST = 2;   % V' = V * b
HSVimg = rgb2hsv(double(RGBimg)./255);
% Extract V channel
Vchan = HSVimg(:,:,3);
if(mode == BRIGHTNESS)
    Vchan = Vchan + b;
else if(mode == CONTRAST)
        Vchan = Vchan*b;
    end 
end
Hchan = HSVimg(:,:,1);
Schan = HSVimg(:,:,2);
compensatedHSVimg = cat(3, Hchan, Schan, Vchan);
compensatedRGBimg = uint8(hsv2rgb(compensatedHSVimg).*255);

end

