function [ distortionPerc] = distortionPercentage( original, new )
% Computes distortion in percentage given two LAB space images
tmp1 = original(:, :, 1);
LABorigin = cat(3, zeros(size(tmp1)), zeros(size(tmp1)), zeros(size(tmp1)));
distNewOriginal = evalDistance(new, original);
distOriginalLABorigin = evalDistance(original, LABorigin);
distortionPerc = (distNewOriginal/distOriginalLABorigin)*100;

end

