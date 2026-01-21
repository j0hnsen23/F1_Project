# F1 Season Simulator (C++)

A simple C++ console program that simulates a Formula 1–style season by generating random race results, applying a DNF chance per driver, awarding points to the top 10 finishers, and printing the final championship standings.

This project is intentionally lightweight and uses only the C++ standard library.

---

## Features

- Simulates any number of races (user input at runtime)
- Randomized finishing order per race (shuffle-based)
- Per-driver DNF chance (defaults to 8%)
- Standard F1-style points for the top 10 finishers:
  - 25, 18, 15, 12, 10, 8, 6, 4, 2, 1
- Prints:
  - Race classification (finishers in order)
  - DNFs listed at the end of each race
  - Final season standings sorted by points

---

## How It Works

Each race simulation follows these steps:

1. **Create a race order**: an index array (`order`) is filled with driver indices.
2. **Randomize the grid**: the order array is shuffled using an `mt19937` RNG.
3. **Apply DNFs**: each driver independently rolls against `DNF_RATE`.
4. **Print results**:
   - Finishers are printed as `P1, P2, ...`
   - DNFs are printed afterwards as `DNF | Name (Team)`
5. **Award points**:
   - Only the top 10 *finishers* receive points (DNFs do not score).
6. **After all races**: standings are sorted by total points and printed.

---

## User Input & How it looks while running

When the program starts, you will be prompted to choose how many races to simulate in the season:

```text
Enter the numbers of races in your season:

==============================
Race 1
===============================
 === Race Result ===
P1 | Driver Name (Team)
P2 | Driver Name (Team)
P3 | Driver Name (Team)
...
DNF | Driver Name (Team)
DNF | Driver Name (Team)

 === Standings ===
1. Driver Name (Team) - 132 points
2. Driver Name (Team) - 118 points
3. Driver Name (Team) - 95 points
...
