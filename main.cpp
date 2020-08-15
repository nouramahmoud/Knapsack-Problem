#include <iostream>
#include <list>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <fstream>

using namespace std;


int Test_Case , NumOfItems, SizeOfKnapsack;
int PopulationSize = 1000;
int p_CrossOver = 0.7;
int p_Mutation = 0.01;
vector<int> Weight;
vector<int> Value;
vector<double> Cumulative;


struct Chromo
{
    vector<int> Genes;
    int Weight;
    int Fitness;
};


vector<Chromo> Population;
vector<Chromo> NewPopulation;




void initiate_population()
{
    while (Population.size() < PopulationSize)
    {
        Chromo c;
        c.Weight = 0;
        c.Fitness = 0;
        for (int i = 0; i < NumOfItems; i++)
        {
            float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
            if ( r < 0.5 )
            {
                c.Genes.push_back(0);
            }
            else
            {
                c.Genes.push_back(1);
                c.Weight += Weight.at(i);
                c.Fitness += Value.at(i);
            }
        }
        if ( c.Weight <= SizeOfKnapsack )
        {
            Population.push_back(c);
        }
    }
}


void Calculate_Cumulative ()
{
    int total = 0;
    for (int i = 0 ; i < Population.size() ; i++)
    {
        total += Population.at(i).Fitness;
    }
    double Previous_Probability = 0;
    for (int i = 0 ; i < Population.size() ; i++)
    {
        double Number = (double)Population.at(i).Fitness / total;
        Previous_Probability += Number;
        Cumulative.push_back(Previous_Probability);
    }
}


int Select ()
{
    float Random_Number = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    for (int i = 0 ; i < PopulationSize ; i++)
    {
        if ( Random_Number < Cumulative.at(i))
        {
            return i;
        }
    }
}

void CrossOver()
{
        int x = Select();
        int y = Select();
    // r1 da l fasl ely h3ml beh cross over
        int r1 = 1;
        Chromo c1 ;
        c1.Fitness = 0;
        c1.Weight = 0;
        Chromo c2;
        c2.Fitness = 0;
        c2.Weight = 0;
        float Random_Number = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        if (Random_Number < p_CrossOver)
    {
        for(int i=0;i<=r1;i++){
            c1.Genes.push_back(Population.at(x).Genes.at(i)) ;
            c1.Weight += (c1.Genes.at(i) * Weight.at(i));
            c1.Fitness += (c1.Genes.at(i) * Value.at(i));
            c2.Genes.push_back(Population.at(y).Genes.at(i)) ;
            c2.Weight += (c2.Genes.at(i) * Weight.at(i));
            c2.Fitness += (c2.Genes.at(i) * Value.at(i));
        }
        for(int j=r1+1;j<NumOfItems;j++){
            c1.Genes.push_back(Population.at(y).Genes.at(j)) ;
            c1.Weight += (c1.Genes.at(j) * Weight.at(j));
            c1.Fitness += (c1.Genes.at(j) * Value.at(j));
            c2.Genes.push_back(Population.at(x).Genes.at(j)) ;
            c2.Weight += (c2.Genes.at(j) * Weight.at(j));
            c2.Fitness += (c2.Genes.at(j) * Value.at(j));
        }

            if (c1.Weight < SizeOfKnapsack && c2.Weight < SizeOfKnapsack) {
            NewPopulation.push_back(c1);
            NewPopulation.push_back(c2);
           }
     }
        else {
        NewPopulation.push_back(Population.at(x));
        NewPopulation.push_back(Population.at(y));


    }
}
void Mutation() {
    for (int i = 0 ; i < NewPopulation.size() ; i ++) {
        Chromo c ;
        c.Fitness = 0;
        c.Weight = 0;
        for (int j = 0 ; j < NumOfItems ; j++) {
            float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
            if ( r < p_Mutation) {
                c.Genes.push_back(1 - NewPopulation.at(i).Genes.at(j));
            }
            else {
                c.Genes.push_back(NewPopulation.at(i).Genes.at(j));
            }
            c.Weight += (c.Genes.at(j) * Weight.at(j));
            c.Fitness += (c.Genes.at(j) * Value.at(j));
        }
        if (c.Weight < SizeOfKnapsack) {
            NewPopulation.at(i) = c;
        }
    }
}

bool compareWeight(const Chromo &a, const Chromo &b) {
    return a.Fitness > b.Fitness;
}

int main()
{
    ifstream inFile;
    inFile.open("‪C:\\Users\\user\\Desktop\\FCI\\7th term\\soft computing\\assignment1\\input_example.txt"); //da el directory bta3 l input file
    if (!inFile) {
    cout << "Unable to open file datafile.txt";
    exit(1);   // call system to stop
}
    cout << "Enter_Number_Of_Test_Cases: " ;
    cin >> Test_Case;
    while(Test_Case > 20){
        cout<<"Invalid Value"<<endl;
        cout<<"Please_Enter_a_Value_Less_Than_Or_Equal_20"<<endl;
        cin>>Test_Case;
    }
    cout << "Enter_Number_Of_Items: " ;
    cin >> NumOfItems;
    while(NumOfItems > 50){
        cout<<"Invalid Value"<<endl;
        cout<<"please_enter_a_value_less_than_or_equal_50"<<endl;
        cin>>NumOfItems;
    }
    cout << "Enter_Size_Of_Knapsack: " ;
    cin >> SizeOfKnapsack;
    for (int i = 0 ; i < NumOfItems ; i ++)
    {
        int weight, value;
        cin >> weight >> value;
        Weight.push_back(weight);
        Value.push_back(value);
    }

    srand((unsigned)time(0));

    initiate_population();

    int j;
    for ( j= 0; j < Test_Case ; j++) {

        Calculate_Cumulative();

        while (NewPopulation.size() < PopulationSize) {
            CrossOver();
        }

        Mutation();
        Population = NewPopulation;

    }

    std::sort(Population.begin(),Population.end(), compareWeight);


    Chromo x = Population.at(0);
    cout << "Case" <<  j << ":"<<x.Fitness <<endl;

    int i ;
    int counter = 0;
    cout <<"Items :" <<endl;
    for ( i = 0; i < NumOfItems ; i ++) {

        if ( x.Genes.at(i) == 1) {
            cout << "(" << Weight.at(i) << "," << Value.at(i) << ")" <<endl;
            counter++;
            }
    }
    cout << "Number of items:" << counter <<endl;

    return 0;
}
