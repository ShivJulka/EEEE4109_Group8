classdef DistanceCalculation < matlab.System
    % untitled Add summary here
    %
    % This template includes the minimum set of functions required
    % to define a System object with discrete state.

    % Public, tunable properties
    properties
    end

    properties(DiscreteState)
    end

    properties(Access = protected)
        
    end

    % Pre-computed constants
    properties(Access = private)
        Position;
        DistanceTravelled;

    end

    methods(Access = protected)
        function setupImpl(obj)
            % Perform one-time calculations, such as computing constants            
            obj.Position = 1;
            obj.DistanceTravelled = 0;
        end

        function P = stepImpl(obj,VehicleDistance,NextCoordDistance)
            
            if(VehicleDistance > 10)
               
                Temp = NextCoordDistance + obj.DistanceTravelled;

                if(VehicleDistance >= Temp)

                    obj.DistanceTravelled = Temp;
                    obj.Position = obj.Position + 1 ;
                end
            end
            P = obj.Position ;
            
        end

        function resetImpl(obj)        
            obj.Position = 1;
            obj.DistanceTravelled = 0;
        end
    end
end
