#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

using namespace std;

// Parameters
const int iterations = 1000;  // Number of iterations
const int last = 100;         // Last few iterations to plot
const double x0 = 0.5;        // Initial condition

// Function to iterate the logistic map for fixed μ
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
    ofstream file("logistic_mu_2.1.txt");
    
    double mu = 2.1;  // Fixed μ value
    vector<double> values;
    
    // Compute logistic map values
    logistic_map(mu, values);

    // Save results to file
    for (double x : values) {
        file << x << endl;
    }

    file.close();
    
    // Print confirmation
    cout << "Logistic map data for μ = 2.1 saved to logistic_mu_2.1.txt.\n";
    
    return 0;
}