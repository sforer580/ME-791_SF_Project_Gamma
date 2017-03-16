//
//  Policy.hpp
//  ME-791_SF_Project_Gamma_project
//
//  Created by Scott S Forer on 3/16/17.
//  Copyright © 2017 Scott S Forer. All rights reserved.
//

#ifndef Policy_hpp
#define Policy_hpp

#include <stdio.h>

using namespace std;

class Policy
{
    friend class Parameters;
    friend class Agent;
    friend class EA;
    friend class City;
    
protected:
    
public:
    vector<City> town;
    double total_dist_traveled;
    double fitness;
};


#endif /* Policy_hpp */
