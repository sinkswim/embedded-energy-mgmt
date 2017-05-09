% Assignment 2, part 1. Assuming only 1 image to be uploaded
DVS_dist_target = 10.0;   % tested with 1.0,  10.0,  15.0
foundVdd = 0.0;
foundBrightB = 0.0;
foundContrB = 0.0;
foundComboB = 0.0;
results = fopen(strcat('resultsDay2Perc', string(DVS_dist_target)), 'w');

% Perform whole analysis on tiff images (standard db)
files = dir('./images/*.tiff');
for file = files'
    foundVdd = 0.0;
    foundBrightB = 0.0;
    foundContrB = 0.0;
    foundComboB = 0.0;
    fprintf(results, 'Filename    PowOrig     PowDVS     PowBrightDVS    PowContrastDVS    PowComboDVS\n');
    [ RGBorig, R, G, B, LABorig, L, a, b, HSVorig ] = getImgData( strcat('./images/' ,file.name) );
    [PowPanelOrig, IcellOrig] = powImgDVS( RGBorig, 15 );
    for i = 14.975:-0.025:12.0
        %compute power_consumption
        %Apply DVS
        %Compute again power consumption
        % compute distorsion
        [ScaledPow, ScaledIcell] = powImgDVS( RGBorig, i );
        RGBVnew = displayed_image(ScaledIcell, i, 1);
        LABVnew = getLAB( RGBVnew);
        distortion = distortionPercentage( LABorig, LABVnew );
        if distortion > DVS_dist_target
            foundVdd = i + 0.025;    % return to previous valid statement
            %compute again all needed stuffs with i = i + 0.25
            % fprintf(file1,...
            % save power and distorsion, Vdd current value
            % save the modifed images as well
            break
        end
    end
    % A DVS filtering with target distortion has been found. Compute its power.
    [ScaledPow, ScaledIcell] = powImgDVS( RGBorig, foundVdd );
    RGBVnew = displayed_image(ScaledIcell, foundVdd, 1);
    LABVnew = getLAB( RGBVnew);
    Orig2DVSDist = distortionPercentage( LABorig, LABVnew );
    [PowDVS, IcellDVS] = powImgDVS( RGBVnew,  foundVdd);


    %Assignment 2, part 2. Brightness mode
    % variable used for indexing the arrays power and distorsion
    minDist = DVS_dist_target;
    for j = 0.025 : 0.025 : 2.0 % inner loop is for b
        % Compute brightness
        % Apply VDS 
        % Compute Power
        % Compute Distorsion
        [RGBbright, HSVbright ] = imgCompensation( RGBorig, j, 1 ); % mode 1: brightness
        [PowPanelBright, IcellBright] = powImgDVS( RGBbright, foundVdd ); % use the Vdd found above
        brightRGBDVS = displayed_image(IcellBright, foundVdd, 1);
        brightLABDVS = getLAB( brightRGBDVS );
        brightDVSDist = distortionPercentage( LABorig, brightLABDVS);
        if (brightDVSDist < (double(DVS_dist_target)/2))
            foundBrightB = j;
            break
        else if(brightDVSDist < minDist)
            minDist = brightDVSDist;
            foundBrightB = j;
            end
        end
    end
    % compute again everything with the new value of B (foundBrightB)
    [RGBbright, HSVbright ] = imgCompensation( RGBorig, foundBrightB, 1 ); % mode 1: brightness
    [PowPanelBright, IcellBright] = powImgDVS( RGBbright, foundVdd ); % use the Vdd found above
    brightRGBDVS = displayed_image(IcellBright, foundVdd, 1);
    brightLABDVS = getLAB( brightRGBDVS );
    Orig2BrightDVSDist = distortionPercentage( LABorig, brightLABDVS);
    [PowBrightDVS, IcellBrightDVS] = powImgDVS( brightRGBDVS,  foundVdd);

    %Assignment 2, part 2. Contrast mode
    minDist = DVS_dist_target;
    for j = 0.025 : 0.025 : 2.0 % inner loop is for b
        % Compute brightness
        % Apply VDS 
        % Compute Power
        % Compute Distorsion
        [RGBcontrast, HSVcontrast ] = imgCompensation( RGBorig, j, 2 ); % mode 2: contrast
        [PowPanelContrast, IcellContrast] = powImgDVS( RGBcontrast, foundVdd ); % use the Vdd found above
        contrastRGBDVS = displayed_image(IcellContrast, foundVdd, 1);
        [contrastLABDVS] = getLAB( contrastRGBDVS );
        contrastDVSDist = distortionPercentage( LABorig, contrastLABDVS);
        if (contrastDVSDist < (double(DVS_dist_target)/2))
            foundContrB = j;
            break
        else if(contrastDVSDist < minDist)
            minDist = contrastDVSDist;
            foundContrB = j;
            end
        end
    end
    % compute again everything with the new value of B (foundContrB)
    [RGBcontrast, HSVbright ] = imgCompensation( RGBorig, foundContrB, 2 ); % mode 2: contrast
    [PowPanelBright, IcellContrast] = powImgDVS( RGBcontrast, foundVdd ); % use the Vdd found above
    contrastRGBDVS = displayed_image(IcellContrast, foundVdd, 1);
    contrastLABDVS = getLAB( contrastRGBDVS );
    Orig2ContrDVSDist = distortionPercentage( LABorig, contrastLABDVS);
    [PowContrastDVS, IcellContrastDVS] = powImgDVS( contrastRGBDVS,  foundVdd); 

    %Assignment 2, part 2. Combo = brightness + contrast modes
    minDist = DVS_dist_target;
    for j = 0.025 : 0.025 : 2.0     
        [RGBCombo, HSVCombo ] = imgCompensation( RGBorig, j, 1 ); % mode 1: brightness
        [RGBCombo, HSVCombo ] = imgCompensation( RGBCombo, j+1, 2 ); % mode 2: contrast
        [PowPanelCombo, IcellCombo] = powImgDVS( RGBCombo, foundVdd ); % use the Vdd found above
        comboRGBDVS = displayed_image(IcellCombo, foundVdd, 1);
        [comboLABDVS] = getLAB( comboRGBDVS );
        comboDVSDist = distortionPercentage( LABorig, comboLABDVS);
        if (comboDVSDist < (double(DVS_dist_target)/2))
            foundComboB = j;
            break
         else if(comboDVSDist < minDist)
            minDist = comboDVSDist;
            foundComboB = j;
             end
        end
    end
    % compute again everything with the new value of B (foundComboB)
    [RGBCombo, HSVCombo ] = imgCompensation( RGBorig, foundComboB, 1 ); % mode 1: brightness
    [RGBCombo, HSVCombo ] = imgCompensation( RGBCombo, foundComboB+1, 2 ); % mode 2: contrast
    [PowPanelCombo, IcellCombo] = powImgDVS( RGBCombo, foundVdd ); % use the Vdd found above
    comboRGBDVS = displayed_image(IcellCombo, foundVdd, 1);
    comboLABDVS = getLAB( comboRGBDVS );
    Orig2ComboDVSDist = distortionPercentage( LABorig, comboLABDVS);
    [PowComboDVS, IcellComboDVS] = powImgDVS( comboRGBDVS,  foundVdd); 

    % Perform power optimizations starting from compensated images
    [ RGBoptBrightness, newVddBrightness, PowBrightDVSOpt ] = optimizePower( RGBbright, foundVdd, DVS_dist_target, LABorig );
    [ RGBoptContrast, newVddContrast, PowContrastDVSOpt ] = optimizePower( RGBcontrast, foundVdd, DVS_dist_target, LABorig );
    [ RGBoptCombo, newVddCombo, PowComboDVSOpt ] = optimizePower( RGBCombo, foundVdd, DVS_dist_target, LABorig );
    
    % Compute all power savings
    Orig2DVSPowSav = PowSavings( PowPanelOrig, PowDVS  );
    Orig2BrightDVSPowSav = PowSavings( PowPanelOrig, PowBrightDVS  );     
    Orig2ContrDVSPowSav = PowSavings( PowPanelOrig, PowContrastDVS );     
    Orig2ComboDVSPowSav = PowSavings( PowPanelOrig, PowComboDVS );
    Orig2BrightDVSPowSavOpt = PowSavings( PowPanelOrig, PowBrightDVSOpt  );     
    Orig2ContrDVSPowSavOpt = PowSavings( PowPanelOrig, PowContrastDVSOpt );     
    Orig2ComboDVSPowSavOpt = PowSavings( PowPanelOrig, PowComboDVSOpt );

    % Print to file
    fprintf(results, '%s    %e    %e     %e     %e     %e\n', file.name, PowPanelOrig, PowDVS, PowBrightDVS, PowContrastDVS, PowComboDVS);
    
    fprintf(results, 'Filename    PowOrig      PowBrightDVSOpt    PowContrastDVSOpt    PowComboDVSOpt\n');
    fprintf(results, '%s    %e     %e     %e     %e\n', file.name, PowPanelOrig, PowBrightDVSOpt, PowContrastDVSOpt, PowComboDVSOpt);
    
    fprintf(results, 'Filename     Orig2DVSDist    Orig2BrightDVSDist     Orig2ContrDVSDist     Orig2ComboDVSDist\n');
    fprintf(results, '%s     %f    %f     %f     %f\n', file.name, Orig2DVSDist, Orig2BrightDVSDist, Orig2ContrDVSDist, Orig2ComboDVSDist);
    fprintf(results, 'Filename   Orig2DVSPowSav    Orig2BrightDVSPowSav     Orig2ContrDVSPowSav     Orig2ComboDVSPowSav\n');
    fprintf(results, '%s    %f    %f    %f    %f\n', file.name, Orig2DVSPowSav, Orig2BrightDVSPowSav, Orig2ContrDVSPowSav, Orig2ComboDVSPowSav);
    
    fprintf(results, 'Filename    Orig2BrightDVSPowSavOpt     Orig2ContrDVSPowSavOpt     Orig2ComboDVSPowSavOpt\n');
    fprintf(results, '%s    %f    %f    %f\n', file.name, Orig2BrightDVSPowSavOpt, Orig2ContrDVSPowSavOpt, Orig2ComboDVSPowSavOpt);
    fprintf(results, 'Filename  newVddBrightness   newVddContrast  newVddCombo \n');
    fprintf(results, '%s    %f   %f   %f\n', file.name, newVddBrightness, newVddContrast, newVddCombo  );
    
    fprintf(results, 'Filename   foundVdd    foundBrightB   foundContrB   foundComboB\n');
    fprintf(results, '%s    %f   %f   %f   %f\n\n\n\n', file.name, foundVdd, foundBrightB, foundContrB, foundComboB);
    
    % Subplotting figures
    subplot(1, 5, 1);
    subimage(RGBorig);
    subplot(1, 5, 2);
    subimage(RGBVnew);
    subplot(1, 5, 3);
    subimage(brightRGBDVS);
    subplot(1, 5, 4);
    subimage(contrastRGBDVS);
    subplot(1, 5, 5);
    subimage(comboRGBDVS);
    print(strcat('./imgcompar/', file.name),'-dpng');
    
    subplot(1, 4, 1);
    subimage(RGBorig);
    subplot(1, 4, 2);
    subimage(RGBoptBrightness);
    subplot(1, 4, 3);
    subimage(RGBoptContrast);
    subplot(1, 4, 4);
    subimage(RGBoptCombo);
    print(strcat('./imgcomparOpt/', file.name),'-dpng');
end

% Perform whole analysis on png images (screenshots)
files = dir('./images/*.png');
for file = files'
    foundVdd = 0.0;
    foundBrightB = 0.0;
    foundContrB = 0.0;
    foundComboB = 0.0;
    fprintf(results, 'Filename    PowOrig     PowDVS     PowBrightDVS    PowContrastDVS    PowComboDVS\n');
    [ RGBorig, R, G, B, LABorig, L, a, b, HSVorig ] = getImgData( strcat('./images/' ,file.name) );
    [PowPanelOrig, IcellOrig] = powImgDVS( RGBorig, 15 );
    for i = 14.975:-0.025:12.0
        %compute power_consumption
        %Apply DVS
        %Compute again power consumption
        % compute distorsion
        [ScaledPow, ScaledIcell] = powImgDVS( RGBorig, i );
        RGBVnew = displayed_image(ScaledIcell, i, 1);
        LABVnew = getLAB( RGBVnew);
        distortion = distortionPercentage( LABorig, LABVnew );
        if distortion > DVS_dist_target
            foundVdd = i + 0.025;    % return to previous valid statement
            %compute again all needed stuffs with i = i + 0.25
            % fprintf(file1,...
            % save power and distorsion, Vdd current value
            % save the modifed images as well
            break
        end
    end
    % A DVS filtering with target distortion has been found. Compute its power.
    [ScaledPow, ScaledIcell] = powImgDVS( RGBorig, foundVdd );
    RGBVnew = displayed_image(ScaledIcell, foundVdd, 1);
    LABVnew = getLAB( RGBVnew);
    Orig2DVSDist = distortionPercentage( LABorig, LABVnew );
    [PowDVS, IcellDVS] = powImgDVS( RGBVnew,  foundVdd);


    %Assignment 2, part 2. Brightness mode
    % variable used for indexing the arrays power and distorsion
    minDist = DVS_dist_target;
    for j = 0.025 : 0.025 : 2.0 % inner loop is for b
        % Compute brightness
        % Apply VDS 
        % Compute Power
        % Compute Distorsion
        [RGBbright, HSVbright ] = imgCompensation( RGBorig, j, 1 ); % mode 1: brightness
        [PowPanelBright, IcellBright] = powImgDVS( RGBbright, foundVdd ); % use the Vdd found above
        brightRGBDVS = displayed_image(IcellBright, foundVdd, 1);
        brightLABDVS = getLAB( brightRGBDVS );
        brightDVSDist = distortionPercentage( LABorig, brightLABDVS);
        if (brightDVSDist < (double(DVS_dist_target)/2))
            foundBrightB = j;
            break
        else if(brightDVSDist < minDist)
            minDist = brightDVSDist;
            foundBrightB = j;
            end
        end
    end
    % compute again everything with the new value of B (foundBrightB)
    [RGBbright, HSVbright ] = imgCompensation( RGBorig, foundBrightB, 1 ); % mode 1: brightness
    [PowPanelBright, IcellBright] = powImgDVS( RGBbright, foundVdd ); % use the Vdd found above
    brightRGBDVS = displayed_image(IcellBright, foundVdd, 1);
    brightLABDVS = getLAB( brightRGBDVS );
    Orig2BrightDVSDist = distortionPercentage( LABorig, brightLABDVS);
    [PowBrightDVS, IcellBrightDVS] = powImgDVS( brightRGBDVS,  foundVdd);

    %Assignment 2, part 2. Contrast mode
    minDist = DVS_dist_target;
    for j = 0.025 : 0.025 : 2.0 % inner loop is for b
        % Compute brightness
        % Apply VDS 
        % Compute Power
        % Compute Distorsion
        [RGBcontrast, HSVcontrast ] = imgCompensation( RGBorig, j, 2 ); % mode 2: contrast
        [PowPanelContrast, IcellContrast] = powImgDVS( RGBcontrast, foundVdd ); % use the Vdd found above
        contrastRGBDVS = displayed_image(IcellContrast, foundVdd, 1);
        [contrastLABDVS] = getLAB( contrastRGBDVS );
        contrastDVSDist = distortionPercentage( LABorig, contrastLABDVS);
        if (contrastDVSDist < (double(DVS_dist_target)/2))
            foundContrB = j;
            break
        else if(contrastDVSDist < minDist)
            minDist = contrastDVSDist;
            foundContrB = j;
            end
        end
    end
    % compute again everything with the new value of B (foundContrB)
    [RGBcontrast, HSVbright ] = imgCompensation( RGBorig, foundContrB, 2 ); % mode 2: contrast
    [PowPanelBright, IcellContrast] = powImgDVS( RGBcontrast, foundVdd ); % use the Vdd found above
    contrastRGBDVS = displayed_image(IcellContrast, foundVdd, 1);
    contrastLABDVS = getLAB( contrastRGBDVS );
    Orig2ContrDVSDist = distortionPercentage( LABorig, contrastLABDVS);
    [PowContrastDVS, IcellContrastDVS] = powImgDVS( contrastRGBDVS,  foundVdd); 

    %Assignment 2, part 2. Combo = brightness + contrast modes
    minDist = DVS_dist_target;
    for j = 0.025 : 0.025 : 2.0     
        [RGBCombo, HSVCombo ] = imgCompensation( RGBorig, j, 1 ); % mode 1: brightness
        [RGBCombo, HSVCombo ] = imgCompensation( RGBCombo, j+1, 2 ); % mode 2: contrast
        [PowPanelCombo, IcellCombo] = powImgDVS( RGBCombo, foundVdd ); % use the Vdd found above
        comboRGBDVS = displayed_image(IcellCombo, foundVdd, 1);
        [comboLABDVS] = getLAB( comboRGBDVS );
        comboDVSDist = distortionPercentage( LABorig, comboLABDVS);
        if (comboDVSDist < (double(DVS_dist_target)/2))
            foundComboB = j;
            break
         else if(comboDVSDist < minDist)
            minDist = comboDVSDist;
            foundComboB = j;
             end
        end
    end
    % compute again everything with the new value of B (foundComboB)
    [RGBCombo, HSVCombo ] = imgCompensation( RGBorig, foundComboB, 1 ); % mode 1: brightness
    [RGBCombo, HSVCombo ] = imgCompensation( RGBCombo, foundComboB+1, 2 ); % mode 2: contrast
    [PowPanelCombo, IcellCombo] = powImgDVS( RGBCombo, foundVdd ); % use the Vdd found above
    comboRGBDVS = displayed_image(IcellCombo, foundVdd, 1);
    comboLABDVS = getLAB( comboRGBDVS );
    Orig2ComboDVSDist = distortionPercentage( LABorig, comboLABDVS);
    [PowComboDVS, IcellComboDVS] = powImgDVS( comboRGBDVS,  foundVdd); 

    % Perform power optimizations starting from compensated images
    [ RGBoptBrightness, newVddBrightness, PowBrightDVSOpt ] = optimizePower( RGBbright, foundVdd, DVS_dist_target, LABorig );
    [ RGBoptContrast, newVddContrast, PowContrastDVSOpt ] = optimizePower( RGBcontrast, foundVdd, DVS_dist_target, LABorig );
    [ RGBoptCombo, newVddCombo, PowComboDVSOpt ] = optimizePower( RGBCombo, foundVdd, DVS_dist_target, LABorig );
    
    % Compute all power savings
    Orig2DVSPowSav = PowSavings( PowPanelOrig, PowDVS  );
    Orig2BrightDVSPowSav = PowSavings( PowPanelOrig, PowBrightDVS  );     
    Orig2ContrDVSPowSav = PowSavings( PowPanelOrig, PowContrastDVS );     
    Orig2ComboDVSPowSav = PowSavings( PowPanelOrig, PowComboDVS );
    Orig2BrightDVSPowSavOpt = PowSavings( PowPanelOrig, PowBrightDVSOpt  );     
    Orig2ContrDVSPowSavOpt = PowSavings( PowPanelOrig, PowContrastDVSOpt );     
    Orig2ComboDVSPowSavOpt = PowSavings( PowPanelOrig, PowComboDVSOpt );

    % Print to file
    fprintf(results, '%s    %e    %e     %e     %e     %e\n', file.name, PowPanelOrig, PowDVS, PowBrightDVS, PowContrastDVS, PowComboDVS);
    
    fprintf(results, 'Filename    PowOrig      PowBrightDVSOpt    PowContrastDVSOpt    PowComboDVSOpt\n');
    fprintf(results, '%s    %e     %e     %e     %e\n', file.name, PowPanelOrig, PowBrightDVSOpt, PowContrastDVSOpt, PowComboDVSOpt);
    
    fprintf(results, 'Filename     Orig2DVSDist    Orig2BrightDVSDist     Orig2ContrDVSDist     Orig2ComboDVSDist\n');
    fprintf(results, '%s     %f    %f     %f     %f\n', file.name, Orig2DVSDist, Orig2BrightDVSDist, Orig2ContrDVSDist, Orig2ComboDVSDist);
    fprintf(results, 'Filename   Orig2DVSPowSav    Orig2BrightDVSPowSav     Orig2ContrDVSPowSav     Orig2ComboDVSPowSav\n');
    fprintf(results, '%s    %f    %f    %f    %f\n', file.name, Orig2DVSPowSav, Orig2BrightDVSPowSav, Orig2ContrDVSPowSav, Orig2ComboDVSPowSav);
    
    fprintf(results, 'Filename    Orig2BrightDVSPowSavOpt     Orig2ContrDVSPowSavOpt     Orig2ComboDVSPowSavOpt\n');
    fprintf(results, '%s    %f    %f    %f\n', file.name, Orig2BrightDVSPowSavOpt, Orig2ContrDVSPowSavOpt, Orig2ComboDVSPowSavOpt);
    fprintf(results, 'Filename  newVddBrightness   newVddContrast  newVddCombo \n');
    fprintf(results, '%s    %f   %f   %f\n', file.name, newVddBrightness, newVddContrast, newVddCombo  );
    
    fprintf(results, 'Filename   foundVdd    foundBrightB   foundContrB   foundComboB\n');
    fprintf(results, '%s    %f   %f   %f   %f\n\n\n\n', file.name, foundVdd, foundBrightB, foundContrB, foundComboB);
    
    % Subplotting figures
    subplot(1, 5, 1);
    subimage(RGBorig);
    subplot(1, 5, 2);
    subimage(RGBVnew);
    subplot(1, 5, 3);
    subimage(brightRGBDVS);
    subplot(1, 5, 4);
    subimage(contrastRGBDVS);
    subplot(1, 5, 5);
    subimage(comboRGBDVS);
    print(strcat('./imgcompar/', file.name),'-dpng');
    
    subplot(1, 4, 1);
    subimage(RGBorig);
    subplot(1, 4, 2);
    subimage(RGBoptBrightness);
    subplot(1, 4, 3);
    subimage(RGBoptContrast);
    subplot(1, 4, 4);
    subimage(RGBoptCombo);
    print(strcat('./imgcomparOpt/', file.name),'-dpng');
end