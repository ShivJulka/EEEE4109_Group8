clear;
clc;
%% Step Size
Max_StepSize = 0.0001;

%% Const Temperature for solar penals
Solar_Temperature = 25;

%% Vehicle Parameters
rider_mass = 70;     %kg
vehicle_mass = 100;  %kg

CG_height = 2.5;     %m
drag_coeff = 0.05;
front_axle = 1.0;   %m
rear_axle = 1.0;    %m
front_area = 0.5;   %m^2
Air_density = 1 ;    %kg/m^3
Gravity = 9.81 ;

TotalVehicleMass = vehicle_mass + rider_mass;

%% Tire Parameters
Rolling_Radius = 0.6;       %m
tire_inertia = 2e-3; %kg*m^2
roll_resist = 0.005;

%% Battery Parameters


%% Load Variables for simulation
Data = load('./RaceData/SavedVariablesForSimulation.mat');
Airpressure = Data.Airpressure;
Incline = Data.Incline;
Irradiance = Data.Irradiance;
Latitude = Data.Latitude;
Longitude = Data.Longitude;
NextCoordDistance = Data.NextCoordDistance;
Temperature = Data.Temperature;
WVDensity = Data.WVDensity;





