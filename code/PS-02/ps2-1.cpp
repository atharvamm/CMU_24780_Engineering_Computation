#include <iostream>
#include <ctime>    // Include for time functions
#include <string>   // Include for strings
#include <cstdlib> // Include for rand functions

// Function to shuffle an array of strings
void shuffledIndices(std::string shuffle[], int n) {
    srand(time(NULL));  // Seed the random number generator with the current time
    int randn = 0;
    std::string temp = "";

    // Shuffle the array
    for (int i = 0; i < n; i++) {
        randn = rand() % n;  // Generate a random index

        // Swap the current element with a randomly chosen element
        temp = shuffle[i];
        shuffle[i] = shuffle[randn];
        shuffle[randn] = temp;
    }
}

int main() {
    std::string lunch[7] = {"Pasta", "Pav Bhaji", "Sandwich", "Pulao", "Burrito", "Misal Pav", "Rice and Curry"};
    std::string days[7] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};

    // Shuffle the lunch options
    shuffledIndices(lunch, 7);

    // Display the shuffled lunch options for each day
    for (int i = 0; i < 7; i++) {
        std::cout << days[i] << "'s Lunch: " << lunch[i] << std::endl;
    }

    return 0;
}
