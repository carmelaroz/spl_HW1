//
// Created by spl211 on 11/11/2021.
//

#include "Studio.h"
using namespace std;
BaseAction::BaseAction() : errorMsg(""), status(COMPLETED){};

BaseAction:: ~BaseAction(){};

string BaseAction::strActionStatus(ActionStatus status){
    if(status == COMPLETED)
        return "Completed";
    else
        return "Error: " + getErrorMsg();
};

ActionStatus BaseAction::getStatus() const {
        return status;       // check
}

void BaseAction::complete() {
    status = COMPLETED;
}
void BaseAction::error(std::string errorMsg) {
    status = ERROR;
    this->errorMsg = errorMsg;
}
string BaseAction::getErrorMsg() const {
    return errorMsg;
}

OpenTrainer::OpenTrainer(int id, std::vector<Customer *> &customersList) :
    trainerId(id), customers(customersList) {};

OpenTrainer::~OpenTrainer() {
    for(Customer *c : customers) {
        delete c;
        c = nullptr;
    }
    customers.clear();
}

void OpenTrainer::act(Studio &studio) {
    if(studio.getTrainer(trainerId)->isOpen() || studio.getTrainer(trainerId) == nullptr) {
        error("Workout session does not exist or is already open.");
        cout << "Error: " << getErrorMsg() << endl;
    }
    else {
        Trainer &trainer = *studio.getTrainer(trainerId);
        trainer.openTrainer();
        // add the customers to trainer's customers list (until the trainer's capacity)
        for(Customer* c : customers) {
            if(studio.getTrainer(trainerId)->getCapacity() > 0) {
                if (c->toString() == "swt") {
                    SweatyCustomer *cus = new SweatyCustomer(c->getName(), c->getId());
                    trainer.getCustomers().push_back(cus);
                }
                if (c->toString() == "chp") {
                    CheapCustomer *cus = new CheapCustomer(c->getName(), c->getId());
                    trainer.getCustomers().push_back(cus);
                }
                if (c->toString() == "mcl") {
                    HeavyMuscleCustomer *cus = new HeavyMuscleCustomer(c->getName(), c->getId());
                    trainer.getCustomers().push_back(cus);
                }
                if (c->toString() == "fbd") {
                    FullBodyCustomer *cus = new FullBodyCustomer(c->getName(), c->getId());
                    trainer.getCustomers().push_back(cus);
                }
            }
            complete();
        }
    }

}
string OpenTrainer::toString() const {
    string ans = "open " + to_string(trainerId) + " ";
    for(Customer* c : customers) {
        ans +=  c->getName() + "," + c->toString() + " ";
    }
    return ans;
}
OpenTrainer::OpenTrainer(const OpenTrainer &other) : trainerId(other.trainerId),customers(other.customers){}
OpenTrainer* OpenTrainer:: clone() const{
    vector<Customer*> list;
    for(Customer* c : customers){
        if (c->toString() == "swt") {
            SweatyCustomer* cus = new SweatyCustomer(c->getName(), c->getId());
            list.push_back(cus);
        }
        if (c->toString() == "chp") {
            CheapCustomer* cus = new CheapCustomer(c->getName(), c->getId());
            list.push_back(cus);
        }
        if (c->toString() == "mcl") {
            HeavyMuscleCustomer* cus = new HeavyMuscleCustomer(c->getName(), c->getId());
            list.push_back(cus);
        }
        if (c->toString() == "fbd") {
            FullBodyCustomer* cus = new FullBodyCustomer(c->getName(), c->getId());
            list.push_back(cus);
        }
    }
    OpenTrainer* open = new OpenTrainer(trainerId, list);
    return open;
}

Order::Order(int id) : trainerId(id) {};

void Order::act(Studio &studio) {
    if(!studio.getTrainer(trainerId)->isOpen() || studio.getTrainer(trainerId) == nullptr) {
        error("Trainer does not exist or is not open.");
        cout << "Error: " << getErrorMsg() << endl;
    }
    else {
        Trainer* t = studio.getTrainer(trainerId);
        vector<int> workoutIds;
        vector<Workout> v = studio.getWorkoutOptions();
        for(Customer *c : studio.getTrainer(trainerId)->getCustomers()) {
            workoutIds = c->order(studio.getWorkoutOptions());
            t->order(c->getId(), workoutIds, v);
        }
        // print :
        for(OrderPair p : studio.getTrainer(trainerId)->getOrders()) {
            for(Customer *c : studio.getTrainer(trainerId)->getCustomers()) {
                if(p.first == c->getId())
                    cout << c->getName() << " Is Doing " << p.second.getName() << endl;
            }
        }
    }

}
string Order::toString() const {
    return "Order " + to_string(trainerId) + " ";
}
Order* Order:: clone() const{
    int tid = trainerId;
    Order *order = new Order(tid);
    return order;
};

MoveCustomer::MoveCustomer(int src, int dst, int customerId) : srcTrainer(src), dstTrainer(dst), id(customerId) {};

void MoveCustomer::act(Studio &studio) {
    Trainer* tsrc = studio.getTrainer(srcTrainer);
    Trainer* tdst = studio.getTrainer(dstTrainer);
    if(!tsrc->isOpen() || !tdst->isOpen() || tsrc == nullptr || tdst == nullptr ||
                 tsrc->getCustomer(id) == nullptr || tdst->getCapacity() == 0) {
        error("Cannot move customer");
        cout << "Error: " << getErrorMsg() << endl;
    }
    else {
        tdst->addCustomer(tsrc->getCustomer(id));
        for(OrderPair p : tsrc->getOrders()) {
            if(p.first == id) {
                tdst->getOrders().push_back(OrderPair(id, p.second));   // add workout
                tdst->setSalary(tdst->getSalary() + p.second.getPrice());  // update salary
            }
        }
        tsrc->removeCustomer(id);
        if(tsrc->getCustomers().size() == 0)
            tsrc->closeTrainer();
    }
}
string MoveCustomer::toString() const {
    string ans = "move " + to_string(srcTrainer) + " " + to_string(dstTrainer) + " " + to_string(id) + " ";
    return ans;
}
MoveCustomer* MoveCustomer::clone() const{
    MoveCustomer *move = new MoveCustomer(this->srcTrainer, this->dstTrainer, this->id);
    return move;
};

Close::Close(int id) : trainerId(id) {};

void Close::act(Studio &studio) {
    if(!studio.getTrainer(trainerId)->isOpen() || studio.getTrainer(trainerId) == nullptr) {
        error("Trainer does not exist or is not open.");
        cout << "Error: " << getErrorMsg() << endl;
    }
    else {
        cout << "Trainer " << trainerId << " closed. Salary " << studio.getTrainer(trainerId)->getSalary() << " NIS" << endl;
        studio.getTrainer(trainerId)->closeTrainer();
    }
}
string Close::toString() const {
    return "close " + to_string(trainerId) + " ";
}
Close* Close:: clone() const{
    int tid = this->trainerId;
    Close *close = new Close(tid);
    return close;
};

CloseAll::CloseAll() {};

void CloseAll::act(Studio &studio) {
    vector<Trainer*> trainerList = studio.getTrainers();
    // closes all workouts and close trainer:
    for(Trainer* t : trainerList) {
        if(t->isOpen()) {
            t->closeTrainer();
            cout << "Trainer " << t->getId() << " closed. Salary " << t->getSalary() << "NIS" << endl;
        }
        studio.getTrainers().clear();
    }
}

string CloseAll::toString() const {
    return "closeall";
}

CloseAll* CloseAll:: clone() const{
    CloseAll* closeall = new CloseAll();
    return closeall;
};

PrintWorkoutOptions::PrintWorkoutOptions() {};

void PrintWorkoutOptions::act(Studio &studio) {
    for(Workout w : studio.getWorkoutOptions()) {
        cout << w.getName() << ", " << w.strType(w.getType()) << ", " << w.getPrice() << endl;
    }
}
string PrintWorkoutOptions::toString() const {
    return "workout_options ";
}

PrintWorkoutOptions* PrintWorkoutOptions:: clone() const{
    PrintWorkoutOptions *print = new PrintWorkoutOptions();
    return print;
};

PrintTrainerStatus::PrintTrainerStatus(int id) : trainerId(id) {};

void PrintTrainerStatus::act(Studio &studio) {
    Trainer* t = studio.getTrainer(trainerId);
    if(!t->isOpen())
      cout << "Trainer " << trainerId << " status: closed" << endl;
    else {
        cout << "Trainer " << trainerId << " status: open" << endl;
        cout << "Customers:" << endl;
        for(Customer* c : t->getCustomers()) {
            cout << c->getId() << " " << c->getName() << endl;
        }
        cout << "Orders:" << endl;
        for(OrderPair p : t->getOrders()) {
            cout << p.second.getName() << " " << p.second.getPrice() << "NIS " << p.first  << endl;
        }
        cout << "Current Trainer’s Salary: " << t->getSalary() << "NIS" << endl;
    }
}
string PrintTrainerStatus::toString() const {
    return "status " + to_string(trainerId) + " ";
}

PrintTrainerStatus* PrintTrainerStatus::clone() const{
    int tid = trainerId;
    PrintTrainerStatus *print = new PrintTrainerStatus(tid);
    return print;
};

PrintActionsLog::PrintActionsLog() {};

void PrintActionsLog::act(Studio &studio) {
    for(BaseAction* b : studio.getActionsLog()) {
        cout << b->toString()  << b->strActionStatus(b->getStatus()) << endl;
    }
}
string PrintActionsLog::toString() const {
    return "log ";
}

PrintActionsLog* PrintActionsLog:: clone() const{
    PrintActionsLog *print = new PrintActionsLog();
    return print;
};

BackupStudio::BackupStudio() {};

void BackupStudio::act(Studio &studio) {
    if(backup) {
        delete backup;
    }
    backup = new Studio(studio);
    this->complete();
}
std::string BackupStudio::toString() const {
    return "backup ";
}

BackupStudio* BackupStudio::clone() const{
    BackupStudio *backup = new BackupStudio();
    return backup;
};

RestoreStudio::RestoreStudio() {};

void RestoreStudio::act(Studio &studio) {
    if(backup == nullptr) {
        error("No backup available");
        cout << getErrorMsg() << endl;
    } else {
        studio = *backup;
        this->complete();
    }
}
std::string RestoreStudio:: toString() const {
    return "restore ";
}

RestoreStudio* RestoreStudio:: clone() const{
    RestoreStudio *restore = new RestoreStudio();
    return restore;
};