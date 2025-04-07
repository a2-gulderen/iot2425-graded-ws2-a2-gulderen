#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <iomanip>

using namespace std;

int main() {
    // Seed random number generator.
    srand(static_cast<unsigned int>(time(nullptr)));

    // Base coordinates (for example, a central point in Bristol).
    double baseLat = 51.45;
    double baseLon = -2.58;

    // Number of records to generate per CSV.
    const int numRecords = 10;

    // Generate CSV files for 4 eBikes.
    for (int i = 1; i <= 4; i++) {
        // Build filename, e.g., data/sim-eBike-1.csv
        string filename = "data/sim-eBike-" + to_string(i) + ".csv";

        // Open file for writing.
        ofstream outFile(filename);
        if (!outFile.is_open()) {
            cerr << "Error: Could not open file " << filename << " for writing." << endl;
            continue;
        }

        // Generate numRecords lines with random offsets.
        for (int j = 0; j < numRecords; j++) {
            // Generate a random offset within ±0.01 degrees (~1km).
            double offsetLat = ((rand() % 2001) - 1000) / 100000.0;
            double offsetLon = ((rand() % 2001) - 1000) / 100000.0;
            double newLat = baseLat + offsetLat;
            double newLon = baseLon + offsetLon;
            // Write the coordinates to CSV (latitude,longitude).
            outFile << fixed << setprecision(6) << newLat << "," << newLon << "\n";
        }

        outFile.close();
        cout << "Generated " << filename << " with " << numRecords << " records." << endl;
    }
    
    return 0;
}
