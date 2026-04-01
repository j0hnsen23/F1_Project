#include <iostream>
#include <algorithm>
#include <random>
#include <vector>
#include <string>
#include <map>

using namespace std;

class Driver {
private:
    string name;
    string team;
    int points;
    int talent;
    int experience;
    int carPerformance;
    int wins;
    int podiums;
    int dnfs;

public:
    Driver(const string& name,
           const string& team,
           int talent,
           int experience,
           int carPerformance)
        : name(name),
          team(team),
          points(0),
          talent(talent),
          experience(experience),
          carPerformance(carPerformance),
          wins(0),
          podiums(0),
          dnfs(0) {}

    string getName() const { return name; }
    string getTeam() const { return team; }
    int getPoints() const { return points; }
    int getTalent() const { return talent; }
    int getExperience() const { return experience; }
    int getCarPerformance() const { return carPerformance; }
    int getWins() const { return wins; }
    int getPodiums() const { return podiums; }
    int getDNFs() const { return dnfs; }

    void addPoints(int pts) { points += pts; }
    void addWin() { wins++; }
    void addPodium() { podiums++; }
    void addDNF() { dnfs++; }
};

class RaceWeekend {
private:
    static constexpr float BASE_DNF_RATE = 0.08f;
    int pointsTable[10] = {25, 18, 15, 12, 10, 8, 6, 4, 2, 1};

    vector<int> qualifyingOrder;
    vector<int> raceOrder;
    vector<bool> didFinish;

public:
    void simulateQualifying(const vector<Driver>& drivers, mt19937& rng) {
        vector<pair<int, int>> qualifyingScores;

        for (int i = 0; i < (int)drivers.size(); i++) {
            uniform_int_distribution<int> randomness(-10, 10);

            // Kvalifisering: bil + talent viktigst, erfaring hjelper litt
            int score = drivers[i].getTalent() * 2
                      + drivers[i].getCarPerformance() * 2
                      + drivers[i].getExperience()
                      + randomness(rng);

            qualifyingScores.push_back({score, i});
        }

        sort(qualifyingScores.begin(), qualifyingScores.end(),
             [](const pair<int, int>& a, const pair<int, int>& b) {
                 return a.first > b.first;
             });

        qualifyingOrder.clear();
        for (const auto& entry : qualifyingScores) {
            qualifyingOrder.push_back(entry.second);
        }

        cout << "\n=== Qualifying Result ===\n";
        for (int i = 0; i < (int)qualifyingOrder.size(); i++) {
            int driverIndex = qualifyingOrder[i];
            cout << "P" << (i + 1) << " | "
                 << drivers[driverIndex].getName()
                 << " (" << drivers[driverIndex].getTeam() << ")\n";
        }
    }

    void simulateRace(vector<Driver>& standings, mt19937& rng) {
        if (qualifyingOrder.empty()) {
            cout << "Error: Qualifying must be simulated before the race.\n";
            return;
        }

        vector<pair<int, int>> raceScores;

        for (int gridPos = 0; gridPos < (int)qualifyingOrder.size(); gridPos++) {
            int driverIndex = qualifyingOrder[gridPos];

            uniform_int_distribution<int> randomness(-12, 12);

            int gridBonus = (int)qualifyingOrder.size() - gridPos;

            // Løp: erfaring og bil teller mer, men talent er fortsatt viktig
            int raceScore = standings[driverIndex].getTalent()
                          + standings[driverIndex].getExperience() * 2
                          + standings[driverIndex].getCarPerformance() * 2
                          + gridBonus
                          + randomness(rng);

            raceScores.push_back({raceScore, driverIndex});
        }

        sort(raceScores.begin(), raceScores.end(),
             [](const pair<int, int>& a, const pair<int, int>& b) {
                 return a.first > b.first;
             });

        raceOrder.clear();
        for (const auto& entry : raceScores) {
            raceOrder.push_back(entry.second);
        }

        didFinish.assign(standings.size(), true);
        uniform_real_distribution<float> dnfRoll(0.0f, 1.0f);

        for (int i = 0; i < (int)raceOrder.size(); i++) {
            int driverIndex = raceOrder[i];

            // Erfaring reduserer DNF-risiko
            float experienceReduction =
                (float)standings[driverIndex].getExperience() / 100.0f * 0.05f;

            float dnfChance = BASE_DNF_RATE - experienceReduction;

            if (dnfChance < 0.02f) {
                dnfChance = 0.02f;
            }

            if (dnfRoll(rng) < dnfChance) {
                didFinish[driverIndex] = false;
                standings[driverIndex].addDNF();
            }
        }

        cout << "\n=== Race Result ===\n";

        vector<int> classifiedDrivers;
        vector<int> dnfDrivers;

        for (int i = 0; i < (int)raceOrder.size(); i++) {
            int driverIndex = raceOrder[i];

            if (didFinish[driverIndex]) {
                classifiedDrivers.push_back(driverIndex);
            } else {
                dnfDrivers.push_back(driverIndex);
            }
        }

        for (int i = 0; i < (int)classifiedDrivers.size(); i++) {
            int driverIndex = classifiedDrivers[i];
            cout << "P" << (i + 1) << " | "
                 << standings[driverIndex].getName()
                 << " (" << standings[driverIndex].getTeam() << ")\n";
        }

        for (int i = 0; i < (int)dnfDrivers.size(); i++) {
            int driverIndex = dnfDrivers[i];
            cout << "DNF | "
                 << standings[driverIndex].getName()
                 << " (" << standings[driverIndex].getTeam() << ")\n";
        }

        // Del ut poeng
        for (int i = 0; i < (int)classifiedDrivers.size() && i < 10; i++) {
            int driverIndex = classifiedDrivers[i];
            standings[driverIndex].addPoints(pointsTable[i]);

            if (i == 0) {
                standings[driverIndex].addWin();
            }

            if (i < 3) {
                standings[driverIndex].addPodium();
            }
        }
    }
};

class SeasonSimulator {
private:
    vector<Driver> standings;
    mt19937 rng;

public:
    SeasonSimulator(const vector<Driver>& drivers)
        : standings(drivers), rng(random_device{}()) {}

    void runSeason(int numberOfRaces) {
        for (int race = 1; race <= numberOfRaces; race++) {
            cout << "\n====================================\n";
            cout << "Race " << race << "\n";
            cout << "====================================\n";

            RaceWeekend weekend;
            weekend.simulateQualifying(standings, rng);
            weekend.simulateRace(standings, rng);
        }

        displayDriverStandings();
        displayConstructorStandings();
    }

    void displayDriverStandings() const {
        vector<Driver> sortedStandings = standings;

        sort(sortedStandings.begin(), sortedStandings.end(),
             [](const Driver& a, const Driver& b) {
                 if (a.getPoints() == b.getPoints()) {
                     return a.getWins() > b.getWins();
                 }
                 return a.getPoints() > b.getPoints();
             });

        cout << "\n=== Driver Standings ===\n";
        for (int i = 0; i < (int)sortedStandings.size(); i++) {
            cout << (i + 1) << ". "
                 << sortedStandings[i].getName()
                 << " (" << sortedStandings[i].getTeam() << ")"
                 << " - " << sortedStandings[i].getPoints() << " pts"
                 << " | Wins: " << sortedStandings[i].getWins()
                 << " | Podiums: " << sortedStandings[i].getPodiums()
                 << " | DNFs: " << sortedStandings[i].getDNFs()
                 << "\n";
        }
    }

    void displayConstructorStandings() const {
        map<string, int> teamPoints;

        for (int i = 0; i < (int)standings.size(); i++) {
            teamPoints[standings[i].getTeam()] += standings[i].getPoints();
        }

        vector<pair<string, int>> sortedTeams;
        for (const auto& entry : teamPoints) {
            sortedTeams.push_back(entry);
        }

        sort(sortedTeams.begin(), sortedTeams.end(),
             [](const pair<string, int>& a, const pair<string, int>& b) {
                 return a.second > b.second;
             });

        cout << "\n=== Constructor Standings ===\n";
        for (int i = 0; i < (int)sortedTeams.size(); i++) {
            cout << (i + 1) << ". "
                 << sortedTeams[i].first
                 << " - " << sortedTeams[i].second << " pts\n";
        }
    }
};

int main() {
    vector<Driver> drivers = {
        Driver("Max Verstappen", "Red Bull", 99, 95, 96),
        Driver("Isack Hadjar", "Red Bull", 84, 40, 96),

        Driver("Lewis Hamilton", "Ferrari", 96, 100, 94),
        Driver("Charles Leclerc", "Ferrari", 95, 88, 94),

        Driver("Lando Norris", "McLaren", 94, 80, 98),
        Driver("Oscar Piastri", "McLaren", 93, 70, 98),

        Driver("George Russell", "Mercedes", 92, 82, 92),
        Driver("Andrea Kimi Antonelli", "Mercedes", 88, 35, 92),

        Driver("Lance Stroll", "Aston Martin", 78, 75, 86),
        Driver("Fernando Alonso", "Aston Martin", 95, 100, 86),

        Driver("Alex Albon", "Williams", 88, 72, 84),
        Driver("Carlos Sainz", "Williams", 91, 90, 84),

        Driver("Liam Lawson", "Racing Bulls", 84, 45, 82),
        Driver("Arvid Lindblad", "Racing Bulls", 82, 25, 82),

        Driver("Esteban Ocon", "Haas", 87, 85, 80),
        Driver("Ollie Bearman", "Haas", 86, 35, 80),

        Driver("Nico Hulkenberg", "Audi", 89, 95, 78),
        Driver("Gabriel Bortoleto", "Audi", 85, 30, 78),

        Driver("Pierre Gasly", "Alpine", 88, 80, 79),
        Driver("Franco Colapinto", "Alpine", 84, 40, 79),

        Driver("Valtteri Bottas", "Cadillac", 87, 98, 76),
        Driver("Sergio Perez", "Cadillac", 90, 92, 76)
    };

    int numberOfRaces;

    cout << "Enter the number of races in your season: ";
    cin >> numberOfRaces;

    while (cin.fail() || numberOfRaces <= 0) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Please enter a valid positive number: ";
        cin >> numberOfRaces;
    }

    SeasonSimulator simulator(drivers);
    simulator.runSeason(numberOfRaces);

    return 0;
}