#include <iostream>
#include <vector>
#include <string>
#include "header.h"

using namespace std;

void Task::markDone()
{
    isCompleted = true;
    cout << "Task: " << title << " marked as done." << endl;
}

void Task::markUndone()
{
    isCompleted = false;
    cout << "Task \"" << title << "\" marked as not done.\n";
}

void Task::addNote()
{
    cout << "Enter note: ";
    string input;
    getline(cin, input);
    if (!notes.empty())
        notes += "\n";
    notes += input;
    cout << "Note added.\n";
}

void Task::starTask()
{
    isStarred = !isStarred;
    cout << "Task \"" << title << "\" " << (isStarred ? "starred" : "unstarred") << ".\n";
}

void Task::addSubtask()
{
    cout << "Enter subtask title: ";
    string subTitle;
    getline(cin, subTitle);
    cout << "Enter subtask deadline (YYYY-MM-DD): ";
    string subDeadline;
    getline(cin, subDeadline);

    Task sub;
    sub.taskID = static_cast<int>(subtasks.size()) + 1;
    sub.title = subTitle;
    sub.deadline = subDeadline;
    sub.isCompleted = false;
    sub.isStarred = false;
    subtasks.push_back(sub);
    cout << "Subtask added.\n";
}

void Task::reschedule()
{
    cout << "Enter new deadline (YYYY-MM-DD): ";
    string newDeadline;
    getline(cin, newDeadline);
    deadline = newDeadline;
    cout << "Task \"" << title << "\" rescheduled to " << deadline << ".\n";
}

void Task::display()
{
    cout << "[" << title << " Task ID: " << taskID << "] "
         << (isStarred ? "* " : "  ")
         << (isCompleted ? "[DONE] " : "[    ] ")
         << title
         << " (Due: " << deadline << ")\n";
    if (!notes.empty())
        cout << "     Notes: " << notes << "\n";
    if (!subtasks.empty())
    {
        cout << "     Subtasks:\n";
        for (Task& sub : subtasks)
        {
            cout << "       ";
            sub.display();
        }
    }
}
