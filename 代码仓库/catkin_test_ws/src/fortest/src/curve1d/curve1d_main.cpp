#include <iostream>
#include <array>

#include "quintic_polynomial_curve1d.h"

using std::cout; using std::endl;

int main()
{
    std::array<double, 3> start_point = {1.0, 1.0, 1.0};
    std::array<double, 3> end_point = {5.0, 5.0, 5.0};
    double param = 5;
    QuinticPolynomialCurve1d quintic_p(start_point,end_point,param);
    cout << quintic_p.ToString() << endl;
    
    return 0;
}