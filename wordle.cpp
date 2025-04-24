#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;


// Add prototypes of helper functions here
void wordleHelper(std::string current, const std::string& original, std::string floating, int index, int dashes, const set<std::string>& dict, set<std::string>& results);
int dashCounter(const std::string& s);

// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    // Add your code here
    // create a set of strings to hold the results
    set<std::string> results;
    // call the dash helper function to count the amount of unknown positions
    int dashes = dashCounter(in);
    // call the wordle helper function to begin the search
    wordleHelper(in, in, floating, 0, dashes, dict, results);
    // return the results
    return results;
}

// Define any helper functions here

// wordle helper function to recursively find valid words
void wordleHelper(std::string current, const std::string& original, std::string floating, int index, int dashes, const set<std::string>& dict, set<std::string>& results){
    // base case: the word has been filled 
    if(index == current.length()){ 
        // make sure that the word is valid and there are no extra floating letters
        if(dict.find(current) != dict.end() && floating.empty()){
            results.insert(current);
        }
        return;
    }
    // skip any fixed positions and recurse through the next position
    if(original[index] != '-'){
        wordleHelper(current, original, floating, index + 1, dashes, dict, results);
        return;
    }
    // loop through floating letters for this position
    for(int i = 0; i < floating.length(); i++){
        // get the floating letter
        char x = floating[i];
        // put it at the current index 
        current[index] = x;
        // remove the floating letter for the next recursive call;
        std::string nextFloating = floating;
        nextFloating.erase(i, 1);
        // recurse to the next index with one less unknown position
        wordleHelper(current, original, nextFloating, index + 1, dashes - 1, dict, results);
    }
    // if there are more unkown positions than floating letters, try more letters
    if(floating.length() < dashes){
        for(char i = 'a'; i <= 'z'; i++){
            // use a bool to avoid reusing floating letters
            bool floatCheck = false;
            for(int j = 0; j < floating.length(); j++){
                if(floating[j] == i){
                    floatCheck = true;
                    break;
                }
            }
            // if the letter was not a floating letter, go ahead and try it
            if(!floatCheck){
                // place it at the current position
                current[index] = i;
                // recurse through the next position
                wordleHelper(current, original, floating, index + 1, dashes - 1, dict, results);
            }
        }
    }
}

// helper function to count the amount of dashes (unknown positions) in the input string
int dashCounter(const std::string& s){
    // initialize counters, loop through string, return count
    int count = 0;
    for(int i = 0; i < s.length(); i++){
        if(s[i] == '-'){
            count++;
        }
    }
    return count;
}
