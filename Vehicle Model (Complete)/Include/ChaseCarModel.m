%WIFI INPUTS
%
%battery power
%
%
%EQUATION FUNCTION
%%%calculate current speed
%%%distance between each 
%
%
%
clc;
clear;
speed=20; %hard code speed to 20m/s for now



%%the route with all of the coordinates
data = load('LonLat.mat');
player = geoplayer(data.A(1),data.B(1),10);
plotRoute(player,data.A,data.B);

%set inital variables for start position
total_distance=0;
time=1;
grade=0;

%loop through all the coordinates to show the path
for i = 1:length(data.A)
    
    %get current elevation
    temp = txsite('Latitude',data.A(i),'Longitude',data.B(i));
    z = elevation(temp);

    %temp, air pressure, water vapour density
    [t,p,wvden] = atmositu(z,'LatitudeModel','Mid','Season','Summer');
    %atmositu(z,'Latitude','Mid','Season','Winter');

    [Monthly,Hourly,Current]=SolarModel(data.A(i),p);
    
    plotPosition(player,data.A(i),data.B(i));

    %to calculate the distance between the current and previous coordinates
    %can then calculate total distance from it
    if i>1
        d=distance(data.A(i-1),data.B(i-1),data.A(i),data.B(i));
        total_distance=total_distance+d;
        time=d/speed;
        grade=incline(data.A(i-1),data.B(i-1),data.A(i),data.B(i),d);
    end

    %display current locations solar irradiance, air pressure, temperature,
    %water density, the total distance travelled and the gradient
    Display(Current,p,t,wvden,total_distance,grade);

    pause(time)

end

%
%
%function to calculate the incline on the route
%
function grade=incline(lat1,long1,lat2,long2,run)
    %get first coordinates elevation
    position1 = txsite('Name','Mt Washington','Latitude',lat1,'Longitude',long1);
    z1 = elevation(position1);
    
    %elevation of coordinate 2
    position2 = txsite('Name','Mt Washington','Latitude',lat2, ...
       'Longitude',long2);
    z2 = elevation(position2);
    
    %calculate rise and therefore the gradient of the 
    rise=z2-z1;
    %disp(rise);
    grade=rise/run;
    %disp(grade);

end



%
%%Distance between two coordinates
function d=distance(lat1,long1,lat2,long2)
    %to radians
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


%
%   SOLAR MODEL
%
%   INPUTS: - Latitude in degrees
%           - Airpressure in the Altitude considered in Pascal (Pa)
%
%   OUTPUT: - Monthly Average Irradiation (W/m²)
%           - Daily Average Irradiation (W/m²)
%           - current day irradiation (W/m²)
%
function [Monthly,Hourly,Current]=SolarModel(Latitude, airpressure)
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

            if DayOfYear <= 31
                Month(1,1) = Month(1,1)+Irradiance_Wm2;
            elseif DayOfYear >=32 && DayOfYear <=59
                Month(1,2) = Month(1,2)+Irradiance_Wm2;
            elseif DayOfYear >=60 && DayOfYear <=90
                Month(1,3) = Month(1,3)+Irradiance_Wm2;
            elseif DayOfYear >=91 && DayOfYear <=120
                Month(1,4) = Month(1,4)+Irradiance_Wm2;
            elseif DayOfYear >=121 && DayOfYear <=151
                Month(1,5) = Month(1,5)+Irradiance_Wm2;
            elseif DayOfYear >=152 && DayOfYear <=181
                Month(1,6) = Month(1,6)+Irradiance_Wm2;
            elseif DayOfYear >=182 && DayOfYear <=212
                Month(1,7) = Month(1,7)+Irradiance_Wm2;
            elseif DayOfYear >=213 && DayOfYear <=243
                Month(1,8) = Month(1,8)+Irradiance_Wm2;
            elseif DayOfYear >=244 && DayOfYear <=273
                Month(1,9) = Month(1,9)+Irradiance_Wm2;
            elseif DayOfYear >=274 && DayOfYear <=304
                Month(1,10) = Month(1,10)+Irradiance_Wm2;
            elseif DayOfYear >=305 && DayOfYear <=334
                Month(1,11) = Month(1,11)+Irradiance_Wm2;
            elseif DayOfYear >=335 && DayOfYear <=365
                Month(1,12) = Month(1,12)+Irradiance_Wm2;
            end
    end
    Month(1,1)=Month(1,1)/31;
    Month(1,2)=Month(1,2)/28;
    Month(1,3)=Month(1,3)/31;
    Month(1,4)=Month(1,4)/30;
    Month(1,5)=Month(1,5)/31;
    Month(1,6)=Month(1,6)/30;
    Month(1,7)=Month(1,7)/31;
    Month(1,8)=Month(1,8)/31;
    Month(1,9)=Month(1,9)/30;
    Month(1,10)=Month(1,10)/31;
    Month(1,11)=Month(1,11)/30;
    Month(1,12)=Month(1,12)/31;
    Monthly=Month;
    Hourly=Hourly/365;
end


%
%Display function
% %
function Display(Hourly,p,T,wvden,distance,grade)
%     Hourly=2;
%     figure
%     hold on
%     str = {"Solar: "+Hourly , "Air Pressure: "+p,...
%         "Temperature: "+T , "Water Vapour Density: "+wvden,...
%         "Total Distance: "+distance};
%     text(2,7,str)
%     refresh
% 

%     t=annotation('textbox');
%     str=["Solar: ",num2str(Hourly), "\n Air Pressure: ",num2str(p),...
%         "\nTemperature: ",num2str(T), "\nWater Vapour Density: ",num2str(wvden),...
%         "\nTotal Distance: ",num2str(distance)];
%     t.FitBoxToText='off';
%     t.EdgeColor='Red';
%     t.Position=[1,0,0.5,0.6];
%     t.String=str;
%     refresh(t)
end
