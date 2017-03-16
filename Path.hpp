//
//  Path.hpp
//  ME-791_SF_Project_Gamma_project
//
//  Created by Scott S Forer on 3/14/17.
//  Copyright © 2017 Scott S Forer. All rights reserved.
//

#ifndef Path_hpp
#define Path_hpp

#include <stdio.h>

using namespace std;

class Path
{
    friend class Parameters;
    friend class EA;
    friend class City;
    
protected:
    
public:
    vector<City> town;
    double total_dist_traveled;
    double fitness;
};

#endif /* Path_hpp */
