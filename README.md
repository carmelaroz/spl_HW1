## Objective ##
This project involves developing a C++ program to manage a fitness studio, implementing object-oriented principles and ensuring efficient memory handling. The program simulates studio operations by opening the studio, assigning customers to trainers, creating workout plans, and calculating trainer salaries. It uses various classes to represent the studio, trainers, workouts, and customers, each with specific attributes and methods. The system supports several customer workout strategies and user-initiated actions like opening sessions, placing orders, moving customers, and printing status reports.
The program utilizing classes, standard data structures, and C++ features such as the “Rule of 5”. Key goals include handling memory efficiently and avoiding leaks.


## Program Functionality ##
1.Studio Management:

• Simulates opening a fitness studio.

• Manages trainers and customers.

• Assigns customers to trainers and organizes workout plans.

• Calculates trainer's salaries based on the workouts conducted.

2.Classes and Structure:

• Studio: Manages a list of trainers and available workout plans.

• Trainer: Represents a trainer with a finite capacity for customers, status flags, and lists of workouts and customers.

• Workout: Represents a workout option with an ID, name, price, and type.

• Customer: Abstract class for different customer types, each with a unique workout ordering strategy.

• BaseAction: Abstract class for different action types, supporting logging and status updates.


3.Customer Types and Ordering Strategies:

• Sweaty Customer: Orders all cardio activities.

• Cheap Customer: Orders the cheapest workout.

• Heavy Muscle Customer: Orders anaerobic exercises from most to least expensive.

• Full Body Customer: Orders a mix of workout types based on specific criteria.


4.Actions:

• OpenTrainer: Opens a trainer’s session and assigns customers.

• Order: Initiates workout orders for a trainer’s session.

• MoveCustomer: Transfers a customer between trainers.

• Close: Closes a trainer’s session and displays the trainer’s salary.

• CloseAll: Closes all sessions and exits the program.

• PrintWorkoutOptions: Displays available workout options.

• PrintTrainerStatus: Shows the status of a specific trainer.

• PrintActionsLog: Lists all performed actions.

• BackupStudio: Saves the current studio status.

• RestoreStudio: Restores the studio from a backup.


5.Input File:

• Defines the number of trainers, their capacities, and available workout options.

• Each line specifies different parameters such as the number of trainers, capacities, and workout details.


6.Program Flow:

• Reads a configuration file to set up the studio.

• Waits for user commands to perform actions.

• Ends when the "closeall" command is issued.
