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

    //Print the result of the race
    cout << "\n === Race Result === \n";

    for (int position = 0; position < driverCount; position++){
        int driverIndex = order[position];

        cout << "P" << (position + 1) << " | "
             << Drivers[driverIndex].name
             << " (" << Drivers[driverIndex].team << ") \n";
    }

    //Award points for top 10
    for(int pos = 0; pos < driverCount && pos < 10; pos++){
        int index = order[pos];
        Standings[index].points += pointsTable[pos];
    }
}

void displayStandings(vector<Driver>& standings) {
        sort(standings.begin(), standings.end(),
            [](const Driver& a, const Driver& b){
                return a.points > b.points;
        });

        cout << "\n === Standings === \n";
        for(int i = 0; i < standings.size(); i++){
            cout << (i+1) << ". "
                 << standings[i].name
                 << "(" <<standings[i].team <<") "
                 << " - " << standings[i].points
                 << " points\n";
        }
    }

int main(){
    
    // Driver
    std::vector<Driver> drivers = {
        {"Max Verstappen", "Red Bull", 0},
        {"Isack Hadjar", "Red Bull", 0},
        {"Lewis Hamilton", "Ferrari", 0},
        {"Charles Leclerc", "Ferrari", 0},
        {"Lando Norris", "McLaren", 0},
        {"Oscar Piastri", "McLaren", 0},
        {"George Russell", "Mercedes", 0},
        {"Andrea Kimi Antonelli", "Mercedes", 0},
        {"Lance Stroll", "Aston Martin", 0},
        {"Fernando Alonso", "Aston Martin", 0},
        {"Alex Albon", "Williams", 0},
        {"Carlos Sainz", "Williams", 0},
        {"Liam Lawson","Racing Bulls",0},
        {"Arvid Lindblad","Racing Bulls",0},
        {"Esteban Ocon","Haas",0},
        {"Ollie Bearman","Haas",0},
        {"Nico Hulkenberg","Audi",0},
        {"Gabriel Bortoleto","Audi",0},
        {"Pierre Gasly","Alpine",0},
        {"Franco Colapinto","Alpine",0},
        {"Valtteri Bottas","Cadillac",0},
        {"Sergio Perez","Cadillac",0}
    };

    vector<Driver> standings = drivers;

    random_device RandomDevice;
    mt19937 rng (RandomDevice());

    int numberOfRaces;
    cout << "Enter the numbers of races in your season: ";
    cin >> numberOfRaces;

    for(int race = 1; race <= numberOfRaces; race++){
        cout << "\n==============================\n";
        cout << "Race " << race << "\n";
        cout << "===============================\n";

        simulateRace(drivers, standings, rng);
    }

    displayStandings(standings);

    return 0;
}