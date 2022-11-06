//
// Created by spl211 on 11/11/2021.
//

#include "../include/Trainer.h"
#include <iostream>
#include <list>
#include <vector>


Trainer::Trainer(int id, int t_capacity) :
        t_id(id), salary(0), capacity(t_capacity), originalCapacity(t_capacity), open(false), customersList(std::vector<Customer *>()),
        orderList(std::vector<OrderPair>()){
};

Trainer::~Trainer() {
    for (Customer *c: customersList) {
        delete c;
    }
}

Trainer::Trainer(const Trainer &other) : t_id(other.t_id), salary(other.salary), capacity(other.capacity),
    originalCapacity(other.originalCapacity), open(other.open), customersList(), orderList(){
    for (OrderPair p: other.orderList) {
        int id = p.first;
        Workout w = p.second;
        OrderPair op(id,w);
        orderList.push_back(op);
    };
    for (Customer *c: other.customersList) {
        if (c->toString() == "swt") {
            SweatyCustomer *cus = new SweatyCustomer(c->getName(),c->getId());
            customersList.push_back(cus);
        }
        if (c->toString() == "chp") {
            CheapCustomer *cus = new CheapCustomer(c->getName(), c->getId());
            customersList.push_back(cus);
        }
        if (c->toString() == "mcl") {
            HeavyMuscleCustomer *cus = new HeavyMuscleCustomer(c->getName(), c->getId());
            customersList.push_back(cus);
        }
        if (c->toString() == "fbd") {
            FullBodyCustomer *cus = new FullBodyCustomer(c->getName(), c->getId());
            customersList.push_back(cus);
        }
    }
}

Trainer &Trainer::operator=(const Trainer &other) {
    if (this != &other) {
        clear();
        capacity = other.capacity;
        open = other.open;
        salary = other.salary;
        originalCapacity = other.originalCapacity;
        for (OrderPair p: other.orderList) {
            orderList.push_back(p);
        }
        for (Customer *c: other.customersList) {
            customersList.push_back(c);
        }
    }
    return *this;
}

Trainer::Trainer(Trainer &&other) : t_id(other.t_id), salary(other.salary), capacity(other.capacity),
    originalCapacity(other.originalCapacity), open(other.open), customersList(), orderList(){     // move constructor
    for (OrderPair p: other.orderList) {
        orderList.push_back(p);
    }
    for (Customer *c: other.customersList) {
        customersList.push_back(c); //check in compile
        c = nullptr;
    }
};

Trainer &Trainer::operator=(Trainer &&other) {
    if (this != &other) {
        clear();
        capacity = other.capacity;
        open = other.open;
        salary = other.salary;
        originalCapacity = other.originalCapacity;
        for (OrderPair p: other.orderList) {
            orderList.push_back(p);
        }
        for (Customer *c: other.customersList) {
            customersList.push_back(c);
            c = nullptr;
        }
    }
    return *this;
}

void Trainer::clear() {
    salary = 0;
    capacity = 0;
    open = false;
    orderList.clear();
    for (Customer *c: customersList) {
        delete &c;
    }
}

int Trainer::getCapacity() const {
    return this->capacity;
}

void Trainer::addCustomer(Customer *customer) {
    if(capacity > 0) {
        customersList.push_back(customer);
        capacity--;
    }

}

void Trainer::removeCustomer(int id) {
    int c = 0;
    std::vector<OrderPair> newOrderList;
    // remove from customer list
    for (Customer *i: customersList) {
        if (i->getId() == id) {
            customersList.erase(customersList.begin() + c);
            for(unsigned j = 0; j < orderList.size(); j++) {
                if(orderList[j].first != id)
                    newOrderList.push_back(OrderPair(orderList[j].first,orderList[j].second));
                else {
                    salary = salary - orderList[j].second.getPrice();
                    capacity++;
                }
            }
            orderList.clear();
            for(OrderPair p : newOrderList){
                orderList.push_back(p);
            }
            break;
        }
        c++;
    }

}

Customer *Trainer::getCustomer(int id) {
    for (Customer *i: customersList) {
        if (i->getId() == id) {
            return i;
        }
    }
    return nullptr;
}

std::vector<Customer *> &Trainer::getCustomers() {
    return customersList;

}

std::vector<OrderPair> &Trainer::getOrders() {
    return orderList;
}

void Trainer::order(const int customer_id, const std::vector<int> workout_ids,
                    const std::vector<Workout> &workout_options) {
    // do we need to check if the customer id is valid?
    for (unsigned i = 0; i < workout_ids.size(); i++) {
        for (unsigned j = 0; j < workout_options.size(); j++) {
            if (workout_ids[i] == workout_options[j].getId()) {
                orderList.push_back(OrderPair(customer_id, workout_options[j]));
                salary += workout_options[j].getPrice();       // update trainers salary
            }
        }
    }
}

void Trainer::openTrainer() {
    if (open)
        std::cout << "trainer is already open" << std::endl;
    else {
        open = true;
    }
}

void Trainer::closeTrainer() {
    capacity = originalCapacity;
    open = false;
    orderList.clear();
    for (Customer *c: customersList) {
        delete c;
        //c = nullptr;
    }
    customersList.clear();
}

int Trainer::getSalary() {
    return salary;
}

void Trainer::setSalary(int salary) {
    this->salary = salary;
}

int Trainer::getId() {
    return t_id;
}

bool Trainer::isOpen() {
    return open;
}