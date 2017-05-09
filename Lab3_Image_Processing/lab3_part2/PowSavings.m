function [ PowSavePerc ] = PowSavings( PowOrig, PowNew )
% Returns percentage of power savings
PowSavePerc = ((PowOrig - PowNew)./PowOrig).*100;
end

