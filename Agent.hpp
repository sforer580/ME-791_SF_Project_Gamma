//
//  Agent.hpp
//  ME-791_SF_Project_Gamma_project
//
//  Created by Scott S Forer on 3/16/17.
//  Copyright © 2017 Scott S Forer. All rights reserved.
//

#ifndef Agent_hpp
#define Agent_hpp

#include <stdio.h>

using namespace std;

class Agent
{
    friend class Parameters;
    friend class EA;
    friend class Policy;
    friend class City;
    
protected:
    
public:
    vector<Policy> path;
};


#endif /* Agent_hpp */
