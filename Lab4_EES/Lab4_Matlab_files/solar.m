% This function is from
% http://www.mathworks.com/matlabcentral/fileexchange/31305-simple-solar-cell-and-panel-model
% This cell is of
% http://www.cs.wmich.edu/~sunseeker/files/A-300%20data%20sheet.pdf

function Ia = solar(Va,Suns,TaC)
% Ia,Va = current and voltage vectors [A] and [V]
% G = number of Suns [] (1 Sun = 1000 W/m2)
% T = temperature of the cell [C]
k = 1.38e-23; % Boltzmann constant [J/K]
q = 1.60e-19; % Elementary charge [C]
n = 1.2; % Quality factor for the diode []. n=2 for crystaline, <2 for amorphous
Vg = 1.12; % Voltage  of the Crystaline Silicon [eV], 1.75eV for Amorphous Silicon
T1 = 273 + 25; % Normalised temperature [K]

% Sunpower A300's values
Voc_T1 = 0.665; % Open-current voltage at T1 [V]. See SunpowerA300CellDatasheet.pdf
Isc_T1 = 0.575; % Short-circuit current at T1 [A]. See SunpowerA300CellDatasheet.pdf (Originally 5.75A)
%Isc_T1 = 0.04;
K0 = 3.5/1000; % Current/Temperature coefficient [A/K]. See SunpowerA300PanelDatasheet.pdf
dVdI_Voc = -0.00985; % dV/dI coefficient at Voc [A/V]. See SunpowerCurves.xlsx

TaK = 273 + TaC; % Convert cell's temperature from Celsius to Kelvin [K]
IL_T1 = Isc_T1 * Suns; % Compute IL depending the suns at T1. Equation (3)
IL = IL_T1 + K0*(TaK - T1); % Apply the temperature effect. Equation (2)
I0_T1 = Isc_T1/(exp(q*Voc_T1/(n*k*T1))-1); % Equation (6)
I0 = I0_T1*(TaK/T1).^(3/n).*exp(-q*Vg/(n*k).*((1./TaK)-(1/T1))); % Equation (5)
Xv = I0_T1*q /(n*k*T1) * exp(q*Voc_T1/(n*k*T1)); % Equation (8)
Rs = - dVdI_Voc - 1/Xv; %Compute the Rs Resistance. Equation (7)
Vt_Ta = n * k * TaK / q; % Equation (9)
Ia = zeros(size(Va)); %Initialize Ia vector
% Compute Ia with Newton method
for j=1:10;
    Ia = Ia - (IL - Ia - I0.*( exp((Va+Ia.*Rs)./Vt_Ta) -1))./(-1 - (I0.*( exp((Va+Ia.*Rs)./Vt_Ta) -1)).*Rs./Vt_Ta);
    Ia(Ia<0) = 0;
end

end