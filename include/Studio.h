#ifndef STUDIO_H_
#define STUDIO_H_W

#include <vector>
#include <string>
#include "Workout.h"
#include "Trainer.h"
#include "Action.h"

class BaseAction;

class Studio{
public:
	Studio();
    ~Studio();
    Studio(const Studio &other);
    Studio(const Studio &&other);
    Studio &operator=(const Studio &other);
    Studio &operator=(Studio &&other);
    void clear();
    Studio(const std::string &configFilePath);
    void start();
    int getNumOfTrainers() const;
    Trainer* getTrainer(int tid);
	const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    std::vector<Workout>& getWorkoutOptions();
    std::vector<Trainer*> getTrainers();

private:
    bool open;
    std::vector<Trainer*> trainers;
    std::vector<Workout> workout_options;
    std::vector<BaseAction*> actionsLog;
};

#endif