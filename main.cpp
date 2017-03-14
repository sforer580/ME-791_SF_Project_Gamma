//
//  main.cpp
//  CS-776_Assingment_4
//
//  Created by Scott S Forer on 10/6/16.
//  Copyright © 2016 Scott S Forer. All rights reserved.
//

#include <iostream>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <time.h>
#include <stdio.h>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <cassert>
#include <ctime>
#include <random>

#include "Parameters.hpp"
#include "City.hpp"
#include "Path.hpp"
#include "EA.hpp"


using namespace std;


int main()
{
    srand(time(NULL));
    Parameters P;
    EA E;
    E.pP = &P;
    E.run_TSP();
    
    return 0;
}
