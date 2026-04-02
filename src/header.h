#ifndef HEADER_H
#define HEADER_H
#include <vector>
#include <string>
using namespace std;

class Task
{
    private:
    int taskID;
    string title;
    string deadline;
    bool isCompleted;
    bool isStarred;
    string notes;
    vector<Task> subtasks;

    public:
    void markDone();
    void markUndone();
    void addNote();
    void starTask();
    void addSubtask();
    void reschedule();
    void display();
};

class TaskList
{
    private:
    string listName;
    vector<Task> tasks;
    int nextID;

    public:
    void addTask();
    bool deleteTask();
    Task* findTask();
    void displayAll();
    string getListName();
};

class Scheduler
{
    private:
    vector<TaskList> lists;
    TaskList* currentList;
    string sessionDate;

    public:
    void createList();
    void switchList();
    bool deleteList();
    void run();
    void displayMenu();
};

class InputHandler
{
    public:
    string getCommand();
    string getTaskInput();
    string getDate();
    int parseCommand();
};




#endif 