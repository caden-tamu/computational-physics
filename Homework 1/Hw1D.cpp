#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>

// Function to calculate numerical solution using Euler's method
void numericalWithBetaEuler(double N0, double totalTime, double growthRate, double beta, double timeStep,
                            std::vector<double>& time, std::vector<double>& population) {
    double N = N0;

    // **Scale beta for population in millions**
    beta *= 1e6; // Convert beta to match millions of population

    for (double t = 0.0; t <= totalTime; t += timeStep) {
        time.push_back(t);
        population.push_back(N);

        // **Calculate dN/dt based on the time**
        double dN_dt;
        if (t <= 100) {
            dN_dt = growthRate * N; // No beta term for t <= 100
        } else {
            dN_dt = growthRate * N - beta * N * N; // Include beta term for t > 100
        }

        // **Update population using Euler's method**
        N += timeStep * dN_dt;

        // **Prevent negative population due to numerical issues**
        if (N < 0) {
            N = 0;
        }
    }
}

// Function to calculate analytical steady-state solution
double steadyState(double growthRate, double beta) {
    // **Scale beta for analytical calculation**
    beta *= 1e6; // Ensure beta is scaled consistently with the population
    return growthRate / beta;
}

int main() {
    // Parameters
    double N0 = 10.0;           // Initial population (in millions)
    double growthRate = 0.02;   // Growth rate (2% per year)
    double beta = 1e-9;         // Anomalous death rate (per individual per year)
    double timeStep = 0.01;     // **Smaller time step (0.01 years) for better accuracy**
    double totalTime = 300.0;  // Total simulation time (2000 years)

    // Vectors for storing time and population
    std::vector<double> time, population;

    // Simulate using Euler's method
    numericalWithBetaEuler(N0, totalTime, growthRate, beta, timeStep, time, population);

    // Calculate analytical steady-state population
    double N_steady = steadyState(growthRate, beta);

    // Write results to a CSV file for analysis
    std::ofstream outFile("population_growth_beta_euler.csv");
    outFile << "Time,Population\n";
    for (size_t i = 0; i < time.size(); ++i) {
        outFile << time[i] << "," << population[i] << "\n";
    }
    outFile.close();

    // Output results to console
    std::cout << "Results written to 'population_growth_beta_euler.csv'.\n";
    std::cout << "Analytical steady-state population: " << N_steady << " million\n";
    std::cout << "Final numerical population: " << population.back() << " million\n";

    return 0;
}
