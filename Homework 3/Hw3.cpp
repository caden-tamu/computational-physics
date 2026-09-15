#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

using namespace std;

// Parameters
const int iterations = 1000;  // Number of iterations per mu value
const int last = 100;
const double x0 = 0.5;        // Initial x0

// Function to iterate the logistic map
void logistic_map(double mu, vector<double> &values) {
    double x = x0;
    for (int i = 0; i < iterations; ++i) {
        x = mu * x * (1 - x);
        if (i >= iterations - last) {  // Store only the last values for plotting
            values.push_back(x);
        }
    }
}

int main() {
    ofstream file("data.txt");


    int steps = 1000;
    double min = 2.5; //Minimum mu for closer observation
    double max = 4.0;
    
    for (int i = 0; i < steps; ++i) {
        double mu = min + (max - min) * i / steps;
        vector<double> values; //creates new each time
        logistic_map(mu, values);
        
        for (double x : values) {
            file << mu << " " << x << endl;
        }
    }

    file.close();
    return 0;
}