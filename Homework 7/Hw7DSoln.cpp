#include <iostream>
#include <vector>
#include <random>
#include <fstream>
#include <cmath>      // for std::log
#include <algorithm>  // for std::find

int main() {
    // Grid dimensions and number of particles
    const int NX = 200;
    const int NY = 200;
    const int N = 2000;

    // Set up random number generation
    std::mt19937 rng(12345);
    std::uniform_int_distribution<int> particleDist(0, N - 1);
    std::uniform_int_distribution<int> directionDist(0, 3);

    // Particle positions (x and y)
    std::vector<int> xPos(N), yPos(N);

    // Place 500 particles in each corner:
    // Bottom-left
    for (int i = 0; i < 500; ++i) {
        xPos[i] = 0;
        yPos[i] = 0;
    }
    // Bottom-right
    for (int i = 500; i < 1000; ++i) {
        xPos[i] = NX - 1;
        yPos[i] = 0;
    }
    // Top-left
    for (int i = 1000; i < 1500; ++i) {
        xPos[i] = 0;
        yPos[i] = NY - 1;
    }
    // Top-right
    for (int i = 1500; i < 2000; ++i) {
        xPos[i] = NX - 1;
        yPos[i] = NY - 1;
    }

    // Total simulation steps: 5,000,000
    const int totalSteps = 5000000;

    // We want 20 snapshot points, including t=1 and t=5,000,000.
    int numPoints = 20;
    std::vector<int> snapshotSteps;
    for (int i = 0; i < numPoints; i++) {
        // Compute equally spaced snapshot time:
        // t = 1 + i * (totalSteps - 1) / (numPoints - 1)
        int t = 1 + static_cast<int>( i * ((totalSteps - 1.0) / (numPoints - 1)) );
        snapshotSteps.push_back(t);
    }

    // Open file to record entropy vs. time
    std::ofstream entropyFile("entropy_vs_time_L200.csv");
    if (!entropyFile) {
        std::cerr << "Error opening entropy_vs_time.csv for writing.\n";
        return 1;
    }
    entropyFile << "t,entropy\n";

    // Function to compute the coarse-grained entropy
    auto computeEntropy = [&](const std::vector<int>& xPos, const std::vector<int>& yPos) -> double {
        // We partition the 100x100 grid into 10x10 coarse cells (100 total)
        const int nCoarseX = 10;
        const int nCoarseY = 10;
        const int M = nCoarseX * nCoarseY;  // 100 cells

        // Count particles in each coarse cell
        std::vector<int> coarseCount(M, 0);
        for (int i = 0; i < N; ++i) {
            int cx = xPos[i] / 10;  // integer division: 0 to 9
            int cy = yPos[i] / 10;
            int idx = cy * nCoarseX + cx;
            coarseCount[idx]++;
        }

        // Compute entropy: S = -sum_i p_i log(p_i)
        double S = 0.0;
        for (int i = 0; i < M; ++i) {
            if (coarseCount[i] > 0) {
                double p = static_cast<double>(coarseCount[i]) / N;
                S -= p * std::log(p);
            }
        }
        return S;
    };

    // Prepare for simulation: track the next snapshot index and its corresponding time.
    int snapshotIndex = 0;
    int nextSnapshotStep = snapshotSteps[snapshotIndex];

    // Main diffusion loop
    for (int step = 0; step <= totalSteps; ++step) {
        // For steps > 0, perform a random move for one particle.
        if (step > 0) {
            int p = particleDist(rng);
            int dir = directionDist(rng);

            int newX = xPos[p];
            int newY = yPos[p];
            switch (dir) {
                case 0: newX++; break;  // Right
                case 1: newX--; break;  // Left
                case 2: newY++; break;  // Up
                case 3: newY--; break;  // Down
            }
            // Reflect at boundaries
            if (newX < 0) newX = 1;
            else if (newX >= NX) newX = NX - 2;
            if (newY < 0) newY = 1;
            else if (newY >= NY) newY = NY - 2;

            xPos[p] = newX;
            yPos[p] = newY;
        }

        // At designated snapshot steps, compute and record the entropy.
        if (step == nextSnapshotStep) {
            double S = computeEntropy(xPos, yPos);
            entropyFile << step << "," << S << "\n";
            std::cout << "t=" << step << ", Entropy = " << S << "\n";

            snapshotIndex++;
            if (snapshotIndex < snapshotSteps.size()) {
                nextSnapshotStep = snapshotSteps[snapshotIndex];
            }
        }
    }

    entropyFile.close();
    return 0;
}
