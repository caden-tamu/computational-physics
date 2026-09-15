#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>

using std::string;

class Projectile {

    public:

        //Used variables

        double height = 60;
        double distanceH = 180;
        double riverWidth = 55;
        double v0 = 50;
        double airDensity = 1.29;
        double dragCoeff = 0.5;
        double rockMassDensity = 2400;
        double rockDiameter = 0.25;
        double windSpeed = 25 * 0.44704; //convert to m/s

        //Initial conditions

        double x = 0.0;
        double y = height;
        double t = 0.0;
        double dt = 0.01;
        double g = 9.8;


        //Function to calculate the trajectory and store in a vector

        double trajectory(double theta0, string file1, bool saveCSV) {

            double thetaRadians = theta0 * (M_PI / 180); // Converting to radians
            double radius = rockDiameter / 2;
            double area = M_PI * pow(radius, 2);
            double mass = (4.0/3.0) * M_PI * pow(radius, 3) * rockMassDensity;

            //Initial velocities

            double Vx = v0 * cos(thetaRadians) - windSpeed;
            double Vy = v0 * sin(thetaRadians);

            std::vector<std::vector<double>> dataOutput;

            //Acts as a reset for the variables 

            x = 0.0;
            y = 60.0;
            t = 0.0;

            dataOutput.push_back({t, x, y, Vx, Vy, sqrt(Vx * Vx + Vy * Vy)}); //Added the intial points to the vector

            while(y >= 0) { //Loops until ground is reached
                double v = sqrt(pow(Vx, 2) + pow(Vy, 2));
                double F_drag = 0.5 * dragCoeff * pow(v ,2) * area * airDensity; //Drag force calculation

                //Accelerations

                double accelX = -F_drag * (Vx / v) / mass; 
                double accelY = -g - (F_drag * (Vy/v) / mass);

                //Euler method for calculating x, y, Vx, Vy, using dt

                Vy += accelY * dt;
                y += Vy * dt;
                Vx += accelX * dt;
                x += Vx * dt;

                dataOutput.push_back({t,x,y,Vx, Vy, v});
                t = t + dt;

            if (saveCSV) {
                writeToCSV(file1, dataOutput);
            }

        } 

        return x;
            
    }

        //Function to write data into a csv file for graphing

        void writeToCSV(const string& file1, const std::vector<std::vector<double>>& data) {
            std::ofstream file(file1);

            file << "time,x,y,vx,vy,v\n";

            for(const auto& i : data) {
                for (size_t j = 0; j < i.size(); ++j) {
                    file << i[j];
                    if (j < i.size() - 1) {
                        file << ",";
                    }
                }
                file << "\n";
            }

            file.close();

        }
};


int main() {

    Projectile obj;

    double anglesToTest[] = {15.0,25.0,33.5,35.0, 45.0}; //First test angles
    double distanceToShore = 235.0;

    std::ofstream impactFile("Impact_Speed_Wind.csv");
    impactFile << "theta,impact_speed\n";


    for (double angle : anglesToTest) {
        string file1 = "Trajectory_With_Wind_Of_" + std::to_string((int)angle) + "_Degrees.csv";
        obj.trajectory(angle, file1, true);
        double range = obj.trajectory(angle, file1, true);


        //Check to see if each angle reaches the shoreline
        if (range >= distanceToShore) {
            std::cout << angle << ": ✅ Reaches the shore (" << range << "m)\n";
        } else {
            std::cout << angle << ": ❌ Falls short (" << range << "m)\n";
        }
    }
 

    double maxRange = 0.0;
    double bestAngle = 0.0;


    // Checks each angle from 5-80 in 0.5 degree intervals, does not save them
    for (double angle = 5.0; angle <= 75.0; angle += 0.5) {
        double range = obj.trajectory(angle, "temp.csv", false); 

        if (range > maxRange) {
            maxRange = range;
            bestAngle = angle;
        }
    }

    //  Compute impact speed for angles from 4 to 80 in 2 increments
    std::cout << "\nComputing impact speeds from 4 to 80 in 2 increments:\n";
    for (double angle = 4.0; angle <= 80.0; angle += 2.0) {
        double range = obj.trajectory(angle, "temp.csv", false); 
        double impact_speed = sqrt(pow(obj.x, 2) + pow(obj.y, 2)); //Calculates the impact speed

        impactFile << angle << "," << impact_speed << "\n"; 
    }

    string bestFile = "Best_Trajectory_With_Wind" + std::to_string((int)bestAngle) + "_Degrees.csv";
    double bestRange = obj.trajectory(bestAngle, bestFile, true); //Finds R_max
    double bestImpactSpeed = sqrt(obj.x * obj.x + obj.y * obj.y); 

    impactFile << bestAngle << "," << bestImpactSpeed << "\n";
    impactFile.close();

    std::cout << "\nMaximum range: " << maxRange << " meters at " << bestAngle << " degrees\n";
    std::cout << "Best angle (" << bestAngle << ") Impact Speed: " << bestImpactSpeed << " m/s\n";

    

    return 0;
}
