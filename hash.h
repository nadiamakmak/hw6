#ifndef HASH_H
#define HASH_H

#include <iostream>
#include <cmath>
#include <random>
#include <chrono>

typedef std::size_t HASH_INDEX_T;

struct MyStringHash {
    HASH_INDEX_T rValues[5] { 983132572, 1468777056, 552714139, 984953261, 261934300 };
    MyStringHash(bool debug = true)
    {
        if(false == debug){
            generateRValues();
        }
    }
    // hash function entry point (i.e. this is h(k))
    HASH_INDEX_T operator()(const std::string& k) const
    {
        // Add your code here
        HASH_INDEX_T result = 0;

        //creating and filling an array that will hold all the integer values of each letter
        int stringArray[k.size()]; 

        for(unsigned int i=0; i<k.size(); i++){
            stringArray[i]=letterDigitToNumber(k[k.size()-1-i]);
        }

        //5 spots, 6 letters each spot
        unsigned long long w[5] = {0, 0, 0, 0, 0};
        int x = 5;
        int wSpot = 0;

        for(int i=4; i>-1; i--){ //starting from the back to account for ordering
            wSpot++;
            int minSpot = (wSpot-1)*6; //get the last spot in each grouping of 6

            //if there are less than 6 characters
            if(k.size() <= (unsigned int)x+1){
                for(int j=k.size()-1; j >= minSpot; j--){
                    if(j==minSpot){ //last spot in this grouping of 6, just add number (36^0=1)
                        w[i]=w[i]+stringArray[j];
                    }
                    else{ //linear time computation (from slide 12 of L19_NumberTheory)
                        w[i]=(w[i]+stringArray[j])*36;
                    }
                }
                break;
            }

            //6 letters or more
            else{
                for(int j=x; j>=x-5; j--){
                    if(j==x-5){ //last spot in this grouping of 6, just add number (36^0=1)
                        w[i]=w[i]+stringArray[j];
                    }
                    else{ //linear time computation (from slide 12 of L19_NumberTheory)
                        w[i]=(w[i]+stringArray[j])*36;
                    }
                }
                x = x+6;
            }
        }

        //multiply by rValues[i] (would be random if not for debugging purposes)
        for (int i=0; i<5; i++){
            result = result + (w[i]*rValues[i]);
        }
        return result;

    }

    // A likely helper function is to convert a-z,0-9 to an integral value 0-35
    HASH_INDEX_T letterDigitToNumber(char letter) const
    {
        // Add code here or delete this helper function if you do not want it
        int num = letter;

        if(num>=97 && num<=122){ //if lowercase (subtracting extra so that the value is the same as uppercase instead of converting the word to lowercase)
            return num-97;
        }
        else if (num>=65 && num<=90){ //if uppercase
            return num-65;
        }
        else if(num>=48 && num<=57){ //if a number
            return num-22;
        }
        return 0;

    }

    // Code to generate the random R values
    void generateRValues()
    {
        // obtain a seed from the system clock:
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 generator (seed);  // mt19937 is a standard random number generator

        // Simply call generator() [it has an operator()] to get another random number
        for(int i{ 0 }; i < 5; ++i)
        {
            rValues[i] = generator();
        }
    }
};

#endif
