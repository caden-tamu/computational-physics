#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>

void numerical(double N0, double totalTime, double growthRate, double timeStep, std::vector<double>& time,
               std::vector<double>& numericalSolnN, std::vector<double>& numericalSolnR) {
    double storedN = N0;

    for (double i = 0.0; i <= totalTime; i += timeStep) {
    time.push_back(i);
    numericalSolnN.push_back(storedN);
    numericalSolnR.push_back(storedN * growthRate);
    storedN += storedN * growthRate * timeStep; // Uses timeStep here
}
}

double analytic(double t, double N0, double growthRate) {
    return N0 * std::exp(growthRate * t);
}

void consistencyAnalysis(double growthRate, double N0, double t1, double dt1, double dt2) {
    int steps_dt1 = static_cast<int>(t1 / dt1);
    int steps_dt2 = static_cast<int>(t1 / dt2);

    double N_dt1 = N0;
    double N_dt2 = N0;

    for (int i = 0; i < steps_dt1; ++i) {
        N_dt1 += dt1 * growthRate * N_dt1;
    }

    for (int i = 0; i < steps_dt2; ++i) {
        N_dt2 += dt2 * growthRate * N_dt2;
    }

    double N_exact_t1 = analytic(t1, N0, growthRate);

    double error_dt1 = std::fabs(N_dt1 - N_exact_t1);
    double error_dt2 = std::fabs(N_dt2 - N_exact_t1);
    double errorRatio = (error_dt1 / error_dt2);

    std::cout << "Consistency Analysis at t = " << t1 << ":\n";
    std::cout << "Numerical solution for dt1 = " << dt1 << ": " << N_dt1 << ", Error: " << error_dt1 << "\n";
    std::cout << "Numerical solution for dt2 = " << dt2 << ": " << N_dt2 << ", Error: " << error_dt2 << "\n";
    std::cout << "Analytical solution: " << N_exact_t1 << "\n";
    std::cout << "Error ratio (should be ~2 for Euler's method): " << errorRatio << "\n\n";
}

void percentageDeviationAfterTwoHalfLives(double N0, double growthRate, double dt) {
    double T_half = std::log(2) / growthRate;
    double tTwoHalfLives = 2.0 * T_half;

    double N_numeric = N0;
    for (double t = 0.0; t < tTwoHalfLives; t += dt) {
        N_numeric += dt * growthRate * N_numeric;
    }

    double N_exact = analytic(tTwoHalfLives, N0, growthRate);

    double percentDeviation = std::fabs(N_numeric - N_exact) / N_exact * 100.0;

    std::cout << "Percentage Deviation After 2 Half-Lives:\n";
    std::cout << "Time (2 half-lives): " << tTwoHalfLives << " years\n";
    std::cout << "Analytical solution: " << N_exact << "\n";
    std::cout << "Numerical solution (dt = " << dt << "): " << N_numeric << "\n";
    std::cout << "Percentage deviation: " << percentDeviation << "%\n\n";
}

int main() {
    double timeStep = 1.0;
    double totalTime = 100.0;
    double growthRate = 0.02;
    double N0 = 10.0;
    std::vector<double> time, numericalSolnN, numericalSolnR;
    std::vector<double> analyticSolnN, analyticSolnR;
    std::vector<double> errorN, errorR;

    numerical(N0, totalTime, growthRate, timeStep, time, numericalSolnN, numericalSolnR);

    for (size_t i = 0; i < time.size(); ++i) {
        double N_exact = analytic(time[i], N0, growthRate);
        double R_exact = growthRate * N_exact;

        analyticSolnN.push_back(N_exact);
        analyticSolnR.push_back(R_exact);

        errorN.push_back(std::fabs(numericalSolnN[i] - N_exact));
        errorR.push_back(std::fabs(numericalSolnR[i] - R_exact));
    }

    double t1 = 100.0;
    double dt1 = timeStep;
    double dt2 = dt1 / 2.0;
    consistencyAnalysis(growthRate, N0, t1, dt1, dt2);

    percentageDeviationAfterTwoHalfLives(N0, growthRate, dt1);

    std::ofstream outFile("population_growth_real1.0.csv");
    outFile << "Time,N_numeric,R_numeric,N_analytical,R_analytical,Error_N,Error_R\n";

    for (size_t i = 0; i < time.size(); ++i) {
        outFile << time[i] << "," << numericalSolnN[i] << "," << numericalSolnR[i] << ","
                << analyticSolnN[i] << "," << analyticSolnR[i] << "," << errorN[i] << "," << errorR[i] << "\n";
    }

    outFile.close();

    std::cout << "Results written to 'population_growth_real1.0.csv'.\n";

    return 0;
}
