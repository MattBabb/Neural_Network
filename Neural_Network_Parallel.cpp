//AHHHHH HELP WHAT HAVE I GOTTEN MYSELF INTO?!?!?!?!!?!?!?!!?!?

// File: Neural_Network_Parallel.cpp
//
// Author: Matthew Babb
// Course: CIS-363
// Assignment: Neural network letter recognition
//
// Puropose: Trains perceptrons in neural network to differentiate 
// between letters and allow user to check validity of a possible letter.
//
// Included code is either provided by Dr. Hoffert(timings), or is 
// original and written by Matthew Babb.

#include<cmath>
#include<ctime>
#include<iostream>
#include<mutex>
#include <sys/time.h>
#include<thread>

using namespace std;

//Learning tools
double ALPHA = 0.1;
double WEIGHTS[25] = {0};
const int NODES = 25;
const double THRESHOLD = 0.5;
mutex m;
double REUSE_TOTAL = 0;

//Training arrays and related values
int LETTERS = 15;
int LETTER_A[25] = {0,0,1,0,0,0,1,0,1,0,1,0,0,0,1,1,1,1,1,1,1,0,0,0,1};
int LETTER_B[25] = {1,1,1,0,0,1,0,1,0,0,1,1,1,0,0,1,0,0,1,0,1,1,1,0,0};
int LETTER_C[25] = {1,1,1,1,1,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,1,1,1,1};
int LETTER_D[25] = {1,1,0,0,0,1,0,1,0,0,1,0,0,1,0,1,0,0,1,0,1,1,1,0,0};
int LETTER_E[25] = {1,1,1,1,0,1,0,0,0,0,1,1,1,0,0,1,0,0,0,0,1,1,1,1,0};
int LETTER_F[25] = {1,1,1,0,0,1,0,0,0,0,1,1,1,0,0,1,0,0,0,0,1,0,0,0,0};
int LETTER_G[25] = {0,1,1,1,0,1,0,0,0,0,1,0,1,1,1,1,0,0,0,1,0,1,1,1,0};
int LETTER_H[25] = {1,0,0,1,0,1,0,0,1,0,1,1,1,1,0,1,0,0,1,0,1,0,0,1,0};
int LETTER_I[25] = {0,1,1,1,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,1,1,1,0};
int LETTER_J[25] = {0,1,1,1,0,0,0,1,0,0,0,0,1,0,0,1,0,1,0,0,1,1,1,0,0};
int LETTER_K[25] = {1,0,0,1,0,1,0,1,0,0,1,1,0,0,0,1,0,1,0,0,1,0,0,1,0};
int LETTER_L[25] = {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,1,1,1,0};
int LETTER_M[25] = {1,0,0,0,1,1,1,0,1,1,1,0,1,0,1,1,0,0,0,1,1,0,0,0,1};
int LETTER_N[25] = {1,0,0,0,1,1,1,0,0,1,1,0,1,0,1,1,0,0,1,1,1,0,0,0,1};
int LETTER_T[25] = {0,1,1,1,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0};
int TRUE_VAL[15] = {0,0,0,0,1,0,0,0,0,0,0,0,0,0,0};

int CALCULATED_TRUTH_VAL[15] = {-1};
int TRUTH_A = 0;
int TRUTH_B = 1;
int TRUTH_C = 2;
int TRUTH_D = 3;
int TRUTH_E = 4;
int TRUTH_F = 5;
int TRUTH_G = 6;
int TRUTH_H = 7;
int TRUTH_I = 8;
int TRUTH_J = 9;
int TRUTH_K = 10;
int TRUTH_L = 11;
int TRUTH_M = 12;
int TRUTH_N = 13;
int TRUTH_T = 14;
// 1 1 1 1 0
// 1 0 0 0 0
// 1 1 1 0 0
// 1 0 0 0 0
// 1 1 1 1 0

//Given code for timings
typedef unsigned long long timestamp_t;
const int MICROSECONDS_PER_SECOND = 1000000.0L;
timestamp_t get_timestamp ()
{
	struct timeval now;
	::gettimeofday (&now, NULL);
	return  now.tv_usec + 
		static_cast<timestamp_t>(now.tv_sec) * MICROSECONDS_PER_SECOND;
}

//Function declarations
void rnd_weight_init(double weights[]);
void training(int training_set[], int truth);
void current_output(int i, int set[]);
void update_weights(int i, int set[], int y, int truth);
double test_letter(int training_set[]);

int main()
{
	//Randomize starting weights
	rnd_weight_init(WEIGHTS);

	//Loop variables and exit strategies
	bool smart = false;
	int count = 0;
	int min_loops = 15;
	int max_loops = 10000;

	//User output the initial random weights given
	cout << "Initial Random Weights: " << endl;
	for(int k = 0; k < NODES; ++k)
	{
		cout << WEIGHTS[k] << " ";
		if(k == 4 || k == 9 || k == 14 || k == 19)
			cout << endl;
	}
	cout << endl << endl;

	timestamp_t t_start = get_timestamp();

	//While loop does the learning, E or not an E; 
	// using E map, Inverse E, and C Map
	while (count < max_loops && smart == false)
	{
		//Resets the Calculated truths for each iteration
		for(int a=0; a < LETTERS; ++a)
		{
			CALCULATED_TRUTH_VAL[a] = -1;
		}
		

		//Call training function 
		training(LETTER_A, TRUTH_A);
		training(LETTER_B, TRUTH_B);
		training(LETTER_C, TRUTH_C);
		training(LETTER_D, TRUTH_D);
		training(LETTER_E, TRUTH_E);
		training(LETTER_F, TRUTH_F);
		training(LETTER_G, TRUTH_G);
		training(LETTER_H, TRUTH_H);
		training(LETTER_I, TRUTH_I);
		training(LETTER_J, TRUTH_J);
		training(LETTER_K, TRUTH_K);
		training(LETTER_L, TRUTH_L);
		training(LETTER_M, TRUTH_M);
		training(LETTER_N, TRUTH_N);
		training(LETTER_T, TRUTH_T);		

		//Check for Matching Real and Calculated truth values
		if((TRUE_VAL[TRUTH_A] == CALCULATED_TRUTH_VAL[TRUTH_A]) 
			&& (TRUE_VAL[TRUTH_B] == CALCULATED_TRUTH_VAL[TRUTH_B]) 
			&& (TRUE_VAL[TRUTH_C] == CALCULATED_TRUTH_VAL[TRUTH_C])
			&& (TRUE_VAL[TRUTH_D] == CALCULATED_TRUTH_VAL[TRUTH_D])
			&& (TRUE_VAL[TRUTH_E] == CALCULATED_TRUTH_VAL[TRUTH_E])
			&& (TRUE_VAL[TRUTH_F] == CALCULATED_TRUTH_VAL[TRUTH_F])
			&& (TRUE_VAL[TRUTH_G] == CALCULATED_TRUTH_VAL[TRUTH_G])
			&& (TRUE_VAL[TRUTH_H] == CALCULATED_TRUTH_VAL[TRUTH_H])
			&& (TRUE_VAL[TRUTH_I] == CALCULATED_TRUTH_VAL[TRUTH_I])
			&& (TRUE_VAL[TRUTH_J] == CALCULATED_TRUTH_VAL[TRUTH_J])
			&& (TRUE_VAL[TRUTH_K] == CALCULATED_TRUTH_VAL[TRUTH_K])
			&& (TRUE_VAL[TRUTH_L] == CALCULATED_TRUTH_VAL[TRUTH_L])
			&& (TRUE_VAL[TRUTH_M] == CALCULATED_TRUTH_VAL[TRUTH_M])
			&& (TRUE_VAL[TRUTH_N] == CALCULATED_TRUTH_VAL[TRUTH_N])
			&& (TRUE_VAL[TRUTH_T] == CALCULATED_TRUTH_VAL[TRUTH_T]))
			smart = true;
			
		if (count < min_loops)
			smart = false;

		//Increment number of iterations for max_loops exit
		++count;
		cout << "Training round " << count << " complete." << endl;
	}
	
	timestamp_t t_finish = get_timestamp();

	//Output final calculated weights for determining E or not E
	cout << endl << endl << "Final Calculated Weights: " << endl;
	for(int k = 0; k < NODES; ++k)
	{
		cout << WEIGHTS[k] << " ";
		if(k == 4 || k == 9 || k == 14 || k == 19)
			cout << endl;
	}
	cout << endl << endl;

	//Timing results
	double secs = (t_finish - t_start) / (MICROSECONDS_PER_SECOND * 1.0);
    long usecs = t_finish - t_start;
	cout << "Learning in parallel (secs): " << secs << ", usecs: "
              << usecs << endl;
			  
	cout << endl << endl;

	//Now we try asking the user for a letter to figure out what it is...
	//Initialize variables for user side computations
	int user_letter[25] = {};
	double result = 0;

	//Safety reset
	for(int a=0; a < LETTERS; ++a)
		{
			CALCULATED_TRUTH_VAL[a] = -1;
		}

	//Ask for user letter
	cout << "Enter 25 values representative of a letter, separated by spaces:" << endl;

	//Read user input into array for use
	for(int p = 0; p < 25; ++p)
	{
		cin >> user_letter[p];
	}
	//Formating
	cout << endl;

	//Calculate network value of user input
	result = test_letter(user_letter);

	//Check to see if results are representative of 'E'
	if(result > THRESHOLD)
	{
		cout << "The letter is an E." << endl;
	}
	else
	{
		cout << "The letter is NOT an E." << endl;
	}


	return 0;
}

void rnd_weight_init(double weights[])
{
	//Seed rand()
	srand(time(NULL));
	double w = 0;

	//Generate random weights
	for(int i = 0; i < NODES; ++i)
	{
		//Range random weights -1.0..1.0
		w = (rand() % 20 - 10);
		w/=10;
		weights[i] = w;
	}
	
}

void training(int training_set[], int truth)
{
	//Create threads
	thread threads[NODES] = {};

	//Thread loops
	for(int i = 0; i < NODES; ++i)
		{
			//Calculate current output
			threads[i] = thread(current_output, i, training_set);
		}
	for(int i = 0; i < NODES; ++i)
		{
			threads[i].join();
		}

	//Floor/Celeing for output
	int y = 0;
	if(REUSE_TOTAL > THRESHOLD)
		y = 1;
	else
		y = 0;

	//Store calculated truth value
	CALCULATED_TRUTH_VAL[truth] = y;

	//New thread loops
	for(int i = 0; i < NODES; ++i)
		{
			//Updating weights
			threads[i] = thread(update_weights, i, training_set, y, truth);
		}
	for(int i = 0; i < NODES; ++i)
		{
			threads[i].join();
		}

	//Reset reusable total
	REUSE_TOTAL = 0;
}
		
void current_output(int i, int set[])
{
	//Calculation wit weight
	double x = (WEIGHTS[i]*set[i]);

	//Sum up current outputs
	m.lock();
	REUSE_TOTAL += x;
	m.unlock();
}

void update_weights(int i, int set[], int y, int truth)
{
	//Update weights
	WEIGHTS[i] = WEIGHTS[i] + ALPHA*(TRUE_VAL[truth] - y)*set[i];
}

double test_letter(int training_set[])
{
	//Create threads
	thread threads[NODES] = {};
	REUSE_TOTAL = 0;

	//Thread loops
	for(int i = 0; i < NODES; ++i)
		{
			//Calculate current output
			threads[i] = thread(current_output, i, training_set);
		}
	for(int i = 0; i < NODES; ++i)
		{
			threads[i].join();
		}

	return REUSE_TOTAL;
}