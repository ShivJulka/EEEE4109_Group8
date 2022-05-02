classdef RaceStrategy < matlab.System
    % untitled Add summary here
    %
    % This template includes the minimum set of functions required
    % to define a System object with discrete state.

    % Declare Public Variables (the ones you need to change from simulation)
    properties
        
        SimulationTime = 2;
    end

    properties(DiscreteState)
       
        % Private Variables
        drivetrain;
    end

    properties(Access = protected)
        
    end

    % Pre-computed constants
    properties(Access = private)

    end

    methods(Access = protected)
        function setupImpl(obj)
            % Start Up function 
            obj.drivetrain = 0.97317;
        end

        function RecommendedSpeed = stepImpl(obj,Lattitude,p,speed,windVelocity,slope,batteryEnergy,remaining)
            
     
            %power that is driving the wheels
            temp=((800*speed/obj.SimulationTime) + (0.5*0.25*1.8*p*(speed-windVelocity)^2) + (0.006*800*9.81) + (800*9.81*sin(slope)));
            Pm=speed*temp;

            %equations for battery storage 
    
            nb=xx;
    
            if Ps>=Pm
                Eb=sqrt(nb)*(Ps-Pm);
            else
                Eb=(1/sqrt(nb)) * (Ps-Pm);
            end
    
            batteryEnergy=batteryEnergy+Eb;

            %equations to calculate how far the car can travel at this speed
            %
            v=(-batteryEnergy*sqrt(nb) + Ps)/temp;
            distance=v*t; %t is the time left to travel 
            remaining=remaining-distance;
            if remaining >1000
                RecommendedSpeed="faster";
            elseif remaining <200
                RecommendedSpeed="slower";
            else
                RecommendedSpeed="constant";
            end 

        end

        function resetImpl(obj)

            % Initialize / reset discrete-state properties
            
            obj.CurrentBatteryEnergy = (obj.initial_Battery_SOC/100)* (obj.BatteryVoltage *obj.BatteryRating_AH * 3600);
            obj.MaxBatteryEnergy = (1)* (obj.BatteryVoltage *obj.BatteryRating_AH * 3600);                  %% in joules
            obj.BatterySOC = obj.initial_Battery_SOC ;          %% battery state of charge steup
        end
    end
end
