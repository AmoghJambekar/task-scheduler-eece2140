#include "header.h"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <sstream>

using std::cin;
using std::cout;
using std::endl;
using std::string;

namespace
{
string trim(const string &s)
{
    size_t start = 0;
    while (start < s.size() && std::isspace(static_cast<unsigned char>(s[start])))
        ++start;
    size_t end = s.size();
    while (end > start && std::isspace(static_cast<unsigned char>(s[end - 1])))
        --end;
    return s.substr(start, end - start);
}

string toLower(string s)
{
    for (char &c : s)
        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    return s;
}
} // namespace

// ---------------- Task ----------------

Task::Task() : taskID(0), isCompleted(false), isStarred(false) {}

Task::Task(int id, const string &taskTitle, const string &due)
    : taskID(id), title(taskTitle), deadline(due), isCompleted(false), isStarred(false)
{
}

int Task::getTaskID() const
{
    return taskID;
}

const string &Task::getTitle() const
{
    return title;
}

const string &Task::getDeadline() const
{
    return deadline;
}

bool Task::getCompleted() const
{
    return isCompleted;
}

bool Task::getStarred() const
{
    return isStarred;
}

void Task::markDone()
{
    isCompleted = true;
    cout << "Task \"" << title << "\" marked as done." << endl;
}

void Task::markUndone()
{
    isCompleted = false;
    cout << "Task \"" << title << "\" marked as not done." << endl;
}

void Task::addNote(const string &noteText)
{
    if (!notes.empty())
        notes += "\n";
    notes += noteText;
    cout << "Note added to task \"" << title << "\"." << endl;
}

void Task::starTask()
{
    isStarred = !isStarred;
    cout << "Task \"" << title << "\" " << (isStarred ? "starred (priority)." : "unstarred.")
         << endl;
}

void Task::addSubtask(const string &subTitle, const std::string &subDeadline)
{
    const int subId = static_cast<int>(subtasks.size()) + 1;
    subtasks.emplace_back(subId, subTitle, subDeadline);
    cout << "Subtask \"" << subTitle << "\" added under \"" << title << "\"." << endl;
}

void Task::reschedule(const string &newDeadline)
{
    deadline = newDeadline;
    cout << "Task \"" << title << "\" rescheduled. New due: " << deadline << endl;
}

void Task::display(int indent) const
{
    string pad(static_cast<size_t>(indent) * 2, ' ');
    cout << pad << "[" << taskID << "] " << (isStarred ? "* " : "  ")
         << (isCompleted ? "[DONE] " : "[    ] ") << title;
    if (!deadline.empty())
        cout << "  | Due: " << deadline;
    cout << endl;
    if (!notes.empty())
    {
        std::istringstream ns(notes);
        string line;
        while (std::getline(ns, line))
            cout << pad << "     Note: " << line << endl;
    }
    if (!subtasks.empty())
    {
        cout << pad << "     Subtasks:" << endl;
        for (const Task &sub : subtasks)
            sub.display(indent + 2);
    }
}

// ---------------- TaskList ----------------

TaskList::TaskList(const string &name) : listName(name), nextID(1) {}

void TaskList::addTask(const string &title, const string &dueDateTime)
{
    tasks.emplace_back(nextID, title, dueDateTime);
    ++nextID;
    cout << "Task added to list \"" << listName << "\"." << endl;
}

bool TaskList::deleteTask(int id)
{
    for (auto it = tasks.begin(); it != tasks.end(); ++it)
    {
        if (it->getTaskID() == id)
        {
            tasks.erase(it);
            cout << "Task " << id << " removed from \"" << listName << "\"." << endl;
            return true;
        }
    }
    return false;
}

Task *TaskList::findTask(int id)
{
    for (auto &t : tasks)
    {
        if (t.getTaskID() == id)
            return &t;
    }
    return nullptr;
}

void TaskList::displayAll() const
{
    cout << "\n--- List: " << listName << " ---" << endl;
    if (tasks.empty())
    {
        cout << "(no tasks)" << endl;
        return;
    }
    std::vector<size_t> order(tasks.size());
    for (size_t i = 0; i < tasks.size(); ++i)
        order[i] = i;
    std::sort(order.begin(), order.end(), [this](size_t a, size_t b) {
        if (tasks[a].getStarred() != tasks[b].getStarred())
            return tasks[a].getStarred() > tasks[b].getStarred();
        return tasks[a].getDeadline() < tasks[b].getDeadline();
    });
    for (size_t idx : order)
        tasks[idx].display(0);
    cout << endl;
}

string TaskList::getListName() const
{
    return listName;
}

// ---------------- InputHandler ----------------

string InputHandler::getCommand()
{
    cout << "\n> ";
    std::getline(cin, rawInput);
    return rawInput;
}

string InputHandler::getTaskInput()
{
    cout << "Enter task title: ";
    string line;
    std::getline(cin, line);
    return trim(line);
}

string InputHandler::getDate()
{
    cout << "Enter due date/time (YYYY-MM-DD or YYYY-MM-DD HH:MM, or leave blank for none): ";
    string line;
    std::getline(cin, line);
    return trim(line);
}

int InputHandler::parseCommand(const string &command) const
{
    string t = trim(command);
    if (t.empty())
        return -1;

    bool allDigits = true;
    for (char c : t)
    {
        if (!std::isdigit(static_cast<unsigned char>(c)))
        {
            allDigits = false;
            break;
        }
    }
    if (allDigits)
    {
        try
        {
            int n = std::stoi(t);
            if (n >= 0 && n <= 12)
                return n;
        }
        catch (...)
        {
        }
    }

    string w = toLower(t);
    if (w == "quit" || w == "exit" || w == "q")
        return 0;
    if (w == "add" || w == "add task")
        return 1;
    if (w == "done" || w == "mark done" || w == "complete")
        return 2;
    if (w == "undone" || w == "mark undone" || w == "not done")
        return 3;
    if (w == "reschedule" || w == "due")
        return 4;
    if (w == "note" || w == "notes")
        return 5;
    if (w == "star" || w == "priority")
        return 6;
    if (w == "new list" || w == "create list" || w == "createlist")
        return 7;
    if (w == "switch" || w == "switch list")
        return 8;
    if (w == "delete list" || w == "removelist")
        return 9;
    if (w == "subtask" || w == "add subtask")
        return 10;
    if (w == "view" || w == "display" || w == "show" || w == "list")
        return 11;
    if (w == "delete" || w == "delete task" || w == "remove")
        return 12;
    if (w == "menu" || w == "help" || w == "?")
        return 99;

    return -1;
}

// ---------------- Scheduler ----------------

Scheduler::Scheduler() : currentListIndex(-1) {}

void Scheduler::setSessionDate(const string &dateTime)
{
    sessionDate = dateTime;
    cout << "Session started at: " << sessionDate << endl;
}

TaskList *Scheduler::getCurrentList()
{
    if (currentListIndex < 0 || currentListIndex >= static_cast<int>(lists.size()))
        return nullptr;
    return &lists[static_cast<size_t>(currentListIndex)];
}

const TaskList *Scheduler::getCurrentList() const
{
    if (currentListIndex < 0 || currentListIndex >= static_cast<int>(lists.size()))
        return nullptr;
    return &lists[static_cast<size_t>(currentListIndex)];
}

int Scheduler::findListIndexByName(const string &name) const
{
    string want = toLower(trim(name));
    for (size_t i = 0; i < lists.size(); ++i)
    {
        if (toLower(lists[i].getListName()) == want)
            return static_cast<int>(i);
    }
    return -1;
}

void Scheduler::createList(const string &name)
{
    string n = trim(name);
    if (n.empty())
    {
        cout << "List name cannot be empty." << endl;
        return;
    }
    if (findListIndexByName(n) >= 0)
    {
        cout << "A list with that name already exists." << endl;
        return;
    }
    lists.emplace_back(n);
    currentListIndex = static_cast<int>(lists.size()) - 1;
    cout << "List \"" << n << "\" created. It is now the active list." << endl;
}

bool Scheduler::switchList(const string &name)
{
    int idx = findListIndexByName(name);
    if (idx < 0)
    {
        cout << "No list named \"" << name << "\"." << endl;
        return false;
    }
    currentListIndex = idx;
    cout << "Active list is now \"" << lists[static_cast<size_t>(idx)].getListName() << "\"."
         << endl;
    return true;
}

bool Scheduler::deleteList(const string &name)
{
    int idx = findListIndexByName(name);
    if (idx < 0)
    {
        cout << "No list named \"" << name << "\"." << endl;
        return false;
    }
    string deletedName = lists[static_cast<size_t>(idx)].getListName();
    lists.erase(lists.begin() + idx);
    if (lists.empty())
    {
        currentListIndex = -1;
    }
    else if (currentListIndex == idx)
    {
        currentListIndex = std::min(idx, static_cast<int>(lists.size()) - 1);
    }
    else if (currentListIndex > idx)
    {
        --currentListIndex;
    }
    cout << "List \"" << deletedName << "\" deleted." << endl;
    return true;
}

void Scheduler::displayMenu() const
{
    cout << "\n========== Daily Task Scheduler ==========\n";
    cout << " Session: " << sessionDate << "\n";
    if (const TaskList *cl = getCurrentList())
        cout << " Active list: " << cl->getListName() << "\n";
    else
        cout << " Active list: (none — create or switch to a list)\n";
    cout << "------------------------------------------\n";
    cout << " 1) Add task (title, optional due, optional target list)\n";
    cout << " 2) Mark task done\n";
    cout << " 3) Mark task not done\n";
    cout << " 4) Reschedule task\n";
    cout << " 5) Add note to task\n";
    cout << " 6) Star / unstar task (priority)\n";
    cout << " 7) Create list\n";
    cout << " 8) Switch list\n";
    cout << " 9) Delete list\n";
    cout << "10) Add subtask to task\n";
    cout << "11) View tasks (current list or all)\n";
    cout << "12) Delete task\n";
    cout << " 0) Quit\n";
    cout << "------------------------------------------\n";
    cout << "Tip: type the number or a keyword (e.g. add, view, quit).\n";
}

void Scheduler::run()
{
    cout << "Enter current date/time for this session (e.g. 2026-04-06 14:30): ";
    string sessionLine;
    std::getline(cin, sessionLine);
    setSessionDate(trim(sessionLine));

    while (true)
    {
        displayMenu();
        string line = input.getCommand();
        int code = input.parseCommand(line);

        if (code == 0)
        {
            cout << "Goodbye." << endl;
            break;
        }
        if (code == 99)
        {
            continue;
        }
        if (code < 0)
        {
            cout << "Unknown command. Try a number from the menu or \"help\"." << endl;
            continue;
        }

        switch (code)
        {
        case 1:
        {
            string title = input.getTaskInput();
            if (title.empty())
            {
                cout << "Title cannot be empty." << endl;
                break;
            }
            string due = input.getDate();
            cout << "Add to list (name, or blank for active list): ";
            string listNameIn;
            std::getline(cin, listNameIn);
            listNameIn = trim(listNameIn);
            TaskList *target = nullptr;
            if (listNameIn.empty())
                target = getCurrentList();
            else
            {
                int li = findListIndexByName(listNameIn);
                if (li < 0)
                {
                    cout << "List not found. Create it first (option 7) or check the name."
                         << endl;
                    break;
                }
                target = &lists[static_cast<size_t>(li)];
            }
            if (!target)
            {
                cout << "No active list. Create a list (7) or specify an existing list name."
                     << endl;
                break;
            }
            target->addTask(title, due);
            break;
        }
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 10:
        case 12:
        {
            TaskList *cl = getCurrentList();
            if (!cl)
            {
                cout << "No active list. Create or switch to a list first." << endl;
                break;
            }
            cout << "Enter task ID: ";
            string idStr;
            std::getline(cin, idStr);
            int tid = 0;
            try
            {
                tid = std::stoi(trim(idStr));
            }
            catch (...)
            {
                cout << "Invalid task ID." << endl;
                break;
            }
            Task *tp = cl->findTask(tid);
            if (!tp)
            {
                cout << "Task " << tid << " not found in \"" << cl->getListName() << "\"."
                     << endl;
                break;
            }
            if (code == 2)
                tp->markDone();
            else if (code == 3)
                tp->markUndone();
            else if (code == 4)
            {
                string nd = input.getDate();
                tp->reschedule(nd);
            }
            else if (code == 5)
            {
                cout << "Enter note text: ";
                string note;
                std::getline(cin, note);
                tp->addNote(note);
            }
            else if (code == 6)
                tp->starTask();
            else if (code == 10)
            {
                string st = input.getTaskInput();
                if (st.empty())
                {
                    cout << "Subtask title cannot be empty." << endl;
                    break;
                }
                string sd = input.getDate();
                tp->addSubtask(st, sd);
            }
            else if (code == 12)
            {
                if (cl->deleteTask(tid))
                { /* message inside */
                }
                else
                    cout << "Could not delete task." << endl;
            }
            break;
        }
        case 7:
        {
            cout << "New list name: ";
            string nm;
            std::getline(cin, nm);
            createList(nm);
            break;
        }
        case 8:
        {
            cout << "List name to switch to: ";
            string nm;
            std::getline(cin, nm);
            switchList(nm);
            break;
        }
        case 9:
        {
            cout << "List name to delete: ";
            string nm;
            std::getline(cin, nm);
            deleteList(nm);
            break;
        }
        case 11:
        {
            cout << "View [current] list or [all] lists? ";
            string mode;
            std::getline(cin, mode);
            mode = toLower(trim(mode));
            if (mode == "all" || mode == "a")
            {
                if (lists.empty())
                    cout << "No lists yet." << endl;
                for (const TaskList &L : lists)
                    L.displayAll();
            }
            else
            {
                const TaskList *cl = getCurrentList();
                if (!cl)
                    cout << "No active list." << endl;
                else
                    cl->displayAll();
            }
            break;
        }
        default:
            cout << "Command not implemented." << endl;
            break;
        }
    }
}
