classdef BatteryFunction < matlab.System
    % untitled Add summary here
    %
    % This template includes the minimum set of functions required
    % to define a System object with discrete state.

    % Public, tunable properties
    properties
        initial_Battery_SOC = 0;  
        BatteryVoltage = 12;
        BatteryRating_AH = 29600 ;
        TimeStep = 0.01;
    end

    properties(DiscreteState)
       
        CurrentBatteryEnergy;
        MaxBatteryEnergy;
        BatterySOC;
    end

    properties(Access = protected)
        
    end

    % Pre-computed constants
    properties(Access = private)

    end

    methods(Access = protected)
        function setupImpl(obj)
            % Perform one-time calculations, such as computing constants 
            obj.MaxBatteryEnergy = obj.BatteryVoltage * obj.BatteryRating_AH * 3600;                   %% in joules
            obj.BatterySOC = obj.initial_Battery_SOC ;          %% battery state of charge steup

            %Check the inital value of battery assigned
            if(obj.initial_Battery_SOC < 0)
                obj.CurrentBatteryEnergy = 0 ;
            else 
                if(obj.initial_Battery_SOC > 100)
                    obj.CurrentBatteryEnergy = obj.MaxBatteryEnergy ;
                else
                    obj.CurrentBatteryEnergy = (obj.initial_Battery_SOC * obj.BatteryVoltage *obj.BatteryRating_AH * 3600)/100;
                end
            end
        end

        function [CurrentStoredEnergy,MaxBatteryCapacity,SOC] = stepImpl(obj,SolarInput,MotorsPow)
            % Implement algorithm. Calculate y as a function of input u and
            % discrete states.
            
            obj.CurrentBatteryEnergy = obj.CurrentBatteryEnergy + obj.TimeStep*(SolarInput - MotorsPow);

            if(obj.CurrentBatteryEnergy > obj.MaxBatteryEnergy)
                  obj.CurrentBatteryEnergy = obj.MaxBatteryEnergy ;
            end
                

            %calculate battery Percentage
            obj.BatterySOC = 100*(obj.CurrentBatteryEnergy / obj.MaxBatteryEnergy) ;
            if(obj.BatterySOC>100)
                 obj.BatterySOC =100 ;
            end
            if(obj.BatterySOC<0)
                 obj.BatterySOC = 0 ;
            end
                
       

            SOC = obj.BatterySOC ;
            CurrentStoredEnergy =obj.CurrentBatteryEnergy ;
            MaxBatteryCapacity = obj.MaxBatteryEnergy;
            
        end

        function resetImpl(obj)

            % Initialize / reset discrete-state properties
            
            obj.CurrentBatteryEnergy = (obj.initial_Battery_SOC/100)* (obj.BatteryVoltage *obj.BatteryRating_AH * 3600);
            obj.MaxBatteryEnergy = (1)* (obj.BatteryVoltage *obj.BatteryRating_AH * 3600);                  %% in joules
            obj.BatterySOC = obj.initial_Battery_SOC ;          %% battery state of charge steup
        end
    end
end
