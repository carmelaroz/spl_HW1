//
// Created by spl211 on 11/11/2021.
//

#include "Customer.h"
#include <algorithm>
#include <limits.h>
using namespace std;


Customer::Customer(std::string c_name, int c_id) :
    name(c_name), id(c_id){};

Customer::~Customer(){};

string Customer::getName() const {
    return this->name;
}

int Customer::getId() const {
    return this->id;
}

SweatyCustomer::SweatyCustomer(string name, int id) : Customer(name, id){}
vector<int> SweatyCustomer::order(const vector<Workout> &workout_options) {
    vector<int> ans;
    for(Workout w : workout_options) {
        if(w.getType() == CARDIO)
            ans.push_back(w.getId());
    }
    return ans;
}
string SweatyCustomer::toString() const {
    return "swt";
}

CheapCustomer::CheapCustomer(string name, int id) : Customer(name, id){}
vector<int> CheapCustomer::order(const vector<Workout> &workout_options) {
    vector<int> ans;
    int id = workout_options.begin()->getId();
    int cheapest = workout_options.begin()->getPrice();
    for(Workout w : workout_options) {
        if(cheapest > w.getPrice()) {
            cheapest = w.getPrice();
            id = w.getId();
        }
        if(cheapest == w.getPrice()) {
            if(id > w.getId()) {
                cheapest = w.getPrice();
                id = w.getId();
            }
        }
    }
    ans.push_back(id);
    return ans;
}
string CheapCustomer::toString() const {
    return "chp";
}

HeavyMuscleCustomer::HeavyMuscleCustomer(string name, int id) : Customer(name, id){}
vector<int> HeavyMuscleCustomer::order(const vector<Workout> &workout_options) {
    vector<int> sorted;
    vector<int> ans;
    for(Workout w : workout_options){
        if(w.getType() == ANAEROBIC)
            sorted.push_back(w.getPrice());
    }
    sort(sorted.begin(),sorted.end());
    for(int i = sorted.size() - 1;i >= 0; i--){
        for(Workout w : workout_options){
            if(w.getPrice() == sorted[i] && w.getType() == ANAEROBIC)
                ans.push_back(w.getId());
        }
    }
    return ans;
}
string HeavyMuscleCustomer::toString() const {
    return "mcl";
}

FullBodyCustomer::FullBodyCustomer(string name, int id) : Customer(name, id){}
vector<int> FullBodyCustomer::order(const vector<Workout> &workout_options) {
    vector<int> ans;
    int cheapest = INT_MAX;
    int id;
    for(Workout w : workout_options) {
        if (w.getType() == CARDIO) {
            if (w.getPrice() < cheapest) {
                cheapest = w.getPrice();
                id = w.getId();
            }
            if (cheapest == w.getPrice()) {
                if (id > w.getId()) {
                    cheapest = w.getPrice();
                    id = w.getId();
                }
            }
        }
    }
    ans.push_back(id);
    id = 0;
    int expensive = 0;
    for(Workout w : workout_options) {
        if(w.getType() == MIXED) {
            if(w.getPrice() > expensive) {
                expensive = w.getPrice();
                id = w.getId();
            }
            if(expensive == w.getPrice()) {
                if(id > w.getId()) {
                    expensive = w.getPrice();
                    id = w.getId();
                }
            }
        }
    }
    ans.push_back(id);

    id = 0;
    for(Workout w : workout_options) {
        if (w.getType() == ANAEROBIC) {
            if (w.getPrice() < cheapest) {
                cheapest = w.getPrice();
                id = w.getId();
            }
            if (cheapest == w.getPrice()) {
                if (id > w.getId()) {
                    cheapest = w.getPrice();
                    id = w.getId();
                }
            }
        }
    }
    ans.push_back(id);
    return ans;
}
string FullBodyCustomer::toString() const {
    return "fbd";
}