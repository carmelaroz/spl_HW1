//
// Created by spl211 on 11/11/2021.
//

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstring>
#include "Studio.h"
//#include "Action.h"
#include "Customer.h"

using namespace std;

Studio::Studio() : open(false), trainers(), workout_options(), actionsLog() {};

Studio::~Studio() {
    workout_options.clear();  // check
    for (Trainer *t: trainers) {
        delete t;
        t = nullptr;
    }
    trainers.clear();
    for (BaseAction *a: actionsLog) {
        delete a;
        a = nullptr;
    }
    actionsLog.clear();
}

Studio::Studio(const Studio &other) : open(other.open), trainers(), workout_options(), actionsLog(){   // copy constructor
    for(Trainer* t : other.trainers) {
        Trainer* trainer = new Trainer(*t);
        trainers.push_back(trainer);
    }
    for(Workout w : other.workout_options) {
        workout_options.push_back(w);
    }
    for(BaseAction* b : other.actionsLog) {
        actionsLog.push_back(b->clone());
    }
}

Studio::Studio(const Studio &&other) : open(other.open), trainers(), workout_options(), actionsLog(){   // move constructor
    for(Trainer* t : other.trainers) {
        trainers.push_back(t);
        t = nullptr;
    }
    for(Workout w : other.workout_options) {
        workout_options.push_back(w);
    }
    for(BaseAction* b : other.actionsLog) {
        actionsLog.push_back(b);
        b = nullptr;
    }
}

Studio &Studio::operator=(const Studio &other) { // copy assignment operator
    if(this != &other) {
        (*this).clear();
        open = other.open;
        for(Trainer* t : other.trainers) {
            Trainer* tr = new Trainer(*t);
            trainers.push_back(tr);
        }
        for(Workout w : other.workout_options) {
            workout_options.push_back(w);
        }
        for(BaseAction* b : other.actionsLog) {
            actionsLog.push_back(b->clone());
        }
    }
    return *this;
}

Studio &Studio::operator=(Studio &&other) {  // The move assignment operator
    if(this != &other) {
        clear();
        open = other.open;
        for(Trainer* t : other.trainers) {
            trainers.push_back(t);
            t = nullptr;
        }
        for(Workout w : other.workout_options) {
            workout_options.push_back(w);
        }
        for(BaseAction* b : other.actionsLog) {
            actionsLog.push_back(b);
            b = nullptr;
        }
    }
    return *this;
}

void Studio::clear() {
    open = false;
    for(Trainer* t : trainers) {
        delete t;
        t = nullptr;
    }
    trainers.clear();
    workout_options.clear();
    for(BaseAction* b : actionsLog) {
        delete b;
        b = nullptr;
    }
    actionsLog.clear();

}

Studio::Studio(const string &configFilePath) : open(false), trainers(), workout_options(), actionsLog() {
    std::fstream newFile(configFilePath);
    string line;
    int numOfTrainers = 0;
    int readCapacities = 0;
    int workout_id = 0;
    while (getline(newFile, line)) {
        if (line.empty() || line[0] == '#')
            continue;
        if (numOfTrainers == 0) {
            stringstream num(line);
            num >> numOfTrainers;
            continue;
        }
        if (readCapacities == 0) {
            long unsigned int i = 0;
            int trainer_id = 0;
            while (i < line.size()){
                Trainer *T = new Trainer(trainer_id, line[i]-'0'); // fix to two-digital or more
                line.erase(0,1);
                trainers.push_back(T);
                trainer_id += 1;
                if((line[i] != '\0')){
                        i += 1;
                }
            }
        readCapacities = 1;
        } else {
            string str_obj(line);
            char *ptr = &str_obj[0];
            ptr = strtok(ptr, ",");
            std::string name(ptr);
            ptr = strtok(NULL, ",");
            std::string workout(ptr);
            ptr = strtok(NULL, ",");
            int price = strtol(ptr, nullptr, 10);
            if (workout == " Anaerobic")
                workout_options.push_back(Workout(workout_id, name, price, WorkoutType::ANAEROBIC));
            if (workout == " Mixed")
                workout_options.emplace_back(Workout(workout_id, name, price, WorkoutType::MIXED));
            if (workout == " Cardio")
                workout_options.emplace_back(Workout(workout_id, name, price, WorkoutType::CARDIO));
            workout_id++;
            continue;
        }

    }
    newFile.close(); //close the file object.
}

void Studio::start() {
    open = true;
    cout << "Studio is now open!" << endl;
    string str;
    int customer_counter = 0;
        while(getline(cin, str)) {
            if (str != "closeall") {
                string cmd = str.substr(0, str.find(" "));
                str.erase(0, str.find(" " )+1);
                if (cmd == "open") {
                    vector<Customer *> customersList;
                    int tid = stoi(str.substr(0, str.find(" "))); // get trainer id
                    str.erase(0, str.find(" ") + 1);
                    while (str.size() != 0) {
                        if(str[0] == ' ')
                            str.erase(0,1);
                        string customer = str.substr(0, str.find(","));  // get customer name
                        str.erase(0, str.find(",")+1);
                        string type = str.substr(0, str.find(" "));// get customer type
                        str.erase(0, str.find(" "));
                        if (type == "swt") {
                            SweatyCustomer *c = new SweatyCustomer(customer, customer_counter);
                            customersList.push_back(c);
                        }
                        if (type == "chp") {
                            CheapCustomer *c = new CheapCustomer(customer, customer_counter);
                            customersList.push_back(c);
                        }
                        if (type == "mcl") {
                            HeavyMuscleCustomer *c = new HeavyMuscleCustomer(customer, customer_counter);
                            customersList.push_back(c);
                        }
                        if (type == "fbd") {
                            FullBodyCustomer *c = new FullBodyCustomer(customer, customer_counter);
                            customersList.push_back(c);
                        }
                        customer_counter++;
                    }
                    OpenTrainer *open = new OpenTrainer(tid, customersList);
                    open->act(*this);
                    actionsLog.push_back(open);
                }
                if (cmd == "order") {
                    int tid = stoi(str.substr(0, str.find(" "))); // get trainer id
                    Order *order = new Order(tid);
                    order->act(*this);
                    actionsLog.push_back(order);
                }
                if (cmd == "move") {
                    int tsrc = stoi(str.substr(0, str.find(" ")));
                    str.erase(0, str.find(" ") + 1);
                    int tdst = stoi(str.substr(0, str.find(" ")));
                    str.erase(0, str.find(" ") + 1);
                    int cid = stoi(str.substr(0, str.find(" ")));
                    str.erase(0, str.find(" ") + 1);
                    MoveCustomer *move = new MoveCustomer(tsrc, tdst, cid);
                    move->act(*this);
                    actionsLog.push_back(move);
                }
                if (cmd == "close") {
                    int tid = stoi(str.substr(0, str.find(" ")));
                    str.erase(0, str.find(" ") + 1);
                    Close *close = new Close(tid);
                    close->act(*this);
                    actionsLog.push_back(close);
                }
                if (cmd == "workout_options") {
                    PrintWorkoutOptions *print = new PrintWorkoutOptions();
                    print->act(*this);
                    actionsLog.push_back(print);
                }
                if (cmd == "status") {
                    int tid = stoi(str.substr(0, str.find(" ")));
                    str.erase(0, str.find(" ") + 1);
                    PrintTrainerStatus *print = new PrintTrainerStatus(tid);
                    print->act(*this);
                    actionsLog.push_back(print);
                }
                if (cmd == "log") {
                    PrintActionsLog *print = new PrintActionsLog();
                    print->act(*this);
                    actionsLog.push_back(print);
                }
                if (cmd == "backup") {
                    BackupStudio *backup = new BackupStudio();
                    backup->act(*this);
                    actionsLog.push_back(backup);
                }
                if (cmd == "restore") {
                    RestoreStudio *restore = new RestoreStudio();
                    restore->act(*this);
                    actionsLog.push_back(restore);
                }
            }
            else {
                CloseAll closeall;
                closeall.act(*this);
                break;
        }
    }
}

int Studio::getNumOfTrainers() const {
    return trainers.size();
}

Trainer *Studio::getTrainer(int tid) {
    for (Trainer *t: trainers) {
        if (tid == t->getId()) {
            return t;
        }
    }
    return nullptr;
}

const std::vector<BaseAction *> &Studio::getActionsLog() const {
    return actionsLog;
}

std::vector<Workout> &Studio::getWorkoutOptions() {
    return workout_options;
}

std::vector<Trainer*> Studio::getTrainers() {
    return trainers;
}