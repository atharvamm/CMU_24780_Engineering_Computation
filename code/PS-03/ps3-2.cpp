// Cessna 172 Weight & Balance Checker
#include <iostream>
#include <string>
#include "fssimplewindow.h"
#include <cmath>

// Function to display a window with weight and momentum visualization
int displayWindow(int polygons[5][2],float finalWeight,float finalMomentum){
    // Open an OpenGL window and set up initial parameters
    FsOpenWindow(50,50,800,600,0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    float x = 0.0,y = 0.0;

    // Adjust finalMomentum and finalWeight for display purposes
    finalMomentum = ((finalMomentum - 50) * 5.0);
    finalWeight = (600 - ((finalWeight - 1500) * 0.6));

    // Enter an interactive loop for OpenGL window
    while(true){
        FsPollDevice();
        auto key = FsInkey();
        if (key == FSKEY_ESC){
            break;
        }
        
        // Draw the weight and balance polygons
		glBegin(GL_LINE_LOOP);

        for (int i = 0; i < 5; i++){
            std::cout << polygons[i][0] << " " << polygons[i][1];
            x = ((polygons[i][0]- 50)*5.0);
            y = 600 - ((polygons[i][1] - 1500 )*0.6);
            glVertex2f(x,y);
        }
        glEnd();
        // std::cout << "Co-Ordinates Weight and Momentum: " << finalWeight << " " << finalMomentum<<std::endl;
        // Draw a point to represent the final weight and momentum
        glPointSize(4);
        glBegin(GL_POINTS);
        glVertex2f(finalMomentum,finalWeight); 
		glEnd();    

        // Flush the graphics buffer and pause for a second
		glFlush();
		FsSleep(1000);
    }

    return 0;
}

int main(void){
    // Initialize variables for input and calculations
    int frontSeat = 0, rearSeat = 0, bagArea1 = 0, bagArea2 = 0, fuelGallons = 0, totalWeight = 0, base_weight = 1639;
    float totalMomentum = 0,base_momentum = 64.4;
    std::cout << "Please input weight details when requested: " << std::endl;
    
    std::cout << "Front Seat Weight: ";
    std::cin >> frontSeat;

    std::cout << "Rear Seat Weight: ";
    std::cin >> rearSeat;

    std::cout << "Bag Area 1 Weight: ";
    std::cin >> bagArea1;

    std::cout << "Bag Area 2 Weight: ";
    std::cin >> bagArea2;

    std::cout << "Fuel in Gallons: ";
    std::cin >> fuelGallons;

    // Calculate weights and momentums
    int weights[8] = {6*fuelGallons, frontSeat, rearSeat, bagArea1, bagArea2};
    weights[6] = -7;
    
    for (int i = 0; i < 6; i++){
        totalWeight = totalWeight + weights[i];
    }

    totalWeight = totalWeight + base_weight;
    weights[5] = totalWeight;
    weights[7] = weights[5] + weights[6];

    float momentums[8] = {48, 37, 73, 95, 123, 0, 48};
    for (int i = 0;  i < 6; i++){
        momentums[i] = (momentums[i] * weights[i])/(float)1000;
        totalMomentum = totalMomentum + momentums[i];
    }
    totalMomentum = totalMomentum + base_momentum;
    momentums[5] = totalMomentum;
    momentums[6] = (momentums[6]*weights[6])/(float)1000;
    momentums[7] = momentums[5] + momentums[6];

    // Set final weight and momentum
    float finalWeight = 0.0,finalMomentum = 0.0;
    finalWeight = weights[7];
    finalMomentum = momentums[7];
    std::cout << "Final Weight and Momentum: " << finalWeight << " " << finalMomentum;

    // Define labels and polygons for weight and balance visualization
    std::string labels[8] = {"Fuel in Gallons", "Front Seat", "Rear Seat","Bag Area 1","Bag Area 2","Ramp Weight", "Take Off Weight"};
    int polygons[5][2] = {
        {52,1500},
        {67,1950},
        {97,2450},
        {116,2450},
        {71,1500}
        };

    // Call the function to display the weight and balance visualization
    displayWindow(polygons,finalWeight,finalMomentum);

    return 0;
}