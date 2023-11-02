// Include required libraries
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <chrono>

void shuffling(int shuffle[], int n) {
    srand(time(NULL));
    int randn = 0;
    int temp = 0;

    for (int i = 0; i < n; i++) {
        randn = rand() % n;
        temp = shuffle[i];
        shuffle[i] = shuffle[randn];
        shuffle[randn] = temp;
    }
}

void generateArray(int array[], int n) {
    int index = 0;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            array[index++] = 100 * i + j;
        }
    }
    std::cout << array[index - 1] << std::endl;
}

int main() {
    int arraySize = 10;
    int numCorrect = 0;
    int flashcards[1000];
    double perCorrect = 0.0;
    int flashNum = 0;
    bool condn = true;
    int num1 = 0, num2 = 0, answer = 0;
    int maxNum,sqNum,product;

    std::cout << "What is the maximum multiplication table range you want to practice ?: ";
    std::cin >> maxNum;
    sqNum = maxNum * maxNum;

    generateArray(flashcards, maxNum);
    shuffling(flashcards, sqNum);

    while (condn) {
        std::cout << "How many flashcards would you like to practice ?: ";
        std::cin >> flashNum;
        condn = flashNum < 1 || flashNum > sqNum;
        if (condn) {
            std::cout << "The number of cards must be between 1 and " << sqNum;
        }
    }

    auto startTime = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < flashNum; i++) {
        num2 = flashcards[i] % 100;
        num1 = (flashcards[i] - num2) / 100; // num2 is always greater than 0
        product = num1 * num2;
        std::cout <<"Q."<<(i+1)<< ".What is " << num1 << " X " << num2 << ": ";
        std::cin >> answer;
        if (answer == product) {
            std::cout << "Correct!!!" << std::endl;
            numCorrect++;
        } else {
            std::cout << "Incorrect, correct answer was: " << product << std::endl;
        }
    }
    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> timeRun = endTime - startTime;

    perCorrect = (numCorrect / (double)flashNum) * 100; // flashNum is greater than 0
    std::cout << "You answered " << flashNum << " questions in " << timeRun.count() << "s. Of these " << numCorrect << " (" << perCorrect << "%) were correct." << std::endl;
    return 0;
}
