function increased = increasing( Load_current_profile )
% This function was created to solely test the dcdc converter's efficiency.
% It assumes the input vector to have 2000 elements and a starting value
% of 144mA for the current.
% It's not called by any function/script but is only used to manually
% obtain an increasing (by steps) current vector.
    count = 100;
    step = 0.01;
    amt = 0.144;
    for i = 1:19
        amt = amt + step;
        Load_current_profile(count:count+100,2) = amt;
        count = count + 100;
        step = step + 0.0001;
    end
    increased = Load_current_profile

end

