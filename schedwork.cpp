#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please

#endif

#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here
//bool assignDay(const AvailabilityMatrix& avail, size_t dailyNeed, size_t maxShifts, DailySchedule& sched, vector<int> shiftsWorked, size_t day, size_t workerIndex);
bool assignDay(const AvailabilityMatrix& avail, size_t dailyNeed, size_t maxShifts, DailySchedule& sched, vector<int>& shiftsWorked, size_t day, size_t workerIndex);
bool canAssign(size_t day, size_t worker, const AvailabilityMatrix& avail, const vector<int>& shiftsWorked, const DailySchedule& sched, size_t maxShifts);

// Add your implementation of schedule() and other helper functions here

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0U){
        return false;
    }
    sched.clear();
    // Add your code below

    // get the total number of days and workers
    size_t totalDays = avail.size();
    size_t totalWorkers = avail[0].size();
    // size the scheduel based on the number of days
    for(size_t i = 0 ; i < totalDays; i++){
      sched.push_back(vector<Worker_T>());
    }
    // vector to track the shifts of each worker
    vector<int> shiftsWorked(totalWorkers, 0);
    // recursive backtracking
    return assignDay(avail, dailyNeed, maxShifts, sched, shiftsWorked, 0, 0);
}

// recursive helper function that assigns workers to days 
bool assignDay(const AvailabilityMatrix& avail, size_t dailyNeed, size_t maxShifts, DailySchedule& sched, vector<int>& shiftsWorked, size_t day, size_t workerIndex){
    // base case: all days have been properly scheduled
    if(day == avail.size()){ 
        return true;
    }
    // check if the current day is full, and if it is, move to assigning the next day with recursive call
    if(sched[day].size() == dailyNeed){
        return assignDay(avail, dailyNeed, maxShifts, sched, shiftsWorked, day + 1, 0);
    }
    // start assigning workers from the index
    for(size_t i = workerIndex; i < avail[0].size(); i++){
        // go through all the checks
        if(canAssign(day, i, avail, shiftsWorked, sched, maxShifts)){
          // assign the worker to the given day
          sched[day].push_back(i);
          shiftsWorked[i]++;
          // use recursion to assign the rest of the workers
          if(assignDay(avail, dailyNeed, maxShifts, sched, shiftsWorked, day, 0)){
            return true;
          }
          // backtrack if recursion fails
          sched[day].pop_back();
          shiftsWorked[i]--;
        }
    }
    return false; 
}

// helper to make sure that a shift can be assigned based off the constraints 
bool canAssign(size_t day, size_t worker, const AvailabilityMatrix& avail, const vector<int>& shiftsWorked, const DailySchedule& sched, size_t maxShifts){
  // check if the worker is available on given day
  if(!avail[day][worker]){
    return false;
  }
  // check if the worker has gone over shift limit
  if(shiftsWorked[worker] >= maxShifts){
    return false;
  }
  // check if worker has already been scheduled for given day
  if(find(sched[day].begin(), sched[day].end(), worker) != sched[day].end()){
    return false;
  }
  // return true if all checks pass
  return true;
}

