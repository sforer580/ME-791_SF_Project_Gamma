//
//  Parameters.hpp
//  ME-791_SF_Project_Gamma_project
//
//  Created by Scott S Forer on 3/14/17.
//  Copyright © 2017 Scott S Forer. All rights reserved.
//

#ifndef Parameters_hpp
#define Parameters_hpp

#include <stdio.h>

using namespace std;

class Parameters
{
    friend class EA;
    friend class Path;
    friend class City;
    
protected:
    
public:
    //Parameters
    int x_dim_max = 50;
    int y_dim_max = 50;
    int pop_size = 200;
    int num_cities = 25;
    int to_kill = pop_size/2;
    int num_swaps = 1;
    int max_gen = 4000;
    int num_sr = 1;
};

#endif /* Parameters_hpp */
