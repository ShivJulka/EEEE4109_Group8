%%This is the function that will run the genetic algorithm for the
%%optimization.
%This will be run on a simulink block within the model however it is easier
%to assess and review the code by uploading this individually
function  target=optimization(remaining,u)
    % Motor Parameters
    MotorEfficiency = 98.3;  % percent
    PeakTorque = 80;         %Nm
    MinimumDriveEfficiency = 94 ;     % percent

    % Wheel Parameters
    MotorDiameter = 0.38;        % meters
    TyreRimDiameter = 0.0762;    % meters


    %convert distance to meters
    remaining=remaining*1000;
    %create a 2d array of the times stores array as time then speed
    times=zeros(25,2);
    
    %create a 2d array of the driving power stores array as time then speed
    power=zeros(25,2);
    
    cost=zeros(25,2);

    r=1;
    %loop through range of speeds based on the speed limits of Austrailia
    for i=12:36
        %%%%%Time to next checkpoint
        time=remaining/i;
        
        %%%%%%%power that is driving the wheels
        % Torque Driven to Motors
        MotorTorque = PeakTorque * u;
        % Total Diameter of Wheel
        WheelDiameter = MotorDiameter+TyreRimDiameter;
        % motor Speed m/s
        MSSpeed = i/3.6;
        % RPM
        RPMSpeed = 9.55 * (MSSpeed/(WheelDiameter/2));
        % rad/s
        WSpeed = (RPMSpeed/60) * (2*3.14159);
        % Motor Drive Efficiency at Motor Torque
        DriveEfficiency = MinimumDriveEfficiency + (WSpeed*0.1);
        % Power Driven to the Motors [W]
        PowerDriven =2 * (MotorTorque * RPMSpeed/9.55 );
        % Power Lost in Motor [W]
        PowerDissM = PowerDriven * ((100-MotorEfficiency)/100);
        % Power Lost in Motor Drive [W]
        PowerDissMD = PowerDriven * ((100-DriveEfficiency)/100);
        % Total Power Loss in Drive & Motor
        TotalPowerDiss = PowerDissMD + PowerDissM;
        % Total Power Out
        Pout = PowerDriven - TotalPowerDiss;
        
        
        
        cost(r,2)=i;
        cost(r,1)=(time/10) +Pout;
        
        
        r=r+1;
        
        
        
    end
    
    %%%
    %Perform bubble sort on the cost array to get column 1 in order
    %%%Will sort array so that the lowest cost is shown first then the
    %%%rest will be shown in ascending order
    % Traverse through all array elements
    for i=1:20
    % range(n) also work but outer loop will
    % repeat one time more than needed.
        % Last i elements are already in place
        for j=1:19
            % traverse the array from 0 to n-i-1
            % Swap if the element found is greater
            % than the next element
            if cost(j,1) > cost(j+1,1)
                temp1=cost(j,1);
                temp2=cost(j,2);
                cost(j,1)=cost(j+1,1);
                cost(j,2)=cost(j+1,2);
                cost(j+1,1) = temp1;
                cost(j+1,2) = temp2;
            end
        end
    end
    
    disp(cost);
    %the first item in the second column will be the ideal speed
    target=cost(1,2);
    
end
