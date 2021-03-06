//
//  City.hpp
//  ME-791_SF_Project_Gamma_project
//
//  Created by Scott S Forer on 3/14/17.
//  Copyright © 2017 Scott S Forer. All rights reserved.
//

#ifndef City_hpp
#define City_hpp

#include <stdio.h>

using namespace std;

class City
{
    friend class Parameters;
    friend class Agent;
    friend class Policy;
    friend class EA;
protected:
    
public:
    int location;
    double x_location;
    double y_location;
    vector<double> distance_to_cities;
};

#endif /* City_hpp */
