#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>

int main() {
    // Loop parameters
    double mu0I = 3e-6;  // μ0 * I (SI units)
    double d = 0.1;      // diameter (m)
    double R = d / 2.0;  // radius = 0.05 m

    // Number of points in x and z directions
    int Nx = 21;
    int Nz = 21;

    double x_min = -0.1;
    double x_max =  0.1;
    double z_min = -0.1;
    double z_max =  0.1;

    double dx = (x_max - x_min) / (Nx - 1);
    double dz = (z_max - z_min) / (Nz - 1);

    // Discretization for the Biot-Savart integral over φ
    int N_phi = 2000;
    double phi_start = 0.0;
    double phi_end   = 2.0 * M_PI;
    double dphi = (phi_end - phi_start) / (N_phi - 1);

    // Output file
    std::ofstream outfile("B_xz_plane.csv");
    // We'll store x, z, Bx, By, Bz
    outfile << "x,z,Bx,By,Bz\n";

    // A small "band" if you want to skip near the wire at x=±R, z=0
    double band = 1e-3;

    // Loop over the grid in x-z plane (y=0)
    for (int ix = 0; ix < Nx; ix++) {
        for (int iz = 0; iz < Nz; iz++) {
            double x = x_min + ix * dx;
            double z = z_min + iz * dz;

            // Optional: skip points too close to the wire
            // if (fabs(fabs(x) - R) < band && fabs(z) < band) {
            //     continue;
            // }

            double Bx_sum = 0.0;
            double By_sum = 0.0;
            double Bz_sum = 0.0;

            // Numerically integrate with trapezoid rule
            for (int j = 0; j < N_phi; j++) {
                double phi = phi_start + j * dphi;

                // Loop parameterization
                // r'(φ) = (R cosφ, R sinφ, 0)
                // dr'   = (-R sinφ, R cosφ, 0)
                double rxp = R * std::cos(phi);
                double ryp = R * std::sin(phi);
                double rzp = 0.0;

                double drx = -R * std::sin(phi);
                double dry =  R * std::cos(phi);
                double drz =  0.0;

                // Field point: (x, 0, z)
                // => (r - r') = (x - rxp, - ryp, z - 0)
                double Rx = x - rxp;
                double Ry = 0.0 - ryp;
                double Rz = z - rzp;

                // Cross product: d(r') × (r - r')
                double cross_x = dry * Rz - drz * Ry;
                double cross_y = drz * Rx - drx * Rz;
                double cross_z = drx * Ry - dry * Rx;

                double dist = std::sqrt(Rx*Rx + Ry*Ry + Rz*Rz);
                double denom = dist*dist*dist;

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

            // Write to CSV
            outfile << std::fixed << std::setprecision(8)
                    << x << "," << z << ","
                    << Bx_sum << "," << By_sum << "," << Bz_sum << "\n";
        }
    }

    outfile.close();
    std::cout << "Data written to B_xz_plane.csv\n";
    return 0;
}
