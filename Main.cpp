#include <iostream>         // Cout, Cin
#include <algorithm>        // Sort
#include <random>           // mt19937, random_device
#include <vector>           // Vector
#include <string>           // String

using namespace std;

struct Driver{
    string name;
    string team;
    int points;
};

int pointsTable[10] = {25,18,15,12,10,8,6,4,2,1};

void simulateRace(
    const vector<Driver>& Drivers,
          vector<Driver>& Standings,
          mt19937& RandomNumberGenerator

){
    int driverCount = Drivers.size();

    //Array sets the "Qualifying" order for the race
    int order[22]; //22 drivers from the 2026 season and onwards
    for(int i = 0; i < driverCount; i++){
        order[i] = i;
    }

    //Shuffle the array
    for(int i = 0; i < driverCount; i++){
        uniform_int_distribution<int> dist(0, driverCount -1);
        int j = dist(RandomNumberGenerator);

        int temp = order[i];
        order[i] = order[j];
        order[j] = temp;
    }
}