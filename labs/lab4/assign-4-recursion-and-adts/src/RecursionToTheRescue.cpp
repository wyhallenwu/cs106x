#include "RecursionToTheRescue.h"
#include "map.h"
#include <climits>
#include <iostream>
#include "Disasters.h"
using namespace std;

/* * * * Doctors Without Orders * * * */

template<typename T>
int get_index(Vector<T>& vec, T& v){
    for(int i = 0; i < vec.size(); i++){
        if(vec[i].name == v.name)
            return i;
    }
    return -1;
}

bool is_valid(const Vector<Patient>& patients){
    return patients.isEmpty();
}

Vector<int> get_all_valid_doctors(const Vector<Doctor>& doctors, const Patient& patient){
    Vector<int> candidates;
    for(int i = 0; i < doctors.size(); i++){
        if(patient.hoursNeeded <= doctors[i].hoursFree)
            candidates.add(i);
    }
    return candidates;
}

void update(Patient& patient, int doctor_index, Vector<Doctor>& doctors, Vector<Patient>& patients, Map<string, Set<string>>& nschedule){
    patients.remove(0);
    doctors[doctor_index].hoursFree -= patient.hoursNeeded;
    nschedule[doctors[doctor_index].name].add(patient.name);
}

void back(Patient& patient, int doctor_index, Vector<Doctor>& doctors, Vector<Patient>& patients, Map<string, Set<string>>& nschedule){
    patients.insert(0, patient);
    doctors[doctor_index].hoursFree += patient.hoursNeeded;
    nschedule[doctors[doctor_index].name].remove(patient.name);
}

void search(Vector<Doctor>& doctors, Vector<Patient>& patients, Map<string, Set<string>>& nschedule, Map<string, Set<string>>& schedule, bool& flag){
    if(is_valid(patients)){
        schedule = nschedule;
        flag = true;
        return;
    }
    Patient p = patients.front();
    Vector<int> candidates = get_all_valid_doctors(doctors, p);
    for(auto& candidate: candidates){
        update(p, candidate, doctors, patients, nschedule);
        search(doctors, patients, nschedule, schedule, flag);
        back(p, candidate, doctors, patients, nschedule);
    }
}

/**
 * Given a list of doctors and a list of patients, determines whether all the patients can
 * be seen. If so, this function fills in the schedule outparameter with a map from doctors
 * to the set of patients that doctor would see.
 *
 * @param doctors  The list of the doctors available to work.
 * @param patients The list of the patients that need to be seen.
 * @param schedule An outparameter that will be filled in with the schedule, should one exist.
 * @return Whether or not a schedule was found.
 */
bool canAllPatientsBeSeen(const Vector<Doctor>& doctors,
                          const Vector<Patient>& patients,
                          Map<string, Set<string>>& schedule) {
    // [TODO: Delete these lines and implement this function!]
    Vector<Doctor> ndoctors = doctors;
    Vector<Patient> npatients = patients;
    Map<string, Set<string>> nschedule = schedule;
    bool flag = false;
    search(ndoctors, npatients, nschedule, schedule, flag);
    return flag;
}

/* * * * Disaster Planning * * * */

Set<string> get_covered_cities(const Map<string, int>& covered_cities){
    Set<string> covered;
    for(auto& city: covered_cities.keys()){
        if(covered_cities[city] > 0){
            covered.add(city);
        }
    }
    return covered;
}

bool is_valid_locations(const Set<string>& cities, const Map<string, int>& covered_cities, int& num_cities){
    return get_covered_cities(covered_cities) == cities && num_cities >= 0;
}


Set<string> get_candidate_cities(Set<string>& cities, const Map<string, int>& covered_cities, const Map<string, Set<string>>& roadNetwork){
    Set<string> ncities = cities;
    // find all cities that can cover the uncovered cities
    // TODO: not so good. can pass all except veryhard task.
//    Set<string> covered = get_covered_cities(covered_cities);
//    ncities.difference(covered);
//    for(auto& city: ncities - covered){
//        ncities.addAll(roadNetwork.get(city));
//    }
//    return ncities.difference(nlocations);
//    return ncities - nlocations;
      return ncities.difference(get_covered_cities(covered_cities));
}

void update_locations(string candidate_city, int& num_cities, Set<string>& nlocations, const Map<string, Set<string>>& roadNetwork, Map<string, int>& covered_cities){
    num_cities -= 1;
    nlocations.add(candidate_city);
    covered_cities[candidate_city] += 1;
    for(auto& city: roadNetwork[candidate_city]){
        covered_cities[city] += 1;
    }
}

void back_locations(string candidate_city, int& num_cities, Set<string>& nlocations, const Map<string, Set<string>>& roadNetwork, Map<string, int>& covered_cities){
    num_cities += 1;
    nlocations.remove(candidate_city);
    covered_cities[candidate_city] = max(0, covered_cities[candidate_city] - 1);
    for(auto& city: roadNetwork[candidate_city]){
        covered_cities[city] = max(0, covered_cities[city] - 1);
    }
}
// C:\Users\allenwu\Desktop\cs106x\labs\lab4\assign-4-recursion-and-adts\res\DisasterPlanning\BritishIsles.dst 4
// C:\Users\allenwu\Desktop\cs106x\labs\lab4\assign-4-recursion-and-adts\res\DisasterPlanning\NortheastUS.dst 7
// C:\Users\allenwu\Desktop\cs106x\labs\lab4\assign-4-recursion-and-adts\res\DisasterPlanning\WesternUS.dst 5

void search(const Map<string, Set<string>>& roadNetwork,
            int& numCities,
            Set<string>& locations, Set<string>& nlocations, Set<string>& cities, Map<string, int>& covered_cities, bool& flag){
    if(!flag && is_valid_locations(cities, covered_cities, numCities)){;
        locations = nlocations;
        flag = true;
        return;
    } else if(flag || numCities <= 0){
        return;
    }
    Set<string> candidate_cities = get_candidate_cities(cities, covered_cities, roadNetwork);
//    cout << "candidates: " << candidate_cities << endl;
    for(auto& city: candidate_cities){
        update_locations(city, numCities, nlocations, roadNetwork, covered_cities);
        search(roadNetwork, numCities, locations, nlocations, cities, covered_cities, flag);
        back_locations(city, numCities, nlocations, roadNetwork, covered_cities);
    }
}

/**
 * Given a transportation grid for a country or region, along with the number of cities where disaster
 * supplies can be stockpiled, returns whether it's possible to stockpile disaster supplies in at most
 * the specified number of cities such that each city either has supplies or is connected to a city that
 * does.
 * <p>
 * This function can assume that every city is a key in the transportation map and that roads are
 * bidirectional: if there's a road from City A to City B, then there's a road from City B back to
 * City A as well.
 *
 * @param roadNetwork The underlying transportation network.
 * @param numCities   How many cities you can afford to put supplies in.
 * @param locations   An outparameter filled in with which cities to choose if a solution exists.
 * @return Whether a solution exists.
 */
bool canBeMadeDisasterReady(const Map<string, Set<string>>& roadNetwork,
                            int numCities,
                            Set<string>& locations) {
    // [TODO: Delete these lines and implement this function!]
    Set<string> cities;
    Map<string, int> covered_cities;
    for(auto& city: roadNetwork.keys()){
        cities.add(city);
        covered_cities[city] = 0;
    }
    auto nlocations = locations;
    bool flag = false;
    search(roadNetwork, numCities, locations, nlocations, cities, covered_cities, flag);
    cout << locations.toString() << endl;
    return flag;
}


/* * * * Winning the Election * * * */

int get_least_popularvotes(int popular_votes){
    return popular_votes / 2 + 1;
}

void step(MinInfo& mininfo, const State& state, int& electoralVotesNeeded){
    mininfo.popularVotesNeeded += get_least_popularvotes(state.popularVotes);
    mininfo.statesUsed.push_back(state);
    electoralVotesNeeded -= state.electoralVotes;
}

void backtrack(MinInfo& mininfo, const State& state, int& electoralVotesNeeded){
    mininfo.popularVotesNeeded -= get_least_popularvotes(state.popularVotes);
    mininfo.statesUsed.pop_back();
    electoralVotesNeeded += state.electoralVotes;
}

void update_mininfo(MinInfo& origin, MinInfo& ninfo){
    origin.popularVotesNeeded += ninfo.popularVotesNeeded;
    origin.statesUsed.addAll(ninfo.statesUsed);
}

/**
 * Given a list of the states in the election, including their popular and electoral vote
 * totals, and the number of electoral votes needed, as well as the index of the lowest-indexed
 * state to consider, returns information about how few popular votes you'd need in order to
 * win that at least that many electoral votes.
 *
 * @param electoralVotesNeeded the minimum number of electoral votes needed
 * @param states All the states in the election (plus DC, if appropriate)
 * @param minStateIndex the lowest index in the states Vector that should be considered
 */
void minPopularVoteToGetAtLeast(int electoralVotesNeeded, const Vector<State>& states, int minStateIndex, MinInfo& result, MinInfo& intermediate) {
    // [TODO: Delete these lines and implement this function!]
    if(electoralVotesNeeded <= 0){
        if(intermediate.popularVotesNeeded < result.popularVotesNeeded){
            result = intermediate;
        }
        return;
    }
    for(int i = minStateIndex; i < states.size(); i++){
        step(intermediate, states[i], electoralVotesNeeded);
        minPopularVoteToGetAtLeast(electoralVotesNeeded, states, i + 1, result, intermediate);
        backtrack(intermediate, states[i], electoralVotesNeeded);
    }
}

/**
 * Given a list of all the states in an election, including their popular and electoral vote
 * totals, returns information about how few popular votes you'd need to win in order to win
 * the presidency.
 *
 * @param states All the states in the election (plus DC, if appropriate)
 * @return Information about how few votes you'd need to win the election.
 */
MinInfo minPopularVoteToWin(const Vector<State>& states) {
    // [TODO: Delete these lines and implement this function!]
    int all_electoralVotes = [&]()->int{
        int sum = 0;
        for(auto& v: states){
            sum += v.electoralVotes;
        }
        return sum;
    }();
    MinInfo result{INT_MAX, Vector<State>{}};
    MinInfo intermediate{};
    minPopularVoteToGetAtLeast(all_electoralVotes / 2 + 1, states, 0, result, intermediate);
    return result;
}
