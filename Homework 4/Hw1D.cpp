#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>
#include <string>


int main() {

    //Declarations

    double e = 0.206;
    double a = 0.39;
    std::vector<double> alphas = {0.0001, 0.001, 0.002};
    double G_M_Sun = 4.0 * std::pow(M_PI, 2);
    
    for(double a : alphas) {

        //time parameter
        double t = 0.0;
        double dt = 1.0e-4;
        double maxT = 2.4; //Approximately 10 orbits
        double xPos = a * (e + 1.0);
        double yPos = 0.0;

        //Circular paths

        double r0 = a * (e + 1.0); // Starting at aphelion
        double v0 = std::sqrt(4.0*std::pow(M_PI, 2) * (2.0/r0 - 1.0/a));

        double Vx = 0.0;
        double Vy = v0;


        //Make the file

        std::ofstream outfile("MercuryOrbitWithGR.txt");
        outfile << "# t x y vx vy \n";

        //Loop through the Euler-Cromer Method
        while(t < maxT) {
            double r = std::sqrt(std::pow(xPos, 2) + std::pow(yPos, 2));
            double correctionConstant = (a + 1) / std::pow(r,2);
            double accelX = (-G_M_Sun * xPos / std::pow(r, 3)) * correctionConstant; 
            double accelY = (-G_M_Sun * yPos / std::pow(r, 3)) * correctionConstant; 

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
    return 0;
}