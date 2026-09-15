#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>

int main() {
    // Loop parameters
    double mu0I = 3e-6;   // μ0 * I (SI units)
    double d = 0.1;       // diameter (m)
    double R = d / 2.0;   // radius = 0.05 m

    // Discretization for the Biot-Savart integral
    int N_phi = 2000;     // # of steps in φ
    double phi_start = 0.0;
    double phi_end   = 2.0 * M_PI;
    double dphi = (phi_end - phi_start) / (N_phi - 1);

    // We'll let t go from -0.1 to +0.1
    // => x=y=z = t
    // => r = sqrt(3)*|t|
    int N_t = 200; 
    double t_min = -0.1;
    double t_max =  0.1;
    double dt = (t_max - t_min) / (N_t - 1);

    // Prepare output file
    std::ofstream outfile("Bxyz_vs_r.csv");
    outfile << "r,Bx,By,Bz\n";

    // Main loop over t
    for (int i = 0; i < N_t; i++) {
        double t = t_min + i * dt;

        // Field point = (t, t, t)
        double x = t;
        double y = t;
        double z = t;

        // We'll compute r = sqrt(x^2 + y^2 + z^2) = sqrt(3)*|t|
        double r_val = std::sqrt(x*x + y*y + z*z);

        // Accumulators for Bx, By, Bz
        double Bx_sum = 0.0;
        double By_sum = 0.0;
        double Bz_sum = 0.0;

        // Trapezoid integration over the loop angle φ
        for (int j = 0; j < N_phi; j++) {
            double phi = phi_start + j * dphi;

            // Position on loop: r'(φ) = (R cosφ, R sinφ, 0)
            // dr' = (-R sinφ, R cosφ, 0) dφ
            double rx_prime = R * std::cos(phi);
            double ry_prime = R * std::sin(phi);
            double rz_prime = 0.0;

            double drx = -R * std::sin(phi);
            double dry =  R * std::cos(phi);
            double drz =  0.0;

            // Vector (r - r')
            double Rx = x - rx_prime;
            double Ry = y - ry_prime;
            double Rz = z - rz_prime;

            // Cross product d(r') × (r - r')
            double cross_x = dry * Rz - drz * Ry;
            double cross_y = drz * Rx - drx * Rz;
            double cross_z = drx * Ry - dry * Rx;

            // Distance^3
            double dist = std::sqrt(Rx*Rx + Ry*Ry + Rz*Rz);
            double denom = dist*dist*dist;

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

        // Complete the trapezoid factor
        Bx_sum *= (dphi / 2.0);
        By_sum *= (dphi / 2.0);
        Bz_sum *= (dphi / 2.0);

        // Write to CSV (r, Bx, By, Bz)
        outfile << std::fixed << std::setprecision(8)
                << r_val << "," << Bx_sum << "," << By_sum << "," << Bz_sum << "\n";
    }

    outfile.close();
    std::cout << "Data written to Bxyz_vs_r.csv\n";
    return 0;
}
