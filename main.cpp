
//HEader Files
#include <string.h>

#include <unistd.h>

#include <math.h>

#include <time.h>

#include <sys/time.h>

#include <ctime>

#include <vector>

#include <iostream>

#include <fstream>

#include <stdlib.h>

#include<stdio.h>





#define EPS 1e-3

//#define DEBUG_PRINT



using namespace std;



// global variables

int N, K, *weight, *Capacity, *linCoef, **quadCoef;


// Solution is stored in this structure
struct Solution

{

    vector<vector<int> > Knapsack;

    vector<int> Capacity_used;

    double Cost;

    vector<int> selected;

};


// Function to store inputs from an instance
// This is a function including a quadratic function
int loadInstance()

{

    cout << "Started loading the instance... " ;



    // define the variables used in this function

    char trash[100];



    // open the file

    ifstream file("/Users/ranjith/Desktop/Knapsack/QMKP/3 Knapsacks/Instance10.txt");

    ofstream File("WeightsValues.txt");

    if (file==NULL)

    {

        cout << "Error opening data file.";

        return(1);

    }



    // start loading the data



    file >> trash ; // first line with the reference for the name of the instance



    file >> N; // number of items

    cout<<N<<" ";



    // allocate memory for the variables

    // if we allocate memory for a vector of 10 elements, the elements will be from 0 to 9

    // to facilitate the notation, I declare the vectors with 11 elements, to access 1 to 10

    // and discard element 0

    weight = new int[N+1];

    linCoef = new int[N+1];

    quadCoef = new int*[N+1];

    for (int i=0; i<=N; i++)

    {

        quadCoef[i] = new int[N+1];

        // initialize all variables to zero

        for (int j=0; j<=N; j++)

        {

            quadCoef[i][j] = 0;

        }

        weight[i] = 0;

        linCoef[i] = 0;

    }

    file >> K;

    cout<<"The number of Knapsacks:"<<K<<endl;

    Capacity=new int[K+1];





    // read the linear coefficients

    for (int i=1; i<=N; i++)

    {

        file >> linCoef[i];

    }



    // read the quadratic coefficients

    for (int i=1; i<=N; i++)

    {

        for (int j=i+1; j<=N; j++)

        {

            file >> quadCoef[i][j];

        }

    }



    file >> trash;



    for(int i=1;i<=K;i++)

    {

        file >> Capacity[i];

        cout<<"Capacity"<<Capacity[i]<<endl;

    }



    double num=0;

    for (int i=1; i<=N; i++)

    {

        file >> weight[i];

        num+=weight[i];



    }

    cout<<"Weight:"<<num<<endl;

    for(int i=1;i<=N;i++)

    {

        File<<i<<"."<<linCoef[i]<<" "<<weight[i]<<endl;

    }



    cout << "finished!" << endl;

    return(1);

}



void CalcValue(Solution &Sol)

{

    Sol.Cost=0;





    for(int i=1;i<=K;i++)

    {



        for(int j=0;j<Sol.Knapsack[i].size();j++)

        {

            Sol.Cost += linCoef[Sol.Knapsack[i][j]];



            for(int f=0;f<j;f++)

            {

                if(Sol.Knapsack[i][f] > Sol.Knapsack[i][j])

                    {Sol.Cost += quadCoef[Sol.Knapsack[i][j]][Sol.Knapsack[i][f]];}

                else

                    {Sol.Cost += quadCoef[Sol.Knapsack[i][f]][Sol.Knapsack[i][j]];}

            }

        }



    }

}



int *InitValues(int num)

{

    int *array;



    array = (int *) calloc(num, sizeof(int));



    for (int i = 0; i < num; i++)

        array[i] = 0;



    return(array);

}



double *InitWeights(int num)

{

    double *array;



    array = (double *) calloc(num, sizeof(double));



    for (int i = 0; i < num; i++)

        array[i] = 1.0/num;



    return(array);

}



int SelectMethod(double *weight)

{

    int    pos;

    double choice;

    double range;



    // arbitrary choice

    choice = ((double) rand()) / RAND_MAX;



    // find range

    range = 0;

    pos = -1;

    do {

        pos++;

        range += weight[pos];

    } while (range <= choice);



    return(pos);



}



void Pack(Solution &temp)

{

    bool improve = true;



    while (improve)

    {

        improve = false;

        // try to pack as many items as possible in the first knapsacks

        for (int k1 = 1; k1<=K; k1++)

        {

            for (int k2=k1+1; k2<=K; k2++)

            {

                for (int pos_item2=0; pos_item2<temp.Knapsack[k2].size(); pos_item2++)

                {

                    //can we send the second item to the first knapsack?

                    if (temp.Capacity_used[k1] + weight[temp.Knapsack[k2][pos_item2]] <= Capacity[k1] )

                    {

                        // send it to k1

                        temp.Capacity_used[k1] += weight[temp.Knapsack[k2][pos_item2]];

                        temp.Capacity_used[k2] -= weight[temp.Knapsack[k2][pos_item2]];

                        temp.Knapsack[k1].push_back(temp.Knapsack[k2][pos_item2]);

                        temp.Knapsack[k2].erase( temp.Knapsack[k2].begin() + pos_item2);

                        improve = true;

                        break;

                    }

                }

            }

        }

    }



    CalcValue(temp);

}





void PrintSol(Solution Sol)

{

    cout << endl;

    cout << "--- COST = " << Sol.Cost << endl;

    cout << "--- Capacities used:" << endl;

    for (int k=1; k<=K; ++k)

    {

        cout << " " << Sol.Capacity_used[k] ;

        if (Sol.Capacity_used[k] > Capacity[k])

        {

            cout << "PROBLEM WITH CAPACITIES. " ;

        }

        cout << ":" ;

        for (int j=0; j<Sol.Knapsack[k].size(); j++)

        {

            cout << " " <<Sol.Knapsack[k][j];

        }

        cout << endl;



    }

    cout << "------------------------" << endl;

    //cin.get();



}



void Improve(Solution &temp)

{

    // try all combinations of switching item1 and item2



    bool improve = true;



    while (improve)

    {

        improve = false;





        // first case: between items in the solution

        for (int k1 = 1; k1<K; k1++)

        {

            for (int k2=k1+1; k2<=K; k2++)

            {

                for (int pos_item1=0; pos_item1<temp.Knapsack[k1].size(); pos_item1++)

                {

                    for (int pos_item2=0; pos_item2<temp.Knapsack[k2].size(); pos_item2++)

                    {

                        //can we invert?

                        if (temp.Capacity_used[k1] - weight[temp.Knapsack[k1][pos_item1]] + weight[temp.Knapsack[k2][pos_item2]] <= Capacity[k1] && temp.Capacity_used[k2] - weight[temp.Knapsack[k2][pos_item2]] + weight[temp.Knapsack[k1][pos_item1]] <= Capacity[k2])

                        {

                            CalcValue(temp);

                            double temp_cost = temp.Cost;



                            int item1 = temp.Knapsack[k1][pos_item1];

                            int item2 = temp.Knapsack[k2][pos_item2];



                            //invert

                            temp.Knapsack[k1][pos_item1] = item2;

                            temp.Knapsack[k2][pos_item2] = item1;



                            // good?

                            CalcValue(temp);

                            if (temp.Cost > temp_cost + 0.01)

                            {

                                // yes, it is good

                                improve = true;



                                temp.Capacity_used[k1] += weight[ item2 ] - weight[ item1 ];

                                temp.Capacity_used[k2] += weight[ item1 ] - weight[ item2 ];

                                break;

                            }

                            else

                            {

                                // undo

                                temp.Knapsack[k2][pos_item2] = item2;

                                temp.Knapsack[k1][pos_item1] = item1;

                            }

                        }

                    	if (improve) break;

                    }

                    if (improve) break;

                }

                if (improve) break;

            }

            if (improve) break;

        }



        //end of first case







         //second case: items in the solution and items not in the solution

        vector <int> nselected(0);//A vector which has all non selected item's number in it.



        for (int k1 = 1; k1<=K; k1++)

        {

            for (int pos_item1=0; pos_item1<temp.Knapsack[k1].size(); pos_item1++)

            {

                for(int items=1;items<=N;items++)

                {

                    nselected.clear();

                    if(temp.selected[items]==0)

                        nselected.push_back(items);

                }

                int Size = nselected.size();



                for (int i=0; i<Size; i++)

                {

                    //can we switch?

                    if (temp.Capacity_used[k1] - weight[temp.Knapsack[k1][pos_item1]] + weight[nselected[i]] <= Capacity[k1])

                    {

                        CalcValue(temp);

                        double temp_cost = temp.Cost;



                        //invert



                        int temp_item = temp.Knapsack[k1][pos_item1];

                        temp.Knapsack[k1][pos_item1] = nselected[i];

                        nselected[i] = temp_item;



                        // good?

                        CalcValue(temp);

                        if (temp.Cost > temp_cost + 0.01)

                        {

                            // yes, it is good

                            improve = true;

                            temp.selected[ temp.Knapsack[k1][pos_item1] ] = 1;

                            temp.selected[ temp_item ] = 0;

                            temp.Capacity_used[k1] += weight[temp.selected[ temp.Knapsack[k1][pos_item1] ]] - weight[nselected[i]];

                            break;

                        }

                        else

                        {

                            // undo

                            nselected[i] = temp.Knapsack[k1][pos_item1];

                            temp.Knapsack[k1][pos_item1] = temp_item;

                        }

                    }

                }

                if (improve) break;

            }

            if (improve) break;

        }

        //end of second case





        // try to insert items in the last knapsack (the one that is emptier after the two previous procedures)

        for (int k=K; k>0; k--)

        {

            for (int i=1; i<=N; i++)

            {

                if (temp.selected[i] == 0)

                {

                    if (temp.Capacity_used[k] + weight[i] <= Capacity[k])

                    {

                        improve = true;

                        temp.Knapsack[k].push_back(i);

                        temp.selected[i] = 1;

                        temp.Capacity_used[k] += weight[i];

                    }

                }

            }

        }

        // end of third procedure



    }



    CalcValue(temp);

}



void Improve3(Solution &temp)

{

    // try all combinations of switching item1, item2, item3

    vector <int> nselected(0);

    bool improve = true;



    while (improve)

    {

        improve = false;

        Improve(temp);



        for (int k1 = 1; k1<=K; k1++)

        {

            for (int k2=k1; k2<=K; k2++)

            {

            	for (int k3=k1; k3<=K; k3++)

            	{

            		// if all knapsacks are different

            		if (k1 != k2 && k1 != k3 && k2 != k3)

            		{

            			// take one item from each knapsack

            			for (int pos1 = 0; pos1 < temp.Knapsack[k1].size(); pos1++)

            			{

            				for (int pos2 = 0; pos2 < temp.Knapsack[k2].size(); pos2++)

            				{

            					for (int pos3 = 0; pos3 < temp.Knapsack[k3].size(); pos3++)

            					{

            						// try to switch in one direction

            						if ( temp.Capacity_used[k1] - weight[temp.Knapsack[k1][pos1]] + weight[temp.Knapsack[k3][pos3]] <= Capacity[k1] &&

            						temp.Capacity_used[k2] - weight[temp.Knapsack[k2][pos2]] + weight[temp.Knapsack[k1][pos1]] <= Capacity[k2] &&

            						temp.Capacity_used[k3] - weight[temp.Knapsack[k3][pos3]] + weight[temp.Knapsack[k2][pos2]] <= Capacity[k3] )

            						{

            							// capacities allow. is it good cost wise?

            							int item1 = temp.Knapsack[k1][pos1];

            							int item2 = temp.Knapsack[k2][pos2];

            							int item3 = temp.Knapsack[k3][pos3];

            							CalcValue(temp);

            							int oldCost = temp.Cost;

            							// switch

            							temp.Knapsack[k1][pos1] = item3;

            							temp.Knapsack[k2][pos2] = item1;

            							temp.Knapsack[k3][pos3] = item2;

            							CalcValue(temp);

            							if (temp.Cost > oldCost + 0.01)

            							{

            								// good, update capacities

            								improve = true;

            								temp.Capacity_used[k1] += weight[item3] - weight[item1];

            								temp.Capacity_used[k2] += weight[item1] - weight[item2];

            								temp.Capacity_used[k3] += weight[item2] - weight[item3];

            								break;

            							}

            							else

            							{

            								//undo

            								temp.Knapsack[k1][pos1] = item1;

            								temp.Knapsack[k2][pos2] = item2;

            								temp.Knapsack[k3][pos3] = item3;

            							}

            						}

            						// try to switch in the other direction

            						else if ( temp.Capacity_used[k1] - weight[temp.Knapsack[k1][pos1]] + weight[temp.Knapsack[k2][pos2]] <= Capacity[k1] &&

            						temp.Capacity_used[k2] - weight[temp.Knapsack[k2][pos2]] + weight[temp.Knapsack[k3][pos3]] <= Capacity[k2] &&

            						temp.Capacity_used[k3] - weight[temp.Knapsack[k3][pos3]] + weight[temp.Knapsack[k1][pos1]] <= Capacity[k3] )

            						{

            							// capacities allow. is it good cost wise?

            							int item1 = temp.Knapsack[k1][pos1];

            							int item2 = temp.Knapsack[k2][pos2];

            							int item3 = temp.Knapsack[k3][pos3];

            							CalcValue(temp);

            							int oldCost = temp.Cost;

            							// switch

            							temp.Knapsack[k1][pos1] = item2;

            							temp.Knapsack[k2][pos2] = item3;

            							temp.Knapsack[k3][pos3] = item1;

            							CalcValue(temp);

            							if (temp.Cost > oldCost + 0.01)

            							{

            								// good, update capacities

            								improve = true;

            								temp.Capacity_used[k1] += weight[item2] - weight[item1];

            								temp.Capacity_used[k2] += weight[item3] - weight[item2];

            								temp.Capacity_used[k3] += weight[item1] - weight[item3];

            								break;

            							}

            							else

            							{

            								//undo

            								temp.Knapsack[k1][pos1] = item1;

            								temp.Knapsack[k2][pos2] = item2;

            								temp.Knapsack[k3][pos3] = item3;

            							}

            						}

            					}

            					if (improve) break;

            				}

            				if (improve) break;

            			}

            		}

            		else if ( k1 == k2 && k2!=k3 )

            		{

            			// select items. pos1 and pos2 must be different

            			for (int pos1=0; pos1<temp.Knapsack[k1].size(); pos1++)

            			{

            				for (int pos2=pos1 +1; pos2<temp.Knapsack[k1].size(); pos2++)

            				{

            					for (int pos3=0; pos3<temp.Knapsack[k3].size(); pos3++)

            					{

            						int item1 = temp.Knapsack[k1][pos1];

            						int item2 = temp.Knapsack[k1][pos2];

            						int item3 = temp.Knapsack[k3][pos3];



            						// try to switch. capacities allow?

            						if (temp.Capacity_used[k1] - weight[item1] - weight[item2] + weight[item3] <= Capacity[k1] &&

            						temp.Capacity_used[k3] - weight[item3] + weight[item1] + weight[item2] <= Capacity[k3] )

            						{

            							// is it good cost wise?

            							CalcValue(temp);

            							int oldCost = temp.Cost;



            							temp.Knapsack[k1][pos1] = item3;

            							temp.Knapsack[k1].erase( temp.Knapsack[k1].begin() + pos2);

            							temp.Knapsack[k3][pos3] = item1;

            							temp.Knapsack[k3].push_back(item2);



            							CalcValue(temp);

            							if (temp.Cost > oldCost + 0.01)

            							{

            								// good. update capacities

            								improve = true;

            								temp.Capacity_used[k1] += weight[item3] - weight[item1] - weight[item2];

            								temp.Capacity_used[k3] += weight[item1] + weight[item2] - weight[item3];

            								break;

            							}

            							else

            							{

            								// undo

            								temp.Knapsack[k1][pos1] = item1;

            								temp.Knapsack[k1].push_back(item2);

            								temp.Knapsack[k3][pos3] = item3;

            								temp.Knapsack[k3].pop_back();

            							}

            						}

            					}

            					if (improve) break;

            				}

            				if (improve) break;

            			}

            		}



            		if (improve) break;

            	}

            	if (improve) break;

            }

            if (improve) break;

        }







		//two items from knapsacks, one item from not selected items

		// list of not selected items

		for(int items=1;items<=N;items++)

		{

			nselected.clear();

			if(temp.selected[items]==0)

				nselected.push_back(items);

		}

		int Size = nselected.size();



		for (int k1=1; k1<=K; k1++)

		{

			for (int k2=1; k2<=K; k2++)

			{

				if (k1 != k2)

				{

					for (int i=0; i<Size; i++)

					{

						for (int pos1=0; pos1<temp.Knapsack[k1].size(); pos1++)

						{

							for (int pos2=0; pos2<temp.Knapsack[k2].size(); pos2++)

							{

								int item1 = temp.Knapsack[k1][pos1];

								int item2 = temp.Knapsack[k2][pos2];

								int item3 = nselected[i];

								// can we switch in one sense?

								if ( temp.Capacity_used[k1] - weight[ item1 ] + weight[ item3 ] <= Capacity[k1] &&

									 temp.Capacity_used[k2] - weight[ item2 ] + weight[ item1 ] <= Capacity[k2])

								{

									// capacities allow. is it cost effective?

									CalcValue(temp);

									int oldCost = temp.Cost;

									temp.Knapsack[k1][pos1] = item3;

									temp.Knapsack[k2][pos2] = item1;

									nselected[i]            = item2;

									CalcValue(temp);

									if (temp.Cost > oldCost + 0.01)

									{

										// update capacities

										temp.Capacity_used[k1] += weight[ item3 ] - weight[ item1 ] ;

										temp.Capacity_used[k2] += weight[ item1 ] - weight[ item2 ] ;

										improve = true;

										temp.selected[item3] = 1;

										temp.selected[item1] = 1;

										temp.selected[item2] = 0;

										break;

									}

									else

									{

										// undo the moves

										temp.Knapsack[k1][pos1] = item1;

										temp.Knapsack[k2][pos2] = item2;

										nselected[i]            = item3;

									}

								}

								// or try in the other sense

								else if (temp.Capacity_used[k1] - weight[ item1 ] + weight[ item2 ] <= Capacity[k1] &&

										 temp.Capacity_used[k2] - weight[ item2 ] + weight[ item3 ] <= Capacity[k2])

								{

									// capacities allow. is it cost effective?

									CalcValue(temp);

									int oldCost = temp.Cost;

									temp.Knapsack[k1][pos1] = item2;

									temp.Knapsack[k2][pos2] = item3;

									nselected[i]            = item1;

									CalcValue(temp);

									if (temp.Cost > oldCost + 0.01)

									{

										// update capacities

										temp.Capacity_used[k1] += weight[ item2 ] - weight[ item1 ] ;

										temp.Capacity_used[k2] += weight[ item3 ] - weight[ item2 ] ;

										improve = true;

										temp.selected[ item1 ] = 0;

										temp.selected[ item2 ] = 1;

										temp.selected[ item3 ] = 1;

										break;

									}

									else

									{

										// undo the moves

										temp.Knapsack[k1][pos1] = item1;

										temp.Knapsack[k2][pos2] = item2;

										nselected[i]            = item3;

									}

								}

							}

							if (improve) break;

						}

						if (improve) break;

					}

				}

				if (k1 == k2)

				{

					for (int i=0; i<Size; i++)

					{

						for (int pos1=0; pos1<temp.Knapsack[k1].size(); pos1++)

						{

							for (int pos2=pos1+1; pos2<temp.Knapsack[k1].size(); pos2++)

							{

								int item1 = temp.Knapsack[k1][pos1];

								int item2 = temp.Knapsack[k1][pos2];

								int item3 = nselected[i];

								// can we switch ?

								if ( temp.Capacity_used[k1] - weight[ item1 ] - weight[ item2 ] + weight[ item3 ] <= Capacity[k1] )

								{

									// capacities allow. is it cost effective?

									CalcValue(temp);

									int oldCost = temp.Cost;



									temp.Knapsack[k1][pos1] = item3;

									nselected[i]            = item2;

									temp.Knapsack[k1].erase( temp.Knapsack[k1].begin() + pos2);

									CalcValue(temp);

									if (temp.Cost > oldCost + 0.01)

									{

										// update capacities

										temp.Capacity_used[k1] += weight[ item3 ] - weight[ item1 ] - weight[ item2 ];

										improve = true;

										temp.selected[ item1 ] = 0;

										temp.selected[ item2 ] = 0;

										temp.selected[ item3 ] = 1;

										break;

									}

									else

									{

										// undo the moves

										nselected[i]            = item3;

										temp.Knapsack[k1][pos1] = item1;

										temp.Knapsack[k1].push_back(item2);

									}

								}

							}

							if (improve) break;

						}

						if (improve) break;

					}

					if (improve) break;

				}

				if (improve) break;

			}

			if (improve) break;

		}







    }

    CalcValue(temp);

}



Solution GenerateSol()

{

    int repetitions=0,repet=0;;

    int j;

    Solution Sol;

    vector <vector <int> > Knapsack(K+1,vector <int>(0));

    vector <int> Capacity_used(K+1,0);

    int *selected;

    selected = new int[N+1];

    for(int i=1;i<=K;i++)

    {

        Capacity_used[i]=0;

    }

    for (int i=1; i<=N; i++)

    {

        selected[i] = 0;

    }



    while( (repetitions < 10000) && (repet<10000) )

    {

        j=rand() % N +1;  //Random item

        if (selected[j] == 0)

        {

            int i = rand()%K +1; // random knapsack

            if (weight[j]+Capacity_used[i]<=Capacity[i])

            {

                Knapsack[i].push_back(j); //pushing that item



                Capacity_used[i]+=weight[j];//updating the capacity



                selected[j] = 1;

            }

            else

            {

                repet++;

            }

        }

        else

        {

            repetitions++;

        }

    }



    Sol.Knapsack = Knapsack;

    Sol.Capacity_used=Capacity_used;

    Sol.selected.push_back(0);

    for(int i=1;i<=N;i++)

    {

        Sol.selected.push_back(selected[i]);

    }





    cout<<"THE KNAPSACK ITSELF:"<<endl<<endl;

    for(int i=1;i<=K;i++)

    {

        cout<<"Knapsack: "<<i<<"contains";

        for(int g=0;g<Sol.Knapsack[i].size();g++)

        {

            cout<<","<<Sol.Knapsack[i][g];

        }

        cout<<endl;

        cout<<"Capacity_used "<<i<<": "<< Sol.Capacity_used[i]<<endl;

    }

    CalcValue(Sol);



    cout<<"Cost: "<<Sol.Cost<<endl;







    Improve3(Sol);







    cout<<"THE KNAPSACK ITSELF:"<<endl<<endl;

    for(int i=1;i<=K;i++)

    {

        cout<<"Knapsack: "<<i<<"contains";

        for(int g=0;g<Sol.Knapsack[i].size();g++)

        {

            cout<<","<<Sol.Knapsack[i][g];

        }

        cout<<endl;

        cout<<"Capacity_used "<<i<<": "<< Sol.Capacity_used[i]<<endl;

    }

    CalcValue(Sol);



    cout<<"Cost: "<<Sol.Cost<<endl;







    //cin.get();





    return Sol;

}



void CloseFacility(Solution &Sol, int knapsack, int position)

{

#ifdef DEBUG_PRINT

    cout<<"Item to be removed: "<<Sol.Knapsack[knapsack][position]<<endl;

#endif



    Sol.Capacity_used[knapsack] -= weight[Sol.Knapsack[knapsack][position]];

    Sol.selected[Sol.Knapsack[knapsack][position]] = 0;

    Sol.Knapsack[knapsack].erase(Sol.Knapsack[knapsack].begin() + position);

}



int LowerRatios(Solution &Sol)   //A function to return item with a lower linear ratio.

{

    float lower;

    int num;

    lower=99999;

    num=-1;

    for(int i=1;i<=K;i++)

    {

        for(int j=0;j<Sol.Knapsack[i].size();j++)

        {

            if(lower > (float)linCoef[Sol.Knapsack[i][j]]/weight[Sol.Knapsack[i][j]])

            {

                lower=(float)linCoef[Sol.Knapsack[i][j]]/weight[Sol.Knapsack[i][j]];

                num=Sol.Knapsack[i][j];

            }

        }

    }

    return num;

}



int LowerValue(Solution Sol)//A function to return a item with a lower value.

{

    int lower;

    int num;

    lower=999999;

    num=-1;

    for(int i=1;i<=K;i++)

    {

        for(int j=0;j<Sol.Knapsack[i].size();j++)

        {

            if(lower > (linCoef[Sol.Knapsack[i][j]]))

            {

                lower=(linCoef[Sol.Knapsack[i][j]]);

                num=Sol.Knapsack[i][j];

            }

        }

    }

    return num;



}



int Quadvalue(Solution &Sol, int knapsack, int position)

{

    int value=0;



    //value += linCoef[Sol.Knapsack[knapsack][position]];



    for(int h=0;h<Sol.Knapsack[knapsack].size();h++)

    {



        if(Sol.Knapsack[knapsack][h] > Sol.Knapsack[knapsack][position])

        {

            value += quadCoef[Sol.Knapsack[knapsack][position]][Sol.Knapsack[knapsack][h]];

        }

        else if (Sol.Knapsack[knapsack][h] < Sol.Knapsack[knapsack][position])

        {

            value += quadCoef[Sol.Knapsack[knapsack][h]][Sol.Knapsack[knapsack][position]];

        }

    }

    return value;

}



int LowerQuadValue(Solution &Sol)//A function to return a item with a lower value.

{

    int lower;

    int num;

    lower=99999;

    int quadvalue;

    num=-1;

    for(int i=1;i<=K;i++)

    {

        for(int j=0;j<Sol.Knapsack[i].size();j++)

        {

            quadvalue=Quadvalue(Sol, i, j);



            if(lower > quadvalue)

            {

                lower=quadvalue;

                num=Sol.Knapsack[i][j];

            }

        }

    }

    //cout<<"Lower:"<<lower<<endl;

    return num;

}



vector <int> LRatios(Solution &Sol, vector<int> numbers) //A function for returning linear ratios in the form of a vector.//Specifially in the increasing order of linear ratios.

{

    bool done = false;



    for (int i=1; i<=N; i++)

    {

        if (Sol.selected[i] == 1)

        {

            float value = (float)linCoef[i]/weight[i];

            done = false;

            // where to insert?

            for (int j=0; j<numbers.size(); j++)

            {

                if (value < (float)linCoef[numbers[j]]/weight[numbers[j]])

                {

                    numbers.insert(numbers.begin()+j, i);

                    done = true;

                    break;

                }

            }

            // in the end!

            if (done == false) numbers.push_back(i);

        }



    }

   return (numbers);

}



void Destroy(Solution &Sol, int method)

{

    vector <int> selected(0);//A vector which has all selected item's number in it.



    for(int i=1;i<=N;i++)

    {

        if(Sol.selected[i]==1)

        {

            selected.push_back(i);



        }

    }

    double uniform = ((double) rand() )/RAND_MAX;

    int p = (int)floor(1 + selected.size() / 2);

    int cont = floor( (p - sqrt( (1 - uniform) * (p - 1) * (p - 1) )) + 0.5 );

    bool done = false;



    switch( method )

    {

        case 0://Remove items randomly....

        {

#ifdef DEBUG_PRINT

            cout<<"Removing Items Randomly"<<endl;

#endif

            int knapsack, item;

            while (cont > 0)

            {

                knapsack = rand()%K +1;



                if (Sol.Knapsack[knapsack].size() > 0)

                {

                    CloseFacility(Sol, knapsack, rand()%Sol.Knapsack[knapsack].size() );

                }

                cont--;

            }

            break;

        }

        case 1://Remove customers based on linear ratios

        {

#ifdef DEBUG_PRINT

            cout<<"Removing Items Based on Linear Cost Ratios"<<endl;

#endif

            vector <int> numbers ;

            numbers = LRatios(Sol, numbers);



            while (cont > 0)

            {

            	done = false;

               // int pos = rand()%5 +1;

               // if (pos > numbers.size() ) pos = numbers.size();

               // int item = numbers[pos-1];



                if (numbers.size() == 0){ break; }

                else

                {

					int item = numbers[0];

					for (int k=1; k<=K; k++)

					{

						for (int j=0; j<Sol.Knapsack[k].size(); j++)

						{

							if (Sol.Knapsack[k][j] == item)

							{

								CloseFacility(Sol, k, j);

								numbers.erase(numbers.begin());

								done = true;

								break;

							}

						}

						if (done) break;

					}

					cont--;

				}

            }

            break;

        }

        case 2://Remove customers based on lower value

        {

#ifdef DEBUG_PRINT

            cout<<"Removing Items based on Lower Values"<<endl;

#endif

            while (cont > 0)

            {

            	done = false;

                int item=LowerValue(Sol);

                if (item == -1){ break; }

                else

                {

					for (int k=1; k<=K; k++)

					{

						for (int j=0; j<Sol.Knapsack[k].size(); j++)

						{

							if (Sol.Knapsack[k][j] == item)

							{

								CloseFacility(Sol, k, j );

								done = true;

								break;

							}

						}

						if (done) break;

					}

					cont--;

				}

            }

            break;

        }

        case 3://Remove Customers based on Quadratic Values

        {

#ifdef DEBUG_PRINT

            cout<<"Removing Items based on Quadratic Ratios";

#endif

            while (cont > 0)

            {

            	done = false;

                int item=LowerQuadValue(Sol);

                if (item == -1){cont--; break;}

                else

				{

					for (int k=1; k<=K; k++)

					{

						for (int j=0; j<Sol.Knapsack[k].size(); j++)

						{

							if (Sol.Knapsack[k][j] == item)

							{

								CloseFacility(Sol, k, j);

								done = true;

							}

						}

						if (done) break;

					}

					cont--;

				}

            }

            break;

        }

             case 4://Empty a random knapsack

             {



				 int h=rand()%K+1;

				 for(int i=0;i<Sol.Knapsack[h].size();i++)

				 {

					 Sol.selected[Sol.Knapsack[h][i]]=0;

				 }

				 Sol.Capacity_used[h]=0;

				 Sol.Knapsack[h].resize(0);

				 CalcValue(Sol);

				 break;



             }



    }



}



int QuadSinglevalue(Solution &Sol,int num,int knapsack)

{

    int value=0;

    value += linCoef[num];

    for(int j=0;j<Sol.Knapsack[knapsack].size();j++)

        if(Sol.Knapsack[knapsack][j]!=num)

        {

            if (num > Sol.Knapsack[knapsack][j])

            {

                value+=quadCoef[Sol.Knapsack[knapsack][j]][ num ];

            }

            else

            {

                value+= quadCoef[ num ][Sol.Knapsack[knapsack][j]];

            }

        }



    return value;

}



vector <int> HigherRatios(Solution &Sol, vector <int> numbers)//A function for returning linear ratios in the form of a vector

{

	numbers.clear();

    bool done=false;

    for(int i=1;i<=N;i++)

    {

        if(Sol.selected[i]==0)

        {

            done = false;

            for (int pos=0; pos < numbers.size(); pos++)

            {

                if ((float)linCoef[i]/(float)weight[i] > (float)linCoef[ numbers[pos] ]/(float)weight[ numbers[pos] ] )

                {

                    // insert in this position

                    numbers.insert(numbers.begin()+pos, i);

                    done=true;

                    break;

                }

            }

            // or the item is the last one on the list

            if (done == false){numbers.push_back(i);}

        }

    }

    return (numbers);

}



vector <int> OptQValue(Solution &Sol,int num, vector <int> element)

{

    element.clear();

    int knapsack=-1;

    int Quad=-1;

    int Final=-1;

    for(int i=1;i<=K;i++)

    {

        if(weight[num]+ Sol.Capacity_used[i]<=Capacity[i])

        {

            Sol.Knapsack[i].push_back(num);

            Quad=QuadSinglevalue(Sol,num,i);

            Sol.Knapsack[i].pop_back();



            if(Quad>Final)

            {

                Final=Quad;

                knapsack=i;

            }

        }

    }

    element.push_back(Final);

    element.push_back(knapsack);

    element.push_back(num);

    return element;



}



vector <vector<int> > HigherQValues(Solution &Sol, vector <vector<int> > Numbers)//A function for returning quadratic ratios in the form of a 2d vector

{

    vector <int> numbers;

    vector<vector<int> > ItemsQValue;

    vector<int>temp;

    vector<int> Temp;

    int h;

    int num=-1;

    float higher;

    int *tempselected;

    tempselected=new int[N+1];

    higher=-1.0;

    for(int i=1;i<=N;i++)

    {

        tempselected[i]=Sol.selected[i];

    }

#ifdef DEBUG_PRINT

    cout<<"Items not selected: "<<endl;

    for(int i=1;i<=N;i++)

        if(tempselected[i]==0)

            cout<<i<<", ";



    cout<<endl;

#endif

n:

    higher = -1.0;

    ItemsQValue.clear();



    for(int i=1;i<=N;i++)

    {

        if(tempselected[i]==0)

        {

            temp.clear();

            temp = OptQValue(Sol, i, temp);

            ItemsQValue.push_back(temp);

        }

    }



    for(int i=0;i<ItemsQValue.size();i++)

    {

        if(higher < ItemsQValue[i][0] - 0.01)

        {

            higher = ItemsQValue[i][0];

            h = i;

        }

    }



#ifdef DEBUG_PRINT

    if (higher > 0) cout<<"Item no: "<<ItemsQValue[h][2]<<" Quadratic Value: "<<ItemsQValue[h][0]<<" Knapsack: "<<ItemsQValue[h][1]<<endl;

#endif

    if(higher > 0)

    {

        numbers.clear();

        numbers.push_back(ItemsQValue[h][0]);

        numbers.push_back(ItemsQValue[h][1]);

        numbers.push_back(ItemsQValue[h][2]);



        Numbers.push_back(numbers);



        tempselected[ ItemsQValue[h][2] ] = 1;



        goto n;

    }



    delete [] tempselected;

    return (Numbers);

}



vector <int> HigherValue(Solution &Sol, vector <int> numbers)//A function for returning linear ratios in the form of a vector

{

    bool done = false;

    for(int i=1;i<=N;i++)

    {

        if(Sol.selected[i]==0)

        {

            done = false;

            for (int pos=0; pos < numbers.size(); pos++)

            {

                if (linCoef[i] > linCoef[ numbers[pos] ])

                {

                    // insert in this position

                    numbers.insert(numbers.begin()+pos, i);

                    done = true;

                    break;

                }

            }

            // or the item is the last one on the list

            if (done == false) numbers.push_back(i);

        }

    }

    return (numbers);

}



vector <int> HigherQRatios(Solution &Sol,vector <vector<int> > Numbers,vector <int>  numbers)//A function for returning linear ratios in the form of a vector

{

    int num=-1;

    float higher=-1;

    int *tempselected;

    tempselected=new int[N+1];

    higher=-1.0;

#ifdef DEBUG_PRINT

    cout<<"The elements in the 2d vector are "<<endl;

    for(int i=1;i<Numbers.size();i++)

    {

        for(int j=0;j<Numbers[i].size();j++)

        {

            cout<<Numbers[i][j]<<", ";

        }

        cout<<endl;

    }

    cout<<endl;

#endif

    for(int i=1;i<=N;i++)

    {

        tempselected[i]=Sol.selected[i];

    }



n:

    num = -1;

    higher = -1;



    for(int i=1;i<Numbers.size();i++)

    {

        if(tempselected[Numbers[i][2]]==0)

        {

            if ((float)(Numbers[i][0]+linCoef[Numbers[i][2]])/weight[Numbers[i][2]] > higher)

            {

                higher = (float)(Numbers[i][0]+linCoef[Numbers[i][2]])/weight[Numbers[i][2]];

                num    = Numbers[i][2];

            }

        }

    }





#ifdef DEBUG_PRINT

    cout<<"Item no: "<<num<<" Ratio Value: "<<higher<<endl;

#endif

    if(num!=-1)

    {

        numbers.push_back(num);

        tempselected[num]=1;



        goto n;

    }



    delete [] tempselected;

    return (numbers);

}



void Repair(Solution &Sol, int method)

{

    int Size=0,num,counter=1;

    int repetitions=0,repet=0;

    vector <int> nselected;



    for(int i=1;i<=N;i++)

    {

        if(Sol.selected[i]==0)

        {

            nselected.push_back(i);



        }

    }

    Size=nselected.size();

#ifdef DEBUG_PRINT

    cout<<"Size: "<<Size<<endl;

    cout<<"not selected "<<endl;

    for(int i=0;i<Size;i++)

    {

        cout<<nselected[i]<<", ";

    }

#endif

    vector <int> knapsacks;





    switch(method)

    {



        case 0://Adding items randomly

        {



#ifdef DEBUG_PRINT

            cout<<endl;

            cout<<"Adding items randomly"<<endl;

            cout<<"Not Repaired Cost: "<<Sol.Cost<<endl;

#endif

            bool done = false;



            while (repetitions < Size)

            {

                // select one item

                int pos_item  = rand()%nselected.size();

                int item      = nselected[pos_item];



                // reset the list of knapsacks

                knapsacks.clear();

                knapsacks.reserve(K+1);

                for (int k=1; k<=K; k++)

                    knapsacks.push_back(k);



                done = false;

                while( repet<K )

                {

                    // select one knapsack

                    int pos_knapsack = rand()%knapsacks.size();

                    int k            = knapsacks[pos_knapsack];



                    if(weight[item]+Sol.Capacity_used[k]<=Capacity[k])

                    {

#ifdef DEBUG_PRINT

                        cout<<" Item no:"<< item <<endl;

                        cout<<"Inserted at Knapsack: "<<k<<endl;

#endif

                        Sol.Knapsack[k].push_back(item);//pushing that element.

                        Sol.Capacity_used[k]+=weight[item];//updating the capacity.

                        Sol.selected[item] = 1;

                        nselected.erase(nselected.begin()+pos_item);

                        repetitions++;

                        done = true;

                        repet = K+1;

                    }

                    else

                    {

                        knapsacks.erase(knapsacks.begin() + pos_knapsack);

                        repet++;

                    }

                }

                // item cannot be inserted in any knapsack. remove it from the list anyway

                if (done == false)

                {

                    nselected.erase(nselected.begin()+pos_item);

                    repetitions++;

                }

            }





            CalcValue(Sol);

            break;

        }

        case 1://Inserting customers based on their linear ratios

        {

            vector <int> numbers(0);

            numbers = HigherRatios(Sol, numbers);



#ifdef DEBUG_PRINT

            cout<<"Adding Items Based on Linear Cost Ratios"<<endl;

            cout<<"The items that are not selected are "<<endl;



            for(int i=0;i<numbers.size();i++)

            {

                cout<<numbers[i]<<", ";

            }

            cout<<endl;

#endif



            int Size = numbers.size();

            bool done = false;



            for(int i=0;i<Size;i++)

            {

                num = numbers[i];

#ifdef DEBUG_PRINT

                cout<<"Item that may be added: "<<num<<" whose ratio is: "<<(float)linCoef[num]/weight[num]<<endl;

#endif

                int repet=0;



                // reset the list of knapsacks

                knapsacks.clear();

                knapsacks.reserve(K+1);

                for (int k=1; k<=K; k++)

                    knapsacks.push_back(k);



                done  = false;

                while (repet < K && Sol.selected[num] == 0)

                {

                    // select one knapsack

                    int pos_knapsack = rand()%knapsacks.size();

                    int k            = knapsacks[pos_knapsack];



                    if(weight[num]+Sol.Capacity_used[k]<=Capacity[k])

                    {

#ifdef DEBUG_PRINT

                        cout<<" Item no:"<< num <<" inserted at Knapsack: "<<k<<endl;

#endif

                        Sol.Knapsack[k].push_back(num);//pushing that element.

                        Sol.Capacity_used[k]+=weight[num];//updating the capacity.

                        Sol.selected[num] = 1;

                        done = true;

                        break;

                    }

                    else

                    {

                        knapsacks.erase(knapsacks.begin() + pos_knapsack);

                        repet++;

                    }

                }

            }



            CalcValue(Sol);

            break;



        }

        case 2://Inserting Customers based on higher Value

        {



            vector <int> numbers;

            numbers = HigherValue(Sol, numbers);

#ifdef DEBUG_PRINT

            cout<<"Adding Items Based on Linear values"<<endl<<"Not Repaired Cost: "<<Sol.Cost<<endl;

            cout<<endl;

            cout<<"The items that are not selected are "<<endl;



            for(int i=0;i<numbers.size();i++)

            {

                cout<<numbers[i]<<", ";

            }

            cout<<endl;

#endif

            int Size = numbers.size();



            bool done=false;



            for(int i=0;i<Size;i++)

            {

                num = numbers[i];

#ifdef DEBUG_PRINT

                cout<<"Item that may be added: "<<num<<" whose value is: "<<linCoef[num]<<endl;

#endif

                int repet=0;



                // reset the list of knapsacks

                knapsacks.clear();

                knapsacks.reserve(K+1);

                for (int k=1; k<=K; k++)

                    knapsacks.push_back(k);



                done = false;

                while (repet < K)

                {

                    // select one knapsack

                    int pos_knapsack = rand()%knapsacks.size();

                    int k            = knapsacks[pos_knapsack];



                    if(weight[num]+Sol.Capacity_used[k]<=Capacity[k])

                    {

#ifdef DEBUG_PRINT

                        cout<<" Item no:"<< num <<endl;

                        cout<<"Inserted at Knapsack: "<<k<<endl;

#endif

                        Sol.Knapsack[k].push_back(num);//pushing that element.

                        Sol.Capacity_used[k]+=weight[num];//updating the capacity.

                        Sol.selected[num] = 1;

                        done = true;

                        repet=K+1;

                    }

                    else

                    {

                        knapsacks.erase(knapsacks.begin() + pos_knapsack);

                        repet++;

                    }

                }

            }



            CalcValue(Sol);

            break;



        }

        case 3://Inserting items based on Quadratic Values

        {



            vector < vector <int> > Numbers;

            Numbers = HigherQValues(Sol, Numbers);



#ifdef DEBUG_PRINT

            cout<<"Inserting Items based on Quadratic values";

            cout<<endl;

            cout<<endl;

            cout<<"The Items that are not selected are"<<endl;

            for(int i=0;i<Numbers.size();i++)

            {

                for(int j=0;j<Numbers[i].size();j++)

                {

                    cout<<Numbers[i][j]<<",";

                }

                cout<<endl;

            }

            cout<<endl;

#endif



            for(int i=0;i<Numbers.size();i++)

            {

                num=Numbers[i][2];

#ifdef DEBUG_PRINT

                cout<<"Item that may be added: "<<num<<endl;

#endif

                int j=Numbers[i][1];



                if(weight[num]+Sol.Capacity_used[j]<=Capacity[j])

                {



#ifdef DEBUG_PRINT

                    cout<<" ITEM no:"<<num<<endl;

#endif

                    Sol.Knapsack[j].push_back(num);//pushing that element.

                    Sol.Capacity_used[j]+=weight[num];//updating the capacity.

                    Sol.selected[num] = 1;

#ifdef DEBUG_PRINT

                    cout<<"ITEM to be added: "<<num<<endl;

#endif

                }

                else

                {

                    int q=1;

                w:

                    if(q<=K)

                    {

                        if(weight[num]+Sol.Capacity_used[q]<=Capacity[q])

                        {

#ifdef DEBUG_PRINT

                            cout<<" Item no:"<<num<<endl;

#endif

                            Sol.Knapsack[q].push_back(num);//pushing that element.

                            Sol.Capacity_used[q]+=weight[num];//updating the capacity.

                            Sol.selected[num] = 1;

#ifdef DEBUG_PRINT

                            cout<<"Item to be added: "<<num<<endl;

#endif

                        }

                        else

                        {

                            q++;

                            goto w;

                        }

                    }

                }

            }



            CalcValue(Sol);

            break;



        }

        case 4://Inserting items based on quadratic ratios

        {



            vector <int> numbers;

            vector < vector <int> > Numbers;



            Numbers = HigherQValues(Sol, Numbers);

            numbers = HigherQRatios(Sol, Numbers, numbers);



#ifdef DEBUG_PRINT

            cout<<endl;

            cout<<"removing Items based on Quadratic Ratios"<<endl;

            cout<<"The items that are not selected are"<<endl;

            for(int i=0;i<numbers.size();i++)

            {

                cout<<numbers[i]<<",";

            }

            cout<<endl;

#endif

            for(int i=0;i<numbers.size();i++)

            {

                num = numbers[i];

#ifdef DEBUG_PRINT

                cout<<"Item that may be added:"<<num<<"please: "<<endl;

#endif

                int j=1;

            t:

                if(j<=K)

                {

                    if(weight[num]+Sol.Capacity_used[j]<=Capacity[j])

                    {

#ifdef DEBUG_PRINT

                        cout<<" Item no:"<<num<<endl;

#endif

                        Sol.Knapsack[j].push_back(num);//pushing that element.

                        Sol.Capacity_used[j]+=weight[num];//updating the capacity.

                        Sol.selected[num] = 1;

#ifdef DEBUG_PRINT

                        cout<<"Item to be added: "<<num<<endl;

#endif

                    }

                    else

                    {

                        j++;

                        goto t;

                    }

                }

            }



            CalcValue(Sol);

            break;

        }

    }



}



int Accept(Solution &newSol, Solution &oldSol, double T)

{

    double prob;



    // acceptance probability

    prob = exp( (oldSol.Cost - newSol.Cost) / T);



    if ( prob > ((double) rand())/RAND_MAX )

        return 1;

    else

        return 0;

}



void UpdateALNS(double *weight, int num, int *util, double rate, int *score)

{

    double sum;



    for (int i = 0; i < num; i++)

        if (util[i] > 0)

        {

            // update weight

            weight[i] = (1 - rate) * weight[i] + rate*score[i]/util[i];



            // reset scores

            score[i] = 0;

        }



    sum = 0;

    for (int i = 0; i < num; i++)

        sum += weight[i];



    for (int i = 0; i < num; i++)

        weight[i] = weight[i]/sum;



}



void ALNS()

{

    Solution current;         // current location solution = s

    Solution incumbent;       // incumbent location solution = s'

    Solution best;            // best location solution  = s_best

    int    nDestroy;        // number of destruction operators

    int    nRepair;         // number of repairing operators

    int    iD;              // id of destruction operator

    int    iR;              // id of repairing operator

    int    *UD;             // destruction operators utilization

    int    *UR;             // repairing operators utilization

    int    *SD;             // destruction operators score

    int    *SR;             // repairing operators score

    int    rewardH = 10;    // reward high

    int    rewardM =  5;    // reward medium

    int    rewardL =  2;    // reward low

    double *WD;             // destruction operators weights

    double *WR;             // repairing operators weights

    double smoothing;       // smoothing factor for operators weights

    double temp;            // simulated annealing temperature

    double finalTemp;       // final temperature

    double alpha;           // cooling rate

    int    iterT;           // estimated total iteration counter

    int    iter;            // iteration counter

    int    eoc;             // end of cycle counter

    char   *str = "output.txt" ;     // output file name

    FILE   *arq;            // file pointer

    struct timeval ITime;   // start time

    struct timeval FTime;   // end time

    nDestroy = 5;

    nRepair = 5;

    smoothing = 0.7;

    temp = 30000;

    finalTemp = 0.01;

    eoc = 500;

    iterT = 20000;

    ofstream File("output.txt");

    ofstream Fi("Results.txt");



    alpha = pow(finalTemp/temp, 1.0/iterT);

    // cout<<"No problem here";

    // solutions initialization

    cout<<"BEST"<<endl;

    best = GenerateSol();

    cout<<"CURRENT"<<endl;

    current = GenerateSol();

    cout<<"INCUMBENT"<<endl;

    incumbent = GenerateSol();



    //CopySol(incumbent,best);

    best = incumbent;

    cout<<"Cost :"<<best.Cost<<endl;





    Improve3(best);



    //CopySol(best,incumbent);

    incumbent = best;



    // operators utilization

    UD = InitValues(nDestroy);

    UR = InitValues(nRepair);



    // operators score

    SD = InitValues(nDestroy);

    SR = InitValues(nRepair);



    // operators weight

    WD = InitWeights(nDestroy);

    WR = InitWeights(nRepair);



    // initialize iteration counter

    iter = 1;



    // start time

    gettimeofday(&ITime, NULL);

    ofstream Fil("/Users/ranjith/Desktop/Knapsack/input.txt");

    Fil << "Best Cost: "<< best.Cost<<endl;

    for(int i=1;i<=K;i++)

    {

        Fil << "Knapsack No:"<<i<<"-";

        for(int j=0;j<best.Knapsack[i].size();j++)

        {

            Fil<<best.Knapsack[i][j]<<" ";

        }

        Fil << endl;

    }



    while (temp > finalTemp)

    {

        //cout << "beginning of ALNS loop. Solution is: " << endl;

        //PrintSol(incumbent);

        //cout << "----Iteration " <<i<< "---------------------------------------" << endl ;

        //cout<<"Incumbent Cost"<<incumbent.Cost<<" "<<"Current Cost"<<current.Cost<<endl;

        //CopySol(incumbent, current);

        current = incumbent;

        iD = SelectMethod(WD);



        //cout << "Destroy Method used :" << iD << "\n" << endl;

        Destroy(current,iD);

        // cout<<endl<<"Current Cost = "<<current.Cost<<endl;



        if (0.4 > ((double) rand() / (RAND_MAX)) )

        {

            Pack(current);

        }



#ifdef DEBUG

        // cout << "Destroyed solution :" << endl;

        // PrintSol(current) ;

#endif



        iR = SelectMethod(WR);

        //cout << "Repair Method used :" << iR << endl;

        Repair(current, iR);



        if (0.3 > ((double) rand() / (RAND_MAX)) )

        {

            Improve3(current);

        }



        if (current.Cost > incumbent.Cost + 0.01) // here is a MAXIMIZATION

        {





            if (current.Cost > best.Cost + 0.01)

            {

                best = current;

                SD[iD] += rewardH;

                SR[iR] += rewardH;

                cout << "Best solution !" << endl;

                Improve3(best);

                PrintSol(best);

                current = best;

                incumbent = best;

            }

            else

            {

                //CopySol(current, incumbent);

                incumbent = current;

                SD[iD] += rewardM;

                SR[iR] += rewardM;

                //cout << "Not best solution, but better than the previous one." << endl ;

            }

        }



        else

        {

            if ( Accept(current, incumbent, temp) )

            {

                //CopySol(current, incumbent);

                incumbent = current;

                SD[iD] += rewardL;

                SR[iR] += rewardL;

                //cout << "Not better solution, but we accept it." << endl ;

            }

            //else cout << "Not accepted." << endl ;

        }





        //test end of cycle

        if (iter % eoc == 0)

        {

            UpdateALNS(WD, nDestroy, UD, smoothing, SD);

            //  UpdateALNS(WR, nRepair, UR, smoothing, SR);



            //CopySol(best, incumbent);

            incumbent = best;

        }

       /*

        // File << "Incumbent Cost: "<< incumbent.Cost<<" ";

        // File<<endl;

        if(iter%50==0)

        {

           //  File << "Best Cost: "<< best.Cost<<" ";

           //  File << endl;

        }

        if(iter%1000==0)

        {

           //  Fi << "Best Cost After "<<iter<<"Iterations:"<<best.Cost<<" ";

           //  Fi << endl;

        }

        */





        temp *= alpha;

        iter++;





        //cout << "Best: " << best.fo << " Incumbent = " << incumbent.fo << " Current: " << current.fo << endl;

   }



    PrintSol(best);

    /*

    File << "Best Cost: "<< best.Cost<<endl;

    for(int i=1;i<=K;i++)

    {

        File << "Knapsack No:"<<i<<"-";

        for(int j=0;j<best.Knapsack[i].size();j++)

        {

            File<<best.Knapsack[i][j]<<" ";

        }

        File << endl;

    }

     */



    // end time



        // end time

        gettimeofday(&FTime, NULL);



        // elapsed time

        double elapsed = (FTime.tv_sec - ITime.tv_sec);

	cout << "Solution time " << elapsed << endl;



}







int main()

{

    srand((unsigned)time(NULL));

    loadInstance();

    ALNS();

    cout << "Hello world!" << endl;

    return 0;

}




