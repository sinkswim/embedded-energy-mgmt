function [ RGBopt, newVdd, optPow ] = optimizePower( inputRGB, startVdd, target_dist, LABorig )
% Given an input RGB and Vdd it returns:
% 1) an RGB image obtained by optimizing the power consumption by 
% furtherly lowering the Vdd (DVS) (thus obtaining a worse distortion
% eq. to target_dist)
% 2) the new Vdd.
for i = startVdd-0.025:-0.025:5.0
        [optPow, optIcell] = powImgDVS( inputRGB, i );
        RGBopt = displayed_image(optIcell, i, 1);
        LABopt = getLAB(RGBopt);
        distortion = distortionPercentage( LABorig, LABopt );
        if distortion > target_dist
            newVdd = i + 0.025;    % return to previous valid statement
            break
        end
end
[inPow, inIcell] = powImgDVS( inputRGB, newVdd );
RGBopt = displayed_image(inIcell, newVdd, 1);
[optPow, optIcell] = powImgDVS( RGBopt, newVdd );

end

