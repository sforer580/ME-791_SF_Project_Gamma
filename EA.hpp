//
//  EA.hpp
//  ME-791_SF_Project_Gamma_project
//
//  Created by Scott S Forer on 3/14/17.
//  Copyright © 2017 Scott S Forer. All rights reserved.
//

#ifndef EA_hpp
#define EA_hpp

#include <stdio.h>

using namespace std;

class EA
{
    friend class Parameters;
    friend class Agent;
    friend class Policy;
    friend class City;
    
protected:
    
public:
    Parameters* pP;
    vector<Agent> individual;
    
    void Initialize_Population();
    void Create_City_Locations();
    void Get_Distances_To_Cities();
    void Display_Path_Info();
    void Get_Total_Dist_Traveled();
    void Get_Fitness();
    void Evaluate();
    int Binary_Select();
    void Down_Select();
    void Mutation(Policy &M);
    void Replicate();
    void Re_Initialize_Population();
    struct Less_Than_Path_Fitness;
    void Run_TSP();
    
    
    //Statistics
    vector<double> max_fitness;
    vector<double> ave_fitness;
    vector<double> min_fitness;
    void Run_Scoreboard();
    void Run_Text_File_Functions();
    void Delete_Text_Files();
};



//-----------------------------------------------------------
//Initializes the population
void EA::Initialize_Population()
{
    Agent A;
    individual.push_back(A);
    for (int p=0; p<pP->pop_size; p++)
    {
        Policy PO;
        individual.at(0).path.push_back(PO);
        for (int c=0; c<pP->num_cities; c++)
        {
            City C;
            individual.at(0).path.at(p).town.push_back(C);
        }
    }
    
    Create_City_Locations();
    Get_Distances_To_Cities();
    //Display_Path_Info();
    
    //randomly shuffles the order in the path for each path in the population
    for (int p=0; p<pP->pop_size; p++)
    {
        random_shuffle(individual.at(0).path.at(p).town.begin() + 1, individual.at(0).path.at(p).town.end());
    }
    
    //inserts an instance for the starting and ending city
    for (int p=0; p<pP->pop_size; p++)
    {
        assert(individual.at(0).path.at(p).town.size() == pP->num_cities);
    }
    
    //Display_Path_Info();
}


//-----------------------------------------------------------
//Creates the city locations for from city 1 to pP->num_cities
void EA::Create_City_Locations()
{
    //creates a random city location for pP->num_cities starting at city 1
    for (int c=0; c<pP->num_cities; c++)
    {
        individual.at(0).path.at(0).town.at(c).location = c;
        individual.at(0).path.at(0).town.at(c).x_location = abs(((double)rand()/RAND_MAX)*pP->x_dim_max);
        individual.at(0).path.at(0).town.at(c).y_location = abs(((double)rand()/RAND_MAX)*pP->y_dim_max);
    }
    
    //copies the cities locations for each tour
    for (int p=1; p<pP->pop_size; p++)
    {
        for (int c=0; c<pP->num_cities; c++)
        {
            individual.at(0).path.at(p).town.at(c).location = individual.at(0).path.at(0).town.at(c).location;
            individual.at(0).path.at(p).town.at(c).x_location = individual.at(0).path.at(0).town.at(c).x_location;
            individual.at(0).path.at(p).town.at(c).y_location = individual.at(0).path.at(0).town.at(c).y_location;
        }
    }
}


//-----------------------------------------------------------
//Calculates the distance to each city form each cities perspective
void EA::Get_Distances_To_Cities()
{
    for (int c=0; c<pP->num_cities; c++)
    {
        for (int cc=0; cc<pP->num_cities; cc++)
        {
            double dist = 0;
            double x = 0;
            double y = 0;
            x = individual.at(0).path.at(0).town.at(c).x_location - individual.at(0).path.at(0).town.at(cc).x_location;
            y = individual.at(0).path.at(0).town.at(c).y_location - individual.at(0).path.at(0).town.at(cc).y_location;
            dist = sqrt((x*x)+(y*y));
            individual.at(0).path.at(0).town.at(c).distance_to_cities.push_back(dist);
        }
    }
    
    for (int p=1; p<pP->pop_size; p++)
    {
        for (int c=0; c<pP->num_cities; c++)
        {
            for (int cc=0; cc<pP->num_cities; cc++)
            {
                individual.at(0).path.at(p).town.at(c).distance_to_cities.push_back(individual.at(0).path.at(0).town.at(c).distance_to_cities.at(cc));
            }
        }
    }
}


//-----------------------------------------------------------
//Displays the information for each path
void EA::Display_Path_Info()
{
    for (int p=0; p<pP->pop_size; p++)
    {
        cout << "PATH" << "\t" << p << endl;
        for (int c=0; c<pP->num_cities; c++)
        {
            cout << individual.at(0).path.at(p).town.at(c).location << "\t";
        }
        cout << endl;
    }
    cout << endl;
    
    for (int p=0; p<pP->pop_size; p++)
    {
        cout << "PATH" << "\t" << p << endl;
        for (int c=0; c<pP->num_cities; c++)
        {
            cout << "CITY" << "\t" << individual.at(0).path.at(p).town.at(c).location << endl;
            cout << "X LOCATION" << "\t" << individual.at(0).path.at(p).town.at(c).x_location << endl;
            cout << "Y LOCATION" << "\t" << individual.at(0).path.at(p).town.at(c).y_location << endl;
        }
        cout << endl;
    }
    cout << endl;
    
    for (int p=0; p<pP->pop_size; p++)
    {
        cout << "PATH" << "\t" << p << endl;
        for (int c=0; c<pP->num_cities; c++)
        {
            cout << "STOP" << "\t" << c << "\t" << "CITY" << "\t" << individual.at(0).path.at(p).town.at(c).location <<  endl;
            for (int cc=0; cc<pP->num_cities; cc++)
            {
                cout << "DISTANCE TO CITY" << "\t" << cc << "\t" << individual.at(0).path.at(p).town.at(c).distance_to_cities.at(cc) << endl;
            }
            cout << endl;
        }
    }
    cout << endl;
}


//-----------------------------------------------------------
//Calculates the total distance traveld given a path
void EA::Get_Total_Dist_Traveled()
{
    for (int p=0; p<pP->pop_size; p++)
    {
        double sum = 0;
        for (int c=0; c<pP->num_cities-1; c++)
        {
            //cout << "in " << endl;
            sum += individual.at(0).path.at(p).town.at(c).distance_to_cities.at(individual.at(0).path.at(p).town.at(c+1).location);
            //cout << "out " << endl;
            //cout << sum << endl;
        }
        //cout << endl;
        individual.at(0).path.at(p).total_dist_traveled = sum;
        //cout << "PATH" << "\t" << pop << endl;
        //cout << "TOTAL DISTACNE TRAVELED" << "\t" << tour.at(pop).total_dist_traveled << endl;
        //cout << endl;
    }
    //cout << endl;
}


//-----------------------------------------------------------
//Calculates the fitness for each path
void EA::Get_Fitness()
{
    for (int p=0; p<pP->pop_size; p++)
    {
        individual.at(0).path.at(p).fitness = 0;
        individual.at(0).path.at(p).fitness = individual.at(0).path.at(p).total_dist_traveled;
        //cout << "path" << "\t" << pop << "\t" << "fitness" << "\t" << tour.at(pop).fitness << endl;
    }
    //cout << endl;
}


//-----------------------------------------------------------
//Runs the evaluation process
void EA::Evaluate()
{
    Get_Total_Dist_Traveled();
    Get_Fitness();
}


//-----------------------------------------------------------
//Randomly selects two individuals and decides which one will die based on their fitness
int EA::Binary_Select()
{
    int loser;
    int index_1 = rand() % individual.at(0).path.size();
    int index_2 = rand() % individual.at(0).path.size();
    while (index_1 == index_2)
    {
        index_2 = rand() % individual.at(0).path.size();
    }
    if(individual.at(0).path.at(index_1).fitness < individual.at(0).path.at(index_2).fitness)
    {
        loser = index_2;
    }
    else
    {
        loser = index_1;
    }
    return loser;
}


//-----------------------------------------------------------
//Runs the down select process
void EA::Down_Select()
{
    for(int k=0; k<pP->to_kill; k++)
    {
        int kill = 0;
        kill = Binary_Select();
        individual.at(0).path.erase(individual.at(0).path.begin() + kill);
    }
}


//-----------------------------------------------------------
//Mutates the copies of the winning individuals
void EA::Mutation(Policy &M)
{
    for (int s=0; s<pP->num_swaps; s++)
    {
        //randomly selects two cities that are not the first of last city
        int index_c1 = (int)rand() % pP->num_cities;
        while (index_c1 == 0)
        {
            index_c1 = (int)rand() % pP->num_cities;
        }
        int index_c2 = (int)rand() % pP->num_cities;
        while (index_c1 == index_c2 || index_c2 == 0)
        {
            index_c2 = (int)rand() % pP->num_cities;
        }
        
        //swaps the info for the two randomly selected cities
        swap(M.town.at(index_c1), M.town.at(index_c2));
    }
}


//-----------------------------------------------------------
//Runs the down select process
void EA::Replicate()
{
    int to_replicate = pP->to_kill;
    for (int r=0; r<to_replicate; r++)
    {
        //cout << r << endl;
        Policy M;
        int spot = rand() % individual.at(0).path.size();
        M = individual.at(0).path.at(spot);
        Mutation(M);
        individual.at(0).path.push_back(M);
    }
}


//-----------------------------------------------------------
//Re-initilaizes the paths
void EA::Re_Initialize_Population()
{
    min_fitness.clear();
    ave_fitness.clear();
    max_fitness.clear();
    //randomly shuffles the order in the path for each path in the population
    for (int p=0; p<pP->pop_size; p++)
    {
        random_shuffle(individual.at(0).path.at(p).town.begin()+1, individual.at(0).path.at(p).town.end());
    }
    //Display_Path_Info();
}


//-----------------------------------------------------------
//sorts the population based on their fitness from highest to lowest
struct EA::Less_Than_Path_Fitness
{
    inline bool operator() (const Policy& struct1, const Policy& struct2)
    {
        return (struct1.fitness < struct2.fitness);
    }
};


//-----------------------------------------------------------
//copies the max, ave, and min fitness
void EA::Run_Scoreboard()
{
    min_fitness.push_back(individual.at(0).path.at(0).fitness);
    double sum = 0;
    for (int p=0; p<pP->pop_size; p++)
    {
        sum += individual.at(0).path.at(p).fitness;
    }
    ave_fitness.push_back(sum/pP->pop_size);
    max_fitness.push_back(individual.at(0).path.at(pP->pop_size-1).fitness);
    
}


//-------------------------------------------------------------------------
//Deletes text files
void EA::Delete_Text_Files()
{
    
    if( remove( "Min_Fitness.txt" ) != 0 )
        perror( "ERROR DELETING FILE Min_Fitness" );
    else
        puts( "Min_Fitness FILE SUCCEDDFULLY DELETED" );
    cout << endl;
    
    
    if( remove( "Ave_Fitness.txt" ) != 0 )
        perror( "ERROR DELETING FILE Ave_Fitness" );
    else
        puts( "Ave_Fitness FILE SUCCEDDFULLY DELETED" );
    cout << endl;
    
    if( remove( "Max_Fitness.txt" ) != 0 )
        perror( "ERROR DELETING FILE Max_Fitness" );
    else
        puts( "Max_Fitness FILE SUCCEDDFULLY DELETED" );
    cout << endl;
}


//-----------------------------------------------------------
//writes the max, ave, and min fitness for each generation to a txt file
void EA::Run_Text_File_Functions()
{
    ofstream File1;
    File1.open("Min_Fitness.txt", ios_base::app);
    ofstream File2;
    File2.open("Ave_Fitness.txt", ios_base::app);
    ofstream File3;
    File3.open("Max_Fitness.txt", ios_base::app);

    for (int i=0; i<pP->max_gen; i++)
    {
        File1 << min_fitness.at(i) << "\t";
    }
    File1 << endl;

    
    for (int i=0; i<pP->max_gen; i++)
    {
        File2 << ave_fitness.at(i) << "\t";
    }
    File2 << endl;
    
    for (int i=0; i<pP->max_gen; i++)
    {
        File3 << max_fitness.at(i) << "\t";
    }
    File3 << endl;
    File1.close();
    File2.close();
    File3.close();
}


//-----------------------------------------------------------
//Runs the entire TSP program
void EA::Run_TSP()
{
    Delete_Text_Files();
    Initialize_Population();
    for (int sr=0; sr<pP->num_sr; sr++)
    {
        cout << "------------------------------------" << endl;
        cout << "SR" << "\t" << sr << endl;
        cout << endl;
        if (sr>0)
        {
            Re_Initialize_Population();
        }
        for (int gen=0; gen<pP->max_gen; gen++)
        {
            if (gen % 100 == 0)
            {
                cout << "SR" << "\t" << sr << "::" << gen << endl;
                cout << endl;
            }
            Evaluate();
            sort(individual.at(0).path.begin(), individual.at(0).path.end(), Less_Than_Path_Fitness());
            Run_Scoreboard();
            Down_Select();
            Replicate();
        }
        Run_Text_File_Functions();
    }
}

#endif /* EA_hpp */
