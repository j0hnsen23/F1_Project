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
    
}