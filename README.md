# computational-physics
This repository showcases the work I completed during my time in undergrad at Texas A\&M University. These biweekly projects outline my skills in Python and C++ to handle complex data and arrive at simplified assumptions based on the data.

## Homework 1 - Numerical Methods & Euler Method

In this assignment, I was tasked to model population growth that is proportional to its current size. Euler's method was employed to numerically solve the population-growth differential equation. It was then compared with the analytical solution to $\frac{dN}{dt} = rN$, providing the solution $N(t) = N_{0} e^{rt}$. I then investigated how adjusting the timesteps and percentage deviation between the analytical and numerical solutions. The population growth and bifurcation was plotted in PlottingHw1.jpynb

## Homework 2 - Projectile Motion

In this assignment, I was tasked to model the trajectory of a rock thrown from a height towards a river, and to determine if the starting angle and velocity was sufficient to reach the other side. The goal was to determine the best launch angle given a variety of adjustments to the system, and to determine the speed of the rock as it reaches the bottom. I included forces from drag force, different wind speeds in multiple directions, and the inclusion of air densities.

## Homework 3 - Bifurcation of Logistic Map

In this assignment, I was tasked with determining the transition to chaos through period doubling of the logistic map $x_n = \mu x_n(1-x_n)$
