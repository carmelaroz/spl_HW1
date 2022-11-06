#ifndef TRAINER_H_
#define TRAINER_H_

#include <vector>
#include "Customer.h"


typedef std::pair<int, Workout> OrderPair;

class Trainer{
public:
    Trainer(int id, int t_capacity);
    virtual ~Trainer();
    Trainer(const Trainer& other);
    Trainer(Trainer&& other);
    Trainer& operator=(const Trainer& other);
    Trainer& operator=(Trainer&& other);
    void clear();
    int getCapacity() const;
    void addCustomer(Customer* customer);
    void removeCustomer(int id);
    Customer* getCustomer(int id);
    std::vector<Customer*>& getCustomers();
    std::vector<OrderPair>& getOrders();
    void order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout>& workout_options);
    void openTrainer();
    void closeTrainer();
    int getSalary();
    void setSalary(int salary);
    bool isOpen();
    int getId();
private:
    int t_id;
    int salary;
    int capacity;
    int originalCapacity;
    bool open;
    std::vector<Customer*> customersList;
    std::vector<OrderPair> orderList; //A list of pairs for each order for the trainer - (customer_id, Workout)
};


#endif