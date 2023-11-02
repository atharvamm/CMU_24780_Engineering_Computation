// Cessna 172 Climb Performance
#include <iostream>
#include <cmath>

// Function to calculate the rate of climb based on altitude and temperature
double rateOfClimb(int altLb,int altUb,int tempLb,int tempUb, int altTarget, int tempTarget, double table[]){
    double rateCur = 0.0;
    int altCur = 0;
    double rateUb = 0.0,rateLb = 0.0;

    // Calculate slopes for temperature interpolation
    double slopeTempUb = (table[4*altLb/2000 + (tempUb+20)/20] - table[4*altLb/2000 + (tempLb+20)/20])/(double)20;
    double slopeTempLb = (table[4*altUb/2000 + (tempUb+20)/20] - table[4*altUb/2000 + (tempLb+20)/20])/(double)20;
    
    // Interpolate rates based on temperature
    rateUb = table[4*altLb/2000 + (tempLb+20)/20] + slopeTempUb*(tempTarget - tempLb);
    rateLb = table[4*altUb/2000 + (tempLb+20)/20] + slopeTempLb * (tempTarget - tempLb);

    // Calculate slope for altitude interpolation
    double slopeAlt = (rateLb - rateUb) / (double)2000;

    // Interpolate the rate of climb based on altitude
    rateCur = rateUb + slopeAlt * (altTarget - altLb);
    return rateCur;
    }



int main(void)
{
	double table[]=
	{
		830.0,770.0,705.0,640.0,
		720.0,655.0,595.0,535.0,
		645.0,585.0,525.0,465.0,
		530.0,475.0,415.0,360.0,
		420.0,365.0,310.0,250.0,
		310.0,255.0,200.0,145.0
	};

    // Define arrays for temperature and altitude ranges
    int temperaturearr[4] = {-20,0,20,40};
    int altitudearr[6] = {0,2000,4000,6000,8000,10000};

    // Initialize variables
    int altTarget = -1,tempTarget = -20;
    double rateClimb = 0.0;
    bool condn1 = false, condn2 = false;

    while (true){
    // Take input from user for altitude and temperature
    std::cout << "Enter Altitude and Temperature: ";
    std::cin >> altTarget >> tempTarget;

    // Check if altitude and temperature are within valid ranges
    condn1 = (altTarget > -1) && (altTarget < 10001); 
    condn2 = (tempTarget > -21) && (tempTarget <  41);

    if (condn1 && condn2){
        break;
    }
    std::cout << "Temperature has to be in the range -20 to 40."<<std::endl;
    std::cout << "Altitude has to be in the range 0 to 10000."<<std::endl;
    }
    
    std::cout << "Current Altitude: " << altTarget << " and Temperature: "<< tempTarget <<std::endl;

    // Find lower and upper bounds for altitude and temperature
    int altLb = 0, altUb = 0;
    for (int altind = 0;altind < 5; altind++){
        if ((altitudearr[altind] <= altTarget) && (altitudearr[altind + 1] >= altTarget)){
            altLb = altitudearr[altind];
            altUb = altitudearr[altind + 1];
        }
    }
    
    int tempLb = 0, tempuB = 0;
    for (int tempind = 0; tempind < 5; tempind++){
        if ((temperaturearr[tempind] <= tempTarget) && (temperaturearr[tempind + 1]>= tempTarget)){
            tempLb = temperaturearr[tempind];
            tempuB = temperaturearr[tempind + 1];
        }
    }

    // Calculate the expected climb rate
    rateClimb = rateOfClimb(altLb,altUb,tempLb,tempuB,altTarget,tempTarget,table);

    std::cout << "Expected Climb Rate is: " << rateClimb << " ft/min" << std::endl;
    return 0;
}