function [ load_currents ] = generateLoadCurrents( dist_type, parameters, n )
% Generates a load current profile based on the inputs:
% dist_type: Type of distribution. 0: Constant current, 1: Bimodal distribution, 2: Uniform distribution
% parameters of the distribution
% n: number of values

% Examples --
% constant current: generateLoadCurrents(0, 2.55, 2000)
% bimodal: generateLoadCurrents(1, [1.275, 3.825, 0.1], 2000)
% uniform: generateLoadCurrents(2, [0.1, 5], 2000)

switch dist_type
    case 0  % constant current
        if(size(parameters) ~= 1)
            errordlg('Please provide 1 parameter for constant current profiling.');
            return
        else
            load_currents = repmat(parameters(1), 1, n);
            load_currents = load_currents';
            hist(load_currents, 100);
            title(sprintf('constant value=%f A',parameters(1)));
            return
        end
    case 1  % bimodal
        if(size(parameters) ~= 3)
            
            errordlg('Please provide 3 parameters for bimodal distribution profiling.');
            return
        else
            m1 = parameters(1); % first mean
            m2 = parameters(2); % second mean
            ratio = 0.5; % weight of the first distribution
            % ratio = parameters(3);
            % x = [randn(n*ratio,1) + m1; randn(n*(1-ratio),1) + m2];
            x = [parameters(3).*randn(n*ratio,1) + m1; parameters(3).*randn(n*(1-ratio),1) + m2];
            load_currents = x(randperm(numel(x)));
            hist(load_currents, 100);
            title(sprintf('mean=%f, median=%f, std dev=%f',mean(load_currents),median(load_currents), parameters(3)))
            return
        end
    case 2  % uniform
        if(size(parameters) ~= 2)
            errordlg('Please provide 2 parameters for uniform distribution profiling.');
            return
        end
        if(parameters(1) > parameters(2))
            errordlg('Please provide the minimum and maximum as parameters(1) and (2) respectively.');
            return
        else
            load_currents = (parameters(2)-parameters(1))*rand(n,1) + parameters(1);
            hist(load_currents, 100);
            title(sprintf('minimum=%f A, maximum=%f A',parameters(1), parameters(2)));
            return
        end
    otherwise
        errordlg('Wrong input: accepted distribution types are 0, 1 and 2');
        return
    end
end

