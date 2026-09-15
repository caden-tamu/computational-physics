#include <iostream>
#include <cmath>
#include <fstream>


int main() {

    //Declarations

    double xPos = 0.39;
    double yPos = 0.0;
    
    double G_M_Sun = 4.0 * std::pow(M_PI, 2);

    //time parameter
    double t = 0.0;
    double dt = 1.0e-4;
    double maxT = 2.4; //Approximately 10 orbits

    //Circular paths

    double r0 = std::sqrt(std::pow(xPos, 2) + std::pow(yPos, 2));
    double v0 = std::sqrt(G_M_Sun / r0); // (4pi^2 / 0.39)^(1/2) = 10.06

    double Vx = 0.0;
    double Vy = v0;


    //Make the file

    std::ofstream outfile("MercuryOrbit.txt");
    outfile << "# t x y vx vy";

    //Loop through the Euler-Cromer Method
    while(t < maxT) {
        double r = std::sqrt(std::pow(xPos, 2) + std::pow(yPos, 2));
        double accelX = (-G_M_Sun * xPos / std::pow(r, 3)); 
        double accelY = (-G_M_Sun * yPos / std::pow(r, 3)); 

        //First I update velocity, then the position

        Vx = Vx + (accelX*dt);
        Vy = Vy + (accelY*dt);
        xPos = xPos + (Vx*dt);
        yPos = yPos + (Vy*dt);

        outfile << t << " " << xPos << " " << yPos << " " << Vx << " " << Vy << "\n";
        t = t + dt;
    }

    outfile.close();

}