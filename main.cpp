//
//  main.cpp
//  CS-776_Assingment_4
//
//  Created by Scott S Forer on 10/6/16.
//  Copyright © 2016 Scott S Forer. All rights reserved.
//

#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <cassert>

using namespace std;


class City
{
public:
    int location;
    double x_location;
    double y_location;
    vector<double> distance_to_cities;
    int selected;
};


class Path
{
public:
    vector<City> town;
    double total_dist_traveled;
    double fitness;
};

vector<Path> tour;


//Parameters
int x_dim_max = 50;
int y_dim_max = 50;
int pop_size = 50;
int num_cities = 100;
int to_kill = pop_size/2;
int kill;
int max_gen = 5000;


//Statistics
int num_trials = 30;
vector<double> max_fitness;
vector<double> ave_fitness;
vector<double> min_fitness;
double ave_max_fitness;
double ave_ave_fitness;
double ave_min_fitness;
vector<double> ave_of_ave_max;
vector<double> ave_of_ave_ave;
vector<double> ave_of_ave_min;



//-----------------------------------------------------------
//Creates the city locations for from city 1 to num_cities
void create_city_locations()
{
    //creates a random city location for num_cities starting at city 1
    for (int c=1; c<num_cities; c++)
    {
        tour.at(0).town.at(c-1).location = c;
        tour.at(0).town.at(c-1).x_location = abs(((double)rand()/RAND_MAX)*x_dim_max);
        tour.at(0).town.at(c-1).y_location = abs(((double)rand()/RAND_MAX)*y_dim_max);
    }
    
    
    //copies the cities locations for each tour
    for (int pop=0; pop<pop_size; pop++)
    {
        for (int c=1; c<num_cities; c++)
        {
            tour.at(pop).town.at(c-1).location = tour.at(0).town.at(c-1).location;
            tour.at(pop).town.at(c-1).x_location = tour.at(0).town.at(c-1).x_location;
            tour.at(pop).town.at(c-1).y_location = tour.at(0).town.at(c-1).y_location;
        }
    }
}


//-----------------------------------------------------------
//Creates the startig and ending city
void create_starting_city_location()
{
    //sets the starting city and ending city for the tour
    for (int pop=0; pop<pop_size; pop++)
    {
        tour.at(pop).town.at(0).location = 0;
        tour.at(pop).town.at(0).x_location = 0;
        tour.at(pop).town.at(0).y_location = 0;
        tour.at(pop).town.at(num_cities).location = 0;
        tour.at(pop).town.at(num_cities).x_location = 0;
        tour.at(pop).town.at(num_cities).y_location = 0;
    }
}


//-----------------------------------------------------------
//Builds population
void build_pop()
{
    for (int pop=0; pop<pop_size; pop++)
    {
        Path P;
        tour.push_back(P);
        for (int c=0; c<num_cities-1; c++)
        {
            City C;
            tour.at(pop).town.push_back(C);
        }
    }
    
    create_city_locations();
    
    //randomly shuffles the order in the path for each path in the population
    for (int pop=0; pop<pop_size; pop++)
    {
        random_shuffle(tour.at(pop).town.begin(), tour.at(pop).town.end());
    }
    
    //inserts an instance for the starting and ending city
    for (int pop=0; pop<pop_size; pop++)
    {
        assert(tour.at(pop).town.size() == num_cities-1);
        City C;
        tour.at(pop).town.insert(tour.at(pop).town.begin() + 0, C);
        assert(tour.at(pop).town.size() == num_cities);
        tour.at(pop).town.push_back(C);
        assert(tour.at(pop).town.size() == num_cities+1);
    }
    
    create_starting_city_location();
    
    
    /*
    for (int pop=0; pop<pop_size; pop++)
    {
        cout << "Path" << "\t" << pop << endl;
        for (int c=0; c<num_cities+1; c++)
        {
            cout << tour.at(pop).town.at(c).location << "\t";
        }
        cout << endl;
        cout << endl;
    }
    */
    
    
     
   /*
    for (int pop=0; pop<pop_size; pop++)
    {
        cout << "Path" << "\t" << pop << endl;
        for (int c=0; c<num_cities+1; c++)
        {
            cout << "City" << "\t" << tour.at(pop).town.at(c).location << endl;
            cout << "X loaction" << "\t" << tour.at(pop).town.at(c).x_location << endl;
            cout << "Y loaction" << "\t" << tour.at(pop).town.at(c).y_location << endl;
        }
        cout << endl;
    }
    cout << endl;
    */
}


//-----------------------------------------------------------
//Calculates the distance to each city form each cities perspective
void get_distances_to_cities()
{
    for (int c=0; c<num_cities+1; c++)
    {
        for (int cc=0; cc<num_cities+1; cc++)
        {
            double dist = 0;
            double x = 0;
            double y = 0;
            x = tour.at(0).town.at(c).x_location - tour.at(0).town.at(cc).x_location;
            y = tour.at(0).town.at(c).y_location - tour.at(0).town.at(cc).y_location;
            dist = sqrt((x*x)+(y*y));
            tour.at(0).town.at(c).distance_to_cities.push_back(dist);
        }
    }
    
    
    for (int pop=1; pop<pop_size; pop++)
    {
        for (int c=0; c<num_cities+1; c++)
        {
            for (int cc=0; cc<num_cities+1; cc++)
            {
                    if (tour.at(pop).town.at(c).location == tour.at(0).town.at(cc).location)
                    {
                        //cout << "check" << "\t" << c << "\t" << cc << endl;
                        //cout << "in" << endl;
                        //cout << tour.at(0).town.at(c).distance_to_cities.at(cc) << endl;
                        for (int ccc=0; ccc< num_cities+1; ccc++)
                        {
                          tour.at(pop).town.at(c).distance_to_cities.push_back(tour.at(0).town.at(cc).distance_to_cities.at(ccc));
                        }
                        //cout << "out " << endl;
                        //cout << tour.at(pop).town.at(c).distance_to_cities.at(cc) << endl;
                    }
            }
        }
    }
    //cout << endl;
    //cout << endl;
    
    /*
    for (int pop=0; pop<pop_size; pop++)
    {
        cout << "Path" << "\t" << pop << endl;
        for (int c=0; c<num_cities+1; c++)
        {
            cout << "stop" << "\t" << c << endl;
            for (int cc=0; cc<num_cities+1; cc++)
            {
                cout << tour.at(pop).town.at(c).distance_to_cities.at(cc) << endl;
            }
            cout << endl;
        }
    }
    */
    
}



//-----------------------------------------------------------
//Calculates the total distance traveld given a path
void get_total_dist_traveled()
{
    for (int pop=0; pop<pop_size; pop++)
    {
        double sum = 0;
        for (int c=0; c<num_cities; c++)
        {
            //cout << "in " << endl;
            sum += tour.at(pop).town.at(c).distance_to_cities.at(tour.at(pop).town.at(c+1).location);
            //cout << "out " << endl;
            //cout << sum << endl;
        }
        //cout << endl;
        //cout << endl;
        tour.at(pop).total_dist_traveled = sum;
        //cout << "Path" << "\t" << pop << endl;
        //cout << "total travel distance" << "\t" << tour.at(pop).total_dist_traveled << endl;
    }
    //cout << endl;
}


//-----------------------------------------------------------
//Calculates the fitness for each path
void get_fitness()
{
    for (int pop=0; pop<pop_size; pop++)
    {
        tour.at(pop).fitness = 0;
        tour.at(pop).fitness = tour.at(pop).total_dist_traveled;
        //cout << "path" << "\t" << pop << "\t" << "fitness" << "\t" << tour.at(pop).fitness << endl;
    }
    //cout << endl;
}


//-----------------------------------------------------------
//randomly selects two individuals and decides which one will die based on their fitness
int binary_select()
{
    int loser;
    int index_1 = rand() % tour.size();
    int index_2 = rand() % tour.size();
    while (index_1 == index_2)
    {
        index_2 = rand() % tour.size();
    }
    if(tour.at(index_1).fitness < tour.at(index_2).fitness)
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
//mutates the copies of the winning individuals
void mutation(Path &M)
{
    //add for loop here for nmber of times cities are swapped
    //randomly selects two cities that are not the first of last city
    //City C1;
    int index_c1 = (int)rand() % num_cities;
    while (index_c1 == 0 || index_c1 == num_cities+1)
    {
        index_c1 = (int)rand() % num_cities;
    }
    
    //cout << "index 1" << "\t" << index_c1 << endl;
    //cout << "index 2 location" << "\t" << M.town.at(index_c1).location << endl;
    //C1 = M.town.at(index_c1);        //copies town.at(index_1) info
    //cout << C1.location << endl;
    
    //City C2;
    int index_c2 = (int)rand() % num_cities;
    while (index_c1 == index_c2 || index_c2 == 0 || index_c2 == num_cities+1)
    {
        index_c2 = (int)rand() % num_cities;
    }
    
    //cout << "index 2" << "\t" << index_c2 << endl;
    //cout << "index 2 location" << "\t" << M.town.at(index_c2).location << endl;
    //C2 = M.town.at(index_c2);        //copies town.at(index_2) info
    //cout << C2.location << endl;
    
    for(int c=0; c<num_cities+1; c++)
    {
        M.town.at(c).selected = 0;
    }
    
    //swaps the info for the two randomly selected cities
    swap(M.town.at(index_c1), M.town.at(index_c2));
    /*
    for (int c=0; c<num_cities; c++)
    {
        if (c == index_c1)
        {
            if (M.town.at(index_c1).selected == 0)
            {
                M.town.at(c) = C2;
                M.town.at(c).selected = 1;
            }
        }
        if (c == index_c2)
        {
            if (M.town.at(index_c2).selected == 0)
            {
                M.town.at(c) = C1;
                M.town.at(c).selected = 1;
            }
        }
    }
    /*
    cout << "New Path" << "\t" << endl;
    for (int c=0; c<num_cities+1; c++)
    {
        cout << M.town.at(c).location << "\t";
    }
    cout << endl;
     */
}



//-----------------------------------------------------------
//runs the natural selection process
void natural_selection()
{
    for(int k=0; k<to_kill; k++)
    {
        kill = binary_select();
        tour.erase(tour.begin() + kill);
    }
    
    int to_replicate = to_kill;
    for (int r=0; r<to_replicate; r++)
    {
        //cout << r << endl;
        Path M;
        int spot = rand() % tour.size();
        /*
        cout << "path to mutate" << endl;
        cout << tour.at(spot).fitness << endl;
        for (int c=0; c<num_cities+1; c++)
        {
            cout << tour.at(spot).town.at(c).location << "\t";
        }
        cout << endl;
        */
        M = tour.at(spot);
        mutation(M);
        tour.push_back(M);
        //cout << "rend = " << rRrR << endl;
    }
}


void re_initialize_pop()
{
    for (int pop=0; pop<pop_size; pop++)
    {
        tour.at(pop).town.erase(tour.at(pop).town.begin()+0);
        tour.at(pop).town.erase(tour.at(pop).town.begin()+num_cities-1);
    }
    
    /*
    for (int pop=0; pop<pop_size; pop++)
    {
        cout << "Path" << "\t" << pop << endl;
        for (int c=0; c<num_cities-1; c++)
        {
            cout << tour.at(pop).town.at(c).location << "\t";
        }
        cout << endl;
        cout << endl;
    }
    */
     
    //randomly shuffles the order in the path for each path in the population
    for (int pop=0; pop<pop_size; pop++)
    {
        random_shuffle(tour.at(pop).town.begin(), tour.at(pop).town.end());
    }
    
    //inserts an instance for the starting and ending city
    for (int pop=0; pop<pop_size; pop++)
    {
        City C;
        tour.at(pop).town.insert(tour.at(pop).town.begin() + 0, C);
        tour.at(pop).town.push_back(C);
    }
    
    for (int pop=0; pop<pop_size; pop++)
    {
        tour.at(pop).town.at(0).location = 0;
        tour.at(pop).town.at(0).x_location = 0;
        tour.at(pop).town.at(0).y_location = 0;
        tour.at(pop).town.at(num_cities).location = 0;
        tour.at(pop).town.at(num_cities).x_location = 0;
        tour.at(pop).town.at(num_cities).y_location = 0;
    }

    
    for (int pop=0; pop<pop_size; pop++)
    {
        for (int c=0; c<num_cities+1; c++)
        {
            double dist = 0;
            double x = 0;
            double y = 0;
            x = tour.at(pop).town.at(0).x_location - tour.at(pop).town.at(c).x_location;
            y = tour.at(pop).town.at(0).y_location - tour.at(pop).town.at(c).y_location;
            dist = sqrt((x*x)+(y*y));
            tour.at(pop).town.at(0).distance_to_cities.push_back(dist);
            tour.at(pop).town.at(num_cities).distance_to_cities.push_back(dist);
        }
    }
    
    /*
    for (int pop=0; pop<pop_size; pop++)
    {
        cout << "Path" << "\t" << pop << endl;
        for (int c=0; c<num_cities+1; c++)
        {
            cout << "stop" << "\t" << c << endl;
            for (int cc=0; cc<num_cities+1; cc++)
            {
                cout << tour.at(pop).town.at(c).distance_to_cities.at(cc) << endl;
            }
            cout << endl;
        }
    }
    */
    
    /*
    for (int pop=0; pop<pop_size; pop++)
    {
        cout << "Path" << "\t" << pop << endl;
        for (int c=0; c<num_cities+1; c++)
        {
            cout << tour.at(pop).town.at(c).location << "\t";
        }
        cout << endl;
        cout << endl;
    }
    */
    
}


//-----------------------------------------------------------
//sorts the population based on their fitness from highest to lowest
struct less_than_par_fitness
{
    inline bool operator() (const Path& struct1, const Path& struct2)
    {
        return (struct1.fitness < struct2.fitness);
    }
};


//-----------------------------------------------------------
//copies the max, ave, and min fitness
void run_scoreboard()
{
    min_fitness.push_back(tour.at(0).fitness);
    double sum = 0;
    for (int p=0; p<pop_size; p++)
    {
        sum += tour.at(p).fitness;
    }
    ave_fitness.push_back(sum/pop_size);
    max_fitness.push_back(tour.at(pop_size-1).fitness);
    
}


//-----------------------------------------------------------
//gets the ave of the ave
void get_ave()
{
    for (int gen=0; gen<max_gen; gen++)
    {
        double ave_max_sum = 0;
        double ave_ave_sum = 0;
        double ave_min_sum = 0;
        for (int trial=0; trial<num_trials; trial++)
        {
            ave_min_sum += max_fitness.at(gen+trial*max_gen);
        }
        ave_min_fitness = ave_min_sum/num_trials;
        ave_of_ave_min.push_back(ave_min_fitness);
        for (int trial=0; trial<num_trials; trial++)
        {
            ave_ave_sum += ave_fitness.at(gen+trial*max_gen);
        }
        ave_ave_fitness = ave_ave_sum/num_trials;
        ave_of_ave_ave.push_back(ave_ave_fitness);
        for (int trial=0; trial<num_trials; trial++)
        {
            ave_max_sum += max_fitness.at(gen+trial*max_gen);
        }
        ave_max_fitness = ave_max_sum/num_trials;
        ave_of_ave_max.push_back(ave_max_fitness);
    }
}


//-----------------------------------------------------------
//writes the max, ave, and min fitness for each generation to a txt file
void write_pop_info_all_gens_to_text()
{
    ofstream File1;
    File1.open("Min Fitness For All Gens.txt");
    ofstream File2;
    File2.open("Ave Fitness For All Gens.txt");
    ofstream File3;
    File3.open("Max Fitness For All Gens.txt");
    ofstream File4;
    File4.open("Ave Min Fitness For All Trails");
    ofstream File5;
    File5.open("Ave Ave Fitness For All Trails");
    ofstream File6;
    File6.open("Ave Max Fitness For All Trails");
    //cout << "fitness across generations" << endl;
    //cout << "max fitness" << endl;
    for (int i=0; i<max_gen*num_trials; i++)
    {
        File1 << min_fitness.at(i) << endl;
    }
    //cout << endl;
    //cout << "ave fitness" << endl;
    for (int i=0; i<max_gen*num_trials; i++)
    {
        File2 << ave_fitness.at(i) << endl;
    }
    //cout << endl;
    //cout << "min fitness" << endl;
    for (int i=0; i<max_gen*num_trials; i++)
    {
        File3 << max_fitness.at(i) << endl;
    }
    for (int i=0; i<max_gen; i++)
    {
        File4 << ave_of_ave_min.at(i) << endl;
    }
    for (int i=0; i<max_gen; i++)
    {
        File5 << ave_of_ave_ave.at(i) << endl;
    }
    for (int i=0; i<max_gen; i++)
    {
        File6 << ave_of_ave_max.at(i) << endl;
    }
    //cout << endl;
    //cout << endl;
    File1.close();
    File2.close();
    File3.close();
    File4.close();
    File5.close();
    File6.close();
}


//-----------------------------------------------------------
//Runs the entire TSP program
void run_TSP()
{
    build_pop();
    get_distances_to_cities();
    for (int trial=0; trial<num_trials; trial++)
    {
        cout << "------------------------------------" << endl;
        cout << "trial" << "\t" << trial << endl;
        cout << endl;
        if (trial>0)
        {
            re_initialize_pop();
            //get_distances_to_cities();
        }
        for (int gen=0; gen<max_gen; gen++)
        {
            if (gen % 100 == 0)
            {
                cout << "generation" << "\t" << gen << endl;
                cout << endl;
            }
            
            get_total_dist_traveled();
            get_fitness();
            sort(tour.begin(), tour.end(), less_than_par_fitness());
            natural_selection();
            get_total_dist_traveled();
            get_fitness();
            
            sort(tour.begin(), tour.end(), less_than_par_fitness());
            run_scoreboard();
            
            /*
            if (gen == max_gen-1)
            {
                cout << "generation" << "\t" << gen << endl;
            }
            if (gen == max_gen-1)
            {
                cout << "Final Population" << endl;
                for (int pop=0; pop<pop_size; pop++)
                {
                    cout << "Path" << "\t" << pop << endl;
                    cout << "Fitness" << "\t" << tour.at(pop).fitness << endl;
                    for (int c=0; c<num_cities+1; c++)
                    {
                        cout << tour.at(pop).town.at(c).location << "\t";
                    }
                    cout << endl;
                    cout << endl;
                }
            }
            */
            
            /*
             for (int pop=0; pop<pop_size; pop++)
             {
             cout << "Path" << "\t" << pop << endl;
             cout << "fitness" << "\t" << tour.at(pop).fitness << endl;
             for (int c=0; c<num_cities+1; c++)
             {
             cout << tour.at(pop).town.at(c).location << "\t";
             }
             cout << endl;
             cout << endl;
             }
             */
        }
    }
    get_ave();
    write_pop_info_all_gens_to_text();
}




int main()
{
    srand(time(NULL));
    run_TSP();
    
    return 0;
}
