#ifndef HEADER_H
#define HEADER_H

#include <string>
#include <vector>

class Task
{
private:
    int taskID;
    std::string title;
    std::string deadline;
    bool isCompleted;
    bool isStarred;
    std::string notes;
    std::vector<Task> subtasks;

public:
    Task();
    Task(int id, const std::string &taskTitle, const std::string &due);

    int getTaskID() const;
    const std::string &getTitle() const;
    const std::string &getDeadline() const;
    bool getCompleted() const;
    bool getStarred() const;

    void markDone();
    void markUndone();
    void addNote(const std::string &noteText);
    void starTask();
    void addSubtask(const std::string &subTitle, const std::string &subDeadline);
    void reschedule(const std::string &newDeadline);
    void display(int indent = 0) const;
};

class TaskList
{
private:
    std::string listName;
    std::vector<Task> tasks;
    int nextID;

public:
    explicit TaskList(const std::string &name);

    void addTask(const std::string &title, const std::string &dueDateTime);
    bool deleteTask(int id);
    Task *findTask(int id);
    void displayAll() const;
    std::string getListName() const;
};

class InputHandler
{
private:
    std::string rawInput;

public:
    std::string getCommand();
    std::string getTaskInput();
    std::string getDate();
    int parseCommand(const std::string &command) const;
};

class Scheduler
{
private:
    std::vector<TaskList> lists;
    int currentListIndex;
    std::string sessionDate;
    InputHandler input;

    TaskList *getCurrentList();
    const TaskList *getCurrentList() const;
    int findListIndexByName(const std::string &name) const;

public:
    Scheduler();

    void setSessionDate(const std::string &dateTime);
    void createList(const std::string &name);
    bool switchList(const std::string &name);
    bool deleteList(const std::string &name);
    void run();
    void displayMenu() const;
};

#endif
