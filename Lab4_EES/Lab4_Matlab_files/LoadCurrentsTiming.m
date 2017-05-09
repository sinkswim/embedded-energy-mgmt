function [ Load_current_profile ] = LoadCurrentsTiming( input_load_vec, hours )
% Given input_load_vec and hours creates a new matrix Load_current profile
% with: 1) col1 - time in seconds;
%       2) col2 - current
%timings = zeros(size(input_load_vec));
tmp = size(input_load_vec);
step = hours*3600/tmp(1);
timings = 0 : step : (hours*3600)-step;
Load_current_profile = cat(2, timings', input_load_vec);

end

