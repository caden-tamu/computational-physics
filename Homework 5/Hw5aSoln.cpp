#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>

int main() {
    // Loop parameters
    double mu0I = 3e-6;   // μ0 * I
    double d = 0.1;       // diameter (m)
    double R = d / 2.0;   // radius = 0.05 m

    // Discretization for the z-axis
    int N_z = 200;
    double z_min = -0.1;
    double z_max =  0.1;
    double dz = (z_max - z_min) / (N_z - 1);

    // Discretization for Biot-Savart integration over φ
    int N_phi = 1000;
    double phi_start = 0.0;
    double phi_end   = 2.0 * M_PI;
    double dphi = (phi_end - phi_start) / (N_phi - 1);

    // Output file: (z, Bx, By, Bz)
    std::ofstream outfile("Bxyz_vs_z.csv");
    outfile << "z,Bx,By,Bz\n";

    // Main loop over z
    for (int i = 0; i < N_z; i++) {
        double z = z_min + i * dz;

        // Accumulators
        double Bx_sum = 0.0;
        double By_sum = 0.0;
        double Bz_sum = 0.0;

        // Trapezoid integration over φ
        for (int j = 0; j < N_phi; j++) {
            double phi = phi_start + j * dphi;

            // Position on loop: (R cosφ, R sinφ, 0)
            // dr': (-R sinφ, R cosφ, 0)
            double rx_prime = R * std::cos(phi);
            double ry_prime = R * std::sin(phi);
            double rz_prime = 0.0;

            double drx = -R * std::sin(phi);
            double dry =  R * std::cos(phi);
            double drz =  0.0;

            // Field point is (0, 0, z)
            // => (r - r') = (-rx_prime, -ry_prime, z - 0)
            double Rx = -rx_prime;
            double Ry = -ry_prime;
            double Rz =  z - rz_prime;

            // Cross product: d(r') × (r - r')
            double cross_x = dry * Rz - drz * Ry;
            double cross_y = drz * Rx - drx * Rz;
            double cross_z = drx * Ry - dry * Rx;

            // |r - r'|^3
            double dist = std::sqrt(Rx*Rx + Ry*Ry + Rz*Rz);
            double denom = dist * dist * dist;

            // Biot-Savart differential
            double dBx = (mu0I / (4.0 * M_PI)) * (cross_x / denom);
            double dBy = (mu0I / (4.0 * M_PI)) * (cross_y / denom);
            double dBz = (mu0I / (4.0 * M_PI)) * (cross_z / denom);

            // Trapezoid weighting
            if (j == 0 || j == N_phi - 1) {
                Bx_sum += dBx;
                By_sum += dBy;
                Bz_sum += dBz;
            } else {
                Bx_sum += 2.0 * dBx;
                By_sum += 2.0 * dBy;
                Bz_sum += 2.0 * dBz;
            }
        }

        // Complete trapezoid factor
        Bx_sum *= (dphi / 2.0);
        By_sum *= (dphi / 2.0);
        Bz_sum *= (dphi / 2.0);

        // Write results
        outfile << std::fixed << std::setprecision(8)
                << z << "," << Bx_sum << "," << By_sum << "," << Bz_sum << "\n";
    }

    outfile.close();
    std::cout << "Data written to Bxyz_vs_z.csv\n";
    return 0;
}
