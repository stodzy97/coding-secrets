/**************************************************************************
*
*
*
*                            School of Engineering
*                  The Robert Gordon University, Aberdeen
*
***************************************************************************
*
*
*                  File name: main.cpp
*                  Author:    christopher stoddart
*                  Created:   16/04/2019
*                  Class:     EN4542
*
*
*
***************************************************************************
*
* Description: This programe trains a robot to trun in 4 diffirent directions:
* Move-Forward, Sharp Right Turn, Slight Right turn and Slight Left Turn. This is
* achieved through the use of a three layer neuron network, with 24 nearons on the first
* layer, 10 on the hidden layer and 4 on the output layer. a confusion matrix is employed
* that allows the accuracy of each output of the neuron network to be calculated.
*
*
*
***************************************************************************/

#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <math.h>


using namespace std;

double training_data [3831][24];
double test_data [1625][24];
double comparison_training_data [3831][4];
double comparison_test_data [1625][4];
double output_neurons [4];
double outer_weight [10][4];
double iner_weight [24][10];
double total_bais [10];
double total_bais_2 [4];
double B1 [10];
double error_margin [3831][4];
double error_margin_2 [3831][4];
double error_squared [3831][4];
double outer_weight_2 [10][4];
double error_margin_3 [1625][4];
double error_margin_4 [1625][4];
double bais_2 [4];
double Bprog [4];
double Bprog_2 [10];
double BA [3831][10];
double BB [3831][10];
double DBH [10];
double DHL [24][10];
double B = 1;

int inputs = 24;
int num_hidden_layers = 10;
int outputs = 4;
int training = 3831;
int testing = 1625;
int epochs = 1000;

double learning = 0.00012;
double random_bais [4];
double random_weight [24][10];
double random_bais_2  [10];
double random_weight_2 [10][4];
double sum = 0;
double MSE = 0;
double MSE_2 = 0;

int num_matches_MF;
int num_unmatched_MF_SlRT;
int num_unmatched_MF_ShRT;
int num_unmatched_MF_SlLT;
int num_unmatched_MF;
int num_matches_SlRT;
int num_unmatched_SlRT_MF;
int num_unmatched_SlRT_ShRT;
int num_unmatched_SlRT_SlLT;
int num_unmatched_SlRT;
int num_matches_ShRT;
int num_unmatched_ShRT_MF;
int num_unmatched_ShRT_SlRT;
int num_unmatched_ShRT_SlLT;
int num_unmatched_ShRT;
int num_matches_SlLT;
int num_unmatched_SlLT_MF;
int num_unmatched_SlLT_SlRT;
int num_unmatched_SlLT_ShRT;
int num_unmatched_SlLT;
int TP_MF;
int TN_MF;
int FP_MF;
int FN_MF;
int TP_SlRT;
int TN_SlRT;
int FP_SlRT;
int FN_SlRT;
int TP_ShRT;
int TN_ShRT;
int FP_ShRT;
int FN_ShRT;
int TP_SlLT;
int TN_SlLT;
int FP_SlLT;
int FN_SlLT;
int Total_error;
int total_matches;


int main()
{
/*************** activate the input and ouptut files *************************/

      ifstream input_file1;      // read from file 1
      ifstream input_file2;      // read from file 2
      ifstream input_file3;      // read from file 3
      ifstream input_file4;      // read from file 4
      ofstream output_file5;     // write to file 5
      ofstream output_file6;     // write to file 6

      input_file1.open("training_data_2.txt");  // open file as input file

      if (input_file1.is_open())              // check if the file is open
      {
          // if open do nothing
      }
      else
      {
          cout << "file 1 not open"<< endl;   // display message if not open
      }

      input_file2.open("testing_data_2.txt");  // open file as an input file

      if (input_file2.is_open())            // check if file is open
      {
          // if open do nothing
      }
      else
      {
          cout << "file 2 not open" << endl;   // if not open display message
      }

      input_file3.open("training_target_2.txt");   // open file as an input file

      if (input_file3.is_open())             // check if file is open
      {
        // if open do nothing
      }
      else
      {
          cout << "file 3 not open" << endl;   // if not display message
      }

      input_file4.open("testing_target_2.txt");    // open file as imput file

      if (input_file4.is_open())               // check if file is open
      {
        // if open do nothing
      }
      else
      {
          cout << "file 4 not open" << endl;      // if not display message
      }


      output_file5.open("MSE_1.txt");     // open file as an output file

      if (output_file5.is_open())        // check if file is open
      {
           // if open do nothing
      }
      else
      {
          cout << " file 5 not open " << endl;       // if not display message
      }

      output_file6.open("MSE_2.txt");    // open file as an output file

      if ( output_file6.is_open())       // check if file is open
      {
        // if open do nothing
      }
      else
      {
          cout << " file 6 not open" << endl;   // if not display message
      }

/************************************************************************/

/*************input training and testing data and targets into arrays********************/

      for (int a = 0; a < training ; a++)
      {
          for (int e = 0; e < inputs; e++)
          {
          input_file1 >> training_data[a][e];   // input the training data into an array
          }
      }

      for (int b = 0; b <testing; b ++)
      {
          for ( int f = 0; f < inputs ; f ++)  // input the tesing data into an array
          {
          input_file2 >> test_data[b][f];
          }
      }

      for (int c = 0; c < training; c ++)
      {
          for (int g = 0; g < outputs; g ++)
          {
              input_file3 >> comparison_training_data[c][g];   // input the training comparison data into an array
          }
      }

      for (int d = 0; d < testing; d++)
      {
          for (int h = 0; h < outputs; h++)
          {
              input_file4 >> comparison_test_data [d][h];    // input the testing comparison data into an array
          }
      }

      input_file1.close();            // close input file

      input_file2.close();            // close input file

      input_file3.close();            // close input file

      input_file4.close();            // close input file

/***************************************************************************/

/*************************generate random numbers**************************/

      for (int d = 0; d < outputs; d++ )
      {
          random_bais [d] = ( (double) rand ()/ double (RAND_MAX) - 0.5);   // generate random numbers between -0.5 and 0.5 and store inot an array
      }

      for (int z = 0; z < num_hidden_layers; z++)
      {

          random_bais_2 [z] = ( (double)rand ()/ double (RAND_MAX) - 0.5);   // generate random numbers between -0.5 and 0.5 and store them into an array
      }

      for (int g = 0; g < num_hidden_layers; g++)
      {
        for (int c = 0; c < inputs ; c++)
        {
          random_weight [c][g] = ( (double) rand ()/ double (RAND_MAX) - 0.5);    // generate random numbers between -0.5 and 0.5 and store them into an array
        }
      }
      for (int y = 0; y < outputs; y++)
      {
          for (int l = 0; l < num_hidden_layers; l++)
          {
             random_weight_2 [l][y] = ( (double) rand ()/ double (RAND_MAX) - 0.5);  // generate random values betwen -0.5 and 0.5 and store them into an array
          }
      }

/****************************************************************************/

/**************************************Feedfoward training***************************************************/

      for (int p = 0; p < epochs; p++)     // loop for the nunumber set to epoch
      {
         sum = 0;                     // reset the error sum back to 0

         for ( int i=0; i < training; i++)
         {

             for(int a = 0; a < outputs; a++)
                   {
                     total_bais_2[a] = 0;   // reset the total bais back to 0
                   }
             for(int b = 0; b < num_hidden_layers; b++)
                    {
                     total_bais[b] = 0;   // reset the total bais back to 0
                    }

             for ( int j=0;j < num_hidden_layers ; j++)
             {
                 for (int f = 0; f < inputs; f++)
                 {
                     iner_weight [f][j] = random_weight[f][j] * training_data [i][f];   // multiply the random generated value with the input value
                     total_bais [j] += iner_weight[f][j];    // add to the total bais
                 }
                total_bais[j] += random_bais_2[j];         // add the random value to the total bais
             }
             for (int o = 0; o < num_hidden_layers; o++)
             {

                 B1[o] =  (1 - exp((-total_bais[o]/B))) /(1+ exp((-total_bais[o]/B)));  // use the bipolar sigmoud to calcuate the total of each bias
             }


            for (int h = 0; h < outputs; h++)
            {
                for ( int r = 0; r < num_hidden_layers; r++)
                {
                    outer_weight[r][h] = random_weight_2[r][h] * B1[r];  // multiply random generated weight with the activation function
                    total_bais_2[h] += outer_weight[r][h];             // add to the total bais
                }
                 total_bais_2[h] += random_bais [h];               // add the random value to the
            }

            for (int k = 0; k < outputs; k++)
            {
              output_neurons [k] = (1 /(1+exp((-total_bais_2[k]/B))));   // use the unipolar sigmoud function to calculate the total of each output
            }

/**************************************************************************************/

/**************************** Error calculation*******************************************************************/
         for ( int num = 0; num < outputs; num++)
         {

             error_margin [i][num] = comparison_training_data[i][num] - output_neurons[num];          // calculate the error
             error_margin_2 [i][num] = output_neurons[num]*(1/B)* (1- output_neurons[num]) * error_margin [i][num];
             error_squared [i][num] = pow(error_margin [i][num],2.0);   // calculate the squared error
             sum += error_squared [i][num];    // add the squared error to the sum total
         }
/******************************************************************************************************************/

/***************************Backpropegation****************************************************/

         for (int c = 0; c < num_hidden_layers; c++)
         {
             for (int d = 0; d < outputs; d++)
             {
                 Bprog [d] = error_margin_2[i][d] * random_weight_2[c][d];  // times the new random weight value by the f'(u) of the output neurons
                 BA[i][c] += Bprog[d];

            BB[i][c] = BA[i][c]* (1/(2*B)) * (1 - pow(B1[c],2.0));   // calcuate the f'(u) for the hidden nuerons
            DBH[c]=  learning * BB[i][c];   // multiple by the learning rate
            random_bais_2[c] += DBH[c];      //  add the new calculated values to the generated random values for the output neurons

            for ( int e = 0; e < inputs; e++)
            {
                DHL[e][c] = learning * BB[i][c] * training_data[i][e];      // calculate new values for the random weights
                random_weight[e][c] += DHL[e][c];
            }

          }
         for ( int a = 0; a < outputs; a++)
         {
             for (int y = 0; y < num_hidden_layers; y++)
             {
               outer_weight_2[y][a] =  learning * error_margin_2[i][a] * B1[y];   //  calculate new values for the outer random weight
               random_weight_2 [y][a] += outer_weight_2[y][a];
              }
             bais_2 [a] =  learning * error_margin_2[i][a];  //  calculate new values for hidden neurons
            random_bais [a] += bais_2 [a];
         }
/**************************************************************************/

}
         }

        MSE = sum/training;    // calculate the MSE for the training
        output_file5 << MSE << endl;   // output the data to the text file
        sum = 0;   // reset the value for the sum back to 0

      }
/***************************************************************************/

        for ( int l = 0; l < testing ; l++)
        {

            for ( int a = 0; a < outputs; a++)
            {
                total_bais_2 [a] = 0;   // reset the array values back to 0
            }

            for ( int b = 0; b < num_hidden_layers; b++)
            {
                total_bais [b] = 0;   // reset the array values back to 0
            }

            for ( int c = 0; c < num_hidden_layers; c++)
            {
                for (int d = 0; d < inputs; d++)
                {
                    iner_weight[d][c] = random_weight[d][c] * test_data[l][d];   // multiply the random value with the test data
                    total_bais [c] += iner_weight [d][c];   // add to the total bais
                }
                total_bais [c] += random_bais_2 [c];    // add the random generate value to the random bias
            }

            for (int o = 0; o < num_hidden_layers; o++)
            {

                B1[o] =  (1 - exp((-total_bais[o]/B)))/(1+ exp((-total_bais[o]/B)));   // apply the bipolar sigmond function
            }


            for (int e = 0; e < outputs; e++)
            {
                for ( int f = 0; f < num_hidden_layers; f++)
                {
                    outer_weight [f][e] = random_weight_2 [f][e] * B1 [f];   // calaculte value for the weight
                    total_bais_2 [e] += outer_weight [f][e];
                }
                total_bais_2 [e] += random_bais [e];            // calculate value for the output neurons
            }

            for (int g = 0; g < outputs; g++)
            {
              output_neurons [g] = (1/(1+exp((-total_bais_2[g]/B))));   // apply the unipolar sigmoud function
            }

            for ( int h = 0; h < outputs; h++)
            {
               error_margin_3 [l][h] = comparison_test_data[l][h] - output_neurons[h];   // calculate the error
               error_margin_4 [l][h]= pow(error_margin_3 [l][h],2.0);        // square the error
               sum += error_margin_4 [l][h];    // add the eror to the sum of errors
            }

/*********************************************************************************************/

/*******************************************************Confusion matrix**********************************************************************************/

        if ( comparison_test_data[l][0] == 1 && comparison_test_data[l][1] == 0 && comparison_test_data [l][2] == 0 && comparison_test_data [l][3] == 0)    // if the comparison output is MF
        {
           if ( output_neurons [0] >= 0.5 && output_neurons [1] < 0.5 && output_neurons [2] < 0.5 && output_neurons [3] < 0.5)  // if the output is MF
           {
               num_matches_MF += 1;
           }
           else if ( output_neurons [0] < 0.5 && output_neurons [1] >= 0.5 && output_neurons [2] < 0.5 && output_neurons [3] < 0.5)  // if the output is SlRT
           {
               num_unmatched_MF_SlRT += 1;
           }
           else if ( output_neurons [0] < 0.5 && output_neurons [1] < 0.5 && output_neurons [2] >= 0.5 && output_neurons [3] < 0.5)  // if the ouput is ShRT
           {
               num_unmatched_MF_ShRT +=1;
           }
           else if ( output_neurons [0] < 0.5 && output_neurons [1] < 0.5 && output_neurons [2] < 0.5 && output_neurons [3] >= 0.5)  // if the output is SlLT
           {
               num_unmatched_MF_SlLT +=1;
           }
           else   // if it doesnt match an output
           {
               num_unmatched_MF += 1;
           }
        }
        else if ( comparison_test_data [l][0] == 0 && comparison_test_data [l][1] == 1 && comparison_test_data [l][2] == 0 && comparison_test_data [l][3] == 0)  // if the comparsion output is SlRT
        {
            if ( output_neurons [0] < 0.5 && output_neurons [1] >= 0.5 && output_neurons [2] < 0.5 && output_neurons [3] < 0.5)  // if the output is SlRT
            {
                num_matches_SlRT += 1;
            }
            else if ( output_neurons [0] >= 0.5 && output_neurons [1] < 0.5 && output_neurons [2] < 0.5 && output_neurons [3] < 0.5)  // if the output is MF
            {
                num_unmatched_SlRT_MF += 1;
            }
            else if ( output_neurons [0] < 0.5 && output_neurons [1] < 0.5 && output_neurons [2] >= 0.5 && output_neurons [3] < 0.5)  // if the output is ShRT
            {
                num_unmatched_SlRT_ShRT += 1;
            }
            else if ( output_neurons [0] < 0.5 && output_neurons [1] < 0.5 && output_neurons [2] < 0.5 && output_neurons [3] >= 0.5)  // if the output is SlLT
            {
                num_unmatched_SlRT_SlLT += 1;
            }
            else         // if it doesnt match an output
            {
                num_unmatched_SlRT += 1;
            }
        }
        else if ( comparison_test_data [l][0] == 0 && comparison_test_data [l][1] ==0 && comparison_test_data [l][2] == 1 && comparison_test_data[l][3] == 0)   // if the comparison output is ShRT
        {
            if ( output_neurons [0] < 0.5 && output_neurons [1] < 0.5 && output_neurons [2] >= 0.5 && output_neurons [3] < 0.5)   // if the output is ShRT
            {
                num_matches_ShRT += 1;
            }
            else if ( output_neurons [0] >= 0.5 && output_neurons [1] < 0.5 && output_neurons [2] < 0.5 && output_neurons [3] < 0.5)   // if the output is MF
            {
                num_unmatched_ShRT_MF += 1;
            }
            else  if ( output_neurons [0] < 0.5 && output_neurons [1] >= 0.5 && output_neurons [2] < 0.5 && output_neurons [3] < 0.5) // if the output is SlRT
            {
                num_unmatched_ShRT_SlRT += 1;
            }
            else if ( output_neurons [0] < 0.5 && output_neurons [1] < 0.5 && output_neurons [2] < 0.5 && output_neurons [3] >= 0.5)  // if the output is SlLT
            {
                num_unmatched_ShRT_SlLT += 1;
            }
            else  // if it doesnt match any output
            {
                num_unmatched_ShRT += 1;
            }
        }
         else if ( comparison_test_data [l][0] == 0 && comparison_test_data [l][1] ==0 && comparison_test_data [l][2] == 0 && comparison_test_data[l][3] == 1) // if the comparison output is SlLT
        {
             if ( output_neurons [0] < 0.5 && output_neurons [1] < 0.5 && output_neurons [2] < 0.5 && output_neurons [3] >= 0.5)   // if the output is SlLT
             {
                 num_matches_SlLT += 1;
             }
             else  if ( output_neurons [0] >= 0.5 && output_neurons [1] < 0.5 && output_neurons [2] < 0.5 && output_neurons [3] < 0.5)  // if the output is MF
             {
                 num_unmatched_SlLT_MF += 1;
             }
             else  if ( output_neurons [0] < 0.5 && output_neurons [1] >= 0.5 && output_neurons [2] < 0.5 && output_neurons [3] < 0.5)  // if the output is SlRT
             {
                 num_unmatched_SlLT_SlRT += 1;
             }
             else if ( output_neurons [0] < 0.5 && output_neurons [1] < 0.5 && output_neurons [2] >= 0.5 && output_neurons [3] < 0.5)   // if the output is ShRT
             {
                 num_unmatched_SlLT_ShRT += 1;
             }
             else  // if it doesnt match any output
             {
                 num_unmatched_SlLT += 1;
             }
        }

          //calculate TP,TN,FP and FN for MF
          TP_MF = num_matches_MF;
          TN_MF = num_matches_SlRT + num_unmatched_SlRT_ShRT +  num_unmatched_SlRT_SlLT +  num_matches_ShRT +  num_unmatched_ShRT_SlRT + num_unmatched_ShRT_SlLT +  num_matches_SlLT + num_unmatched_SlLT_SlRT + num_unmatched_SlLT_ShRT;
          FP_MF = num_unmatched_MF_SlRT + num_unmatched_MF_ShRT +  num_unmatched_MF_SlLT;
          FN_MF = num_unmatched_SlLT_MF + num_unmatched_ShRT_MF + num_unmatched_SlLT_MF;

          //calculate TP,TN,FP and FN for SlRT
          TP_SlRT = num_matches_SlRT;
          TN_SlRT = num_matches_MF + num_unmatched_MF_ShRT + num_unmatched_MF_SlLT  + num_matches_ShRT + num_unmatched_ShRT_MF + num_unmatched_ShRT_SlLT + num_matches_SlLT + num_unmatched_SlLT_MF + num_unmatched_SlLT_ShRT;
          FP_SlRT = num_unmatched_SlRT_MF + num_unmatched_SlRT_ShRT + num_unmatched_SlRT_SlLT;
          FN_SlRT = num_unmatched_MF_SlRT + num_unmatched_ShRT_SlRT + num_unmatched_SlLT_SlRT;

          //calculate TP,TN,FP and FN for ShRT
          TP_ShRT = num_matches_ShRT;
          TN_ShRT = num_matches_MF + num_unmatched_MF_SlRT + num_unmatched_MF_SlLT + num_matches_SlRT + num_unmatched_SlRT_MF + num_unmatched_SlRT_SlLT + num_matches_SlLT + num_unmatched_SlLT_MF + num_unmatched_SlLT_SlRT;
          FP_ShRT = num_unmatched_ShRT_MF + num_unmatched_ShRT_SlRT + num_unmatched_ShRT_SlLT;
          FN_ShRT = num_unmatched_SlLT_ShRT + num_unmatched_SlRT_ShRT +  num_unmatched_MF_ShRT;

          //calculate TP,TN,FP and FN for SlLT
          TP_SlLT = num_matches_SlLT;
          TN_SlLT = num_matches_ShRT + num_unmatched_ShRT_MF + num_unmatched_ShRT_SlRT +  num_matches_SlRT +  num_unmatched_SlRT_MF + num_unmatched_SlRT_ShRT + num_matches_MF + num_unmatched_MF_SlRT + num_unmatched_MF_ShRT;
          FP_SlLT = num_unmatched_SlLT_MF + num_unmatched_SlLT_SlRT +  num_unmatched_SlLT_ShRT;
          FN_SlLT = num_unmatched_ShRT_SlLT + num_unmatched_SlRT_SlLT + num_unmatched_MF_SlLT;

          Total_error = num_unmatched_MF + num_unmatched_SlRT + num_unmatched_ShRT + num_unmatched_SlLT;  // calculate the total error
          total_matches = num_matches_SlLT + num_matches_ShRT + num_matches_SlRT + num_matches_MF;    // calculate the matched results

/************************************************************************************************************************************************/

}

        MSE_2 = sum / testing;    // calculate the MSE for the testing

        output_file6 << MSE_2 << endl;   // output the data to the text file

/**********************output results from confusion matrix******************************/

      cout << " TP_MF is: " << TP_MF << endl;
      cout << " TN_MF is: " << TN_MF<< endl;
      cout << " FP_MF is: " << FP_MF << endl;
      cout << " FN_MF is: " << FN_MF << endl;
      cout << " TP_SlRT is: " << TP_SlRT << endl;
      cout << " TN_SlRT is: " << TN_SlRT << endl;
      cout << " FP_SlRT is: " << FP_SlRT << endl;
      cout << " FN_SlRT is: " << FN_SlRT << endl;
      cout << " TP_ShRT is: " << TP_ShRT<< endl;
      cout << " TN_ShRT is: " << TN_ShRT<< endl;
      cout << " FP_ShRT is: " << FP_ShRT << endl;
      cout << " FN_ShRT is: " << FN_ShRT << endl;
      cout << " TP_SlLT is: " << TP_SlLT << endl;
      cout << " TN_SlLT is: " << TN_SlLT << endl;
      cout << " FP_SlLT is: " << FP_SlLT << endl;
      cout << " FN_SlLT is: " << FN_SlLT<< endl;
      cout << " Total error is: " << Total_error << endl;
      cout << " Toal number of matches is: " << total_matches << endl;


/*****************************************************************************************/

      return  0;
}







