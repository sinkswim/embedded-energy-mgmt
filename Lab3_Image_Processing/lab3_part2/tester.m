Vnew = 13;
[RGBorig, R, G, B, LABorig, L, a, b, HSVorig ] = getImgData( './images/4.2.05.tiff' );
[PowPanelOrig, IcellOrig] = powImgDVS( RGBorig, 13 );% it was 15
RGBV13 = displayed_image(IcellOrig, Vnew, 1);
[RGBbright, HSVbright ] = imgCompensation( RGBorig, 0.25, 1 );
[PowPanelBright, IcellBright] = powImgDVS( RGBbright, 13 );% it was 15
brightRGBV13 = displayed_image(IcellBright, Vnew, 1);
[LABV13] = getLAB( RGBV13 );
[brightLABV13] = getLAB( brightRGBV13 );
subplot(1,3,1)
subimage(RGBorig)
subplot(1,3,2)
subimage(RGBV13)
subplot(1,3,3)
subimage(brightRGBV13)
PowPanelOrig
[PowPanel13, Icell13] = powImgDVS( RGBV13, Vnew );
PowPanel13
[PowPanelbright13, Icellbright13] = powImgDVS( brightRGBV13, Vnew );
PowPanelbright13
OrigToV13Dist = distortionPercentage(LABorig, LABV13)
OrigToV13BrightDist = distortionPercentage(LABorig, brightLABV13)