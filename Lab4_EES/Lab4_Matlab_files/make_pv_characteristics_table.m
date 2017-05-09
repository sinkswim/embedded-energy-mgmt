clear all

%% Load daily solar irradiation profile
load('example_variable_set.mat');   
G_vec = [min(G_log(:,2)):(max(G_log(:,2)) - min(G_log(:,2)))/49:max(G_log(:,2))]/1000;
G_simulation_input = G_log;
G_simulation_input(:,2) = G_simulation_input(:,2)/1000;

%% Generate a table as Ipv = table(G, Vpv)
Vpv_min = 0;
Vpv_max = 0.65;
Vpv_vec = [Vpv_min:(Vpv_max - Vpv_min)/99:Vpv_max];
for i=1:length(Vpv_vec)
    Ipv(:,i) = solar(Vpv_vec(i), G_vec, 25);
    Ppv(:,i) = Vpv_vec(i) * Ipv(:,i); % get Ppv = table(G, Vpv)
end

%% Generate a table as Ipv = table(G, Ppv) by using interp1(). 
%% Please use the smaller value before power knee point. 
Ppv_min = min(min(Ppv(:,:)));
Ppv_max = max(max(Ppv(:,:)));
Ppv_vec = [Ppv_min: (Ppv_max - Ppv_min)/99: Ppv_max];

for i=1:length(G_vec)
    % to use smaller value before the power knee point.
    Ipv_max_idx = find(Ppv(i,:)== max(Ppv(i,:)), 1, 'last');
    %implement here
    Ppv_to_Ipv(i,:) = interp1(Ppv(i,1:Ipv_max_idx), Ipv(i,1:Ipv_max_idx), Ppv_vec);
 
end
Ppv_to_Ipv(find(Ppv_to_Ipv < 0)) = 0;
Ppv_to_Ipv(isnan(Ppv_to_Ipv)) = 0;
surf(Ppv_to_Ipv)
zlabel('Current (A)')
ylabel('Irradiance (W/m^2)')
xlabel('Power (W)')
