targetHB = 15.0;    % fix a X% distortion for hungry blue distortion
hungryAmount = 0;
resultsFile = fopen(strcat('resultsDay1HB', string(targetHB)), 'w')
fprintf(resultsFile, 'Filename      PowOriginal PowHungryBlue PowHistEq DistPercOrig2HungryBlue  DistPercOrig2HistEq    PowSaveOrig2HungryBlue    PowSaveOrig2HistEq     hungryAmount\n');
files = dir('./images/*.tiff');
for file = files'
    [ RGBorig, R, G, B, LABorig, L, a, b, HSVorig ] = getImgData( strcat('./images/' ,file.name) );
    for i=1:3:255   % loop through various hungry blue distortions to find the target one
        [ hungryBlueRGB, hungryBlueLAB] = hungryBlue( RGBorig, i );
        DistPercOrig2HungryBlue = distortionPercentage( LABorig, hungryBlueLAB );
        if(DistPercOrig2HungryBlue > targetHB)
            hungryAmount = i - 3;
            break
        end
    end  
    [ hungryBlueRGB, hungryBlueLAB] = hungryBlue( RGBorig, hungryAmount );
    [ histeqRGB, histeqHSV ] = valueEq( HSVorig );
    [heqL, heqa, heqb] = rgb2lab(histeqRGB);  
    histeqLAB = cat(3, heqL, heqa, heqb);
    PowOrig = powImgColor( RGBorig );
    PowHungryBlue = powImgColor( hungryBlueRGB );
    PowHistEq = powImgColor( histeqRGB );
    DistPercOrig2HungryBlue = distortionPercentage( LABorig, hungryBlueLAB );
    DistPercOrig2HistEq = distortionPercentage( LABorig, histeqLAB );
    PowSaveOrig2HungryBlue = PowSavings( PowOrig, PowHungryBlue );
    PowSaveOrig2HistEq = PowSavings( PowOrig, PowHistEq );
    fprintf(resultsFile, '%s   %e   %e   %e   %f   %f   %f   %f    %d\n', file.name, PowOrig, PowHungryBlue, PowHistEq, DistPercOrig2HungryBlue, DistPercOrig2HistEq, PowSaveOrig2HungryBlue, PowSaveOrig2HistEq, hungryAmount);
    subplot(1,3,1);
    subimage(RGBorig);
    subplot(1,3,2);
    subimage(hungryBlueRGB);
    subplot(1,3,3);
    subimage(histeqRGB);
    print(strcat('./imgcompar/', file.name),'-dpng');
end

files = dir('./images/*.png');
for file = files'
    [ RGBorig, R, G, B, LABorig, L, a, b, HSVorig ] = getImgData( strcat('./images/' ,file.name) );
    for i=1:3:255   % loop through various hungry blue distortions to find the target one
        [ hungryBlueRGB, hungryBlueLAB] = hungryBlue( RGBorig, i );
        DistPercOrig2HungryBlue = distortionPercentage( LABorig, hungryBlueLAB );
        if(DistPercOrig2HungryBlue > targetHB)
            hungryAmount = i - 3;
            break
        end
    end  
    [ hungryBlueRGB, hungryBlueLAB] = hungryBlue( RGBorig, hungryAmount );
    [ histeqRGB, histeqHSV ] = valueEq( HSVorig );
    [heqL, heqa, heqb] = rgb2lab(histeqRGB);  
    histeqLAB = cat(3, heqL, heqa, heqb);
    PowOrig = powImgColor( RGBorig );
    PowHungryBlue = powImgColor( hungryBlueRGB );
    PowHistEq = powImgColor( histeqRGB );
    DistPercOrig2HungryBlue = distortionPercentage( LABorig, hungryBlueLAB );
    DistPercOrig2HistEq = distortionPercentage( LABorig, histeqLAB );
    PowSaveOrig2HungryBlue = PowSavings( PowOrig, PowHungryBlue );
    PowSaveOrig2HistEq = PowSavings( PowOrig, PowHistEq );
    fprintf(resultsFile, '%s   %e   %e   %e   %f   %f   %f   %f    %d\n', file.name, PowOrig, PowHungryBlue, PowHistEq, DistPercOrig2HungryBlue, DistPercOrig2HistEq, PowSaveOrig2HungryBlue, PowSaveOrig2HistEq, hungryAmount);
    subplot(1,3,1);
    subimage(RGBorig);
    subplot(1,3,2);
    subimage(hungryBlueRGB);
    subplot(1,3,3);
    subimage(histeqRGB);
    print(strcat('./imgcompar/', file.name),'-dpng');
end