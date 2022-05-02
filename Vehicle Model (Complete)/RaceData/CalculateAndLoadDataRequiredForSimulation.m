%% Script used to preload values when opening the simulink model of the vehicle
% The aim of this script is to load longitude and latitude values of the race route and using
% those values to calculate Air pressure, Temperature, Irrdiance, Water vapour density etc


% Load File which contains Longitude and latitude values of the route
PositionData = load('LonLat.mat');

% get longitude and latitudnal values and save them in separate variables
Latitude = PositionData.A;
Longitude = PositionData.B;

% Declare Data arrays to save calculated data 
NextCoordDistance=zeros(1,length(Longitude));      %save distance value in meters between current and next point
Temperature=zeros(1,length(Longitude));            %save temperature values
Airpressure=zeros(1,length(Longitude));             %save air pressure value
WVDensity=zeros(1,length(Longitude));               %save water vapour density value
Irradiance=zeros(1,length(Longitude));              % store calculated value of Irradiance
Incline=zeros(1,length(Longitude));                 % Save Incline Value

%Declare Temporary variables
CurrentPos = 0;
NextPos = 0;

%create a for loop and get data for each lon and latitude value and save
%them in their separate data variables
for i = 1:length(Longitude)
    
    %calculate and store temperature, airpressure and water vapour density
    %values
    [Temperature(i),Airpressure(i),WVDensity(i)]=GetEnvironmentParameters(Latitude(i),Longitude(i));

    %Calculate Irradiance value
    Irradiance(i) = GetIrradianceValue(Latitude(i),Airpressure(i));

    % check if the next coordinate is not the last coordinate than
    % calculate distance otherwise store next coordinate distance of zero
    if(i==length(Longitude))
        NextCoordDistance(i) = 0;
        NextPos = 0 ;
    else
        NextCoordDistance(i) = distance(Latitude(i),Longitude(i),Latitude(i+1),Longitude(i+1));
        NextPos = GetCoordElevationValue(Latitude(i+1),Longitude(i+1));
    end

    
    %Calculate Incline
    CurrentPos = GetCoordElevationValue(Latitude(i),Longitude(i));
    Incline(i) = CalculateIncline(CurrentPos,NextPos,NextCoordDistance(i));

end

%Save calculated values into a matlab Script
matlab.io.saveVariablesToScript('SavedVariablesForSimulation.mat',{'Latitude','Longitude', ...
    'NextCoordDistance','Temperature','Airpressure','WVDensity','Irradiance','Incline'});


%function to calculate incline between two points
function Incline = CalculateIncline(NextPos, CurrentPos, DistanceBetween2Coord)

    %calculate rise and therefore the gradient of the 
    rise=NextPos-CurrentPos;
    
    %calculate Incline
    Incline = rise/DistanceBetween2Coord;

end


%%function to calculate Distance between two coordinates
function d=distance(lat1,long1,lat2,long2)
    %converting to radians
    lat1=lat1*pi/180;
    lat2=lat2*pi/180;
    long1=long1*pi/180;
    long2=long2*pi/180;


    R=6371*1000; %Radius of the earth
    deltaLon=(long2-long1);
    deltaLat=(lat2-lat1);
    
    a=(sin(deltaLat/2))^2 + cos(lat1)*cos(lat2)*(sin(deltaLon/2)^2);
    c=2*atan2(sqrt(a),sqrt(1-a));
    d=R*c; %distance in meters
    %disp(d);
end



% function to get the elevation value of a position given the latitude and
% longitude values
function f = GetCoordElevationValue(Latitude,Longitude)
    position = txsite('Name','Mt Washington','Latitude',Latitude,'Longitude',Longitude);
    f = elevation(position);
end


%% Function to calculate temperature, air pressure and water vapour density
function [t,p,wvden] = GetEnvironmentParameters(Latitude,Longitude)
    
    %get current elevation
    temp = txsite('Latitude',Latitude,'Longitude',Longitude);
    ElevatedValue = elevation(temp);

    [t,p,wvden] = atmositu(ElevatedValue,'LatitudeModel','Mid','Season','Summer');
end

% function to calculate irradiance 
function Irr = GetIrradianceValue(Latitude,airpressure)
    

    %get current date
    t=datetime('today');
    Date=day(t,'dayofyear');
    
    %define parameters
    Month=zeros(1,12);       
    p=airpressure; 
    Hourly=0;
    
    for DayOfYear=1:365
            t=0.75; % transmittance (unitless)
            S=1367; %solar constant (w/m^2)
            hours=[7,8,9,10,11,12,13,14,15,16,17];
            hangle=(12.0-hours)*15.0*pi/180.0; % Note pi/180.0 is the factor to convert angles in degrees in radiances.
            declangle=23.45*sin(2.0*pi*(284.0+DayOfYear)/365.0)*pi/180.0;
            cosz=sin(Latitude*pi/180.0)*sin(declangle)+cos(Latitude*pi/180.0)*cos(declangle)*cos(hangle);
            m=p./(101.3*cosz); %optical airmass
            Sb=cosz*S.*(t.^m); % Sb is the beam radiation on a horizontal surface
            Sd=0.3*(1.0-t.^m)*S.*cosz; % Sd is the diffuse radiation on a horizontal surface
            St=Sb+Sd; % St is the total radiation;
            
            Hourly=(Hourly+St);
            Irradiance_Wm2 = mean(St);
            %Jan = 1 - 31 (31)
            %Feb = 32 - 59 (28)
            %Mar = 60 - 90 (31)
            %Abr = 91 - 120 (30)
            %May = 121 - 151 (31)
            %Jun = 152 - 181 (30)
            %Jul = 182 - 212 (31)
            %Aug = 213 - 243 (31)
            %Sep = 244 - 273 (30)
            %Oct = 274 - 304 (31)
            %Nov = 305 - 334 (30)
            %Dec = 335 - 365 (31)
            if Date==DayOfYear
                Current=Irradiance_Wm2;
            end
    end
    %send Current solar irrdiance 
    Irr = Current;

end
