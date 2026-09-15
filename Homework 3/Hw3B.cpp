#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <cmath>
#include <iomanip> // For formatting

using namespace std;

// Logistic map function
double logistic_map(double x, double mu) {
    return mu * x * (1 - x);
}

// Function to compute trajectory and stable fixpoints
vector<double> compute_trajectory(double mu, double x0, int iterations, int last) {
    vector<double> x_values;
    x_values.push_back(x0);

    for (int i = 0; i < iterations; ++i) {
        x_values.push_back(logistic_map(x_values.back(), mu));
    }

    // Extract last `last` values (to detect fixpoints)
    set<double> stable_values;
    for (int i = iterations - last; i < iterations; ++i) {
        stable_values.insert(round(x_values[i] * 1e6) / 1e6);  // Round to 6 decimals
    }

    return vector<double>(stable_values.begin(), stable_values.end());
}

int main() {
    double mu_values[] = {2.8, 3.5, 3.9};  // Period-1, Period-4, Chaotic
    double x0 = 0.2;
    int iterations = 500, last = 100;

    ofstream file("trajectories.txt");
    file << fixed << setprecision(6);

    for (double mu : mu_values) {
        vector<double> fixpoints = compute_trajectory(mu, x0, iterations, last);
        
        cout << "μ = " << mu << " | Stable Fixpoints: ";
        for (double x : fixpoints) {
            cout << x << " ";
            file << mu << " " << x << endl;  // Save to file
        }
        cout << endl;
    }

    file.close();
    cout << "Trajectories saved to 'trajectories.txt'. Use Python for plotting." << endl;
    return 0;
}
