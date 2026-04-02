# Daily Task Scheduler

## Team Members

- Amogh Jambekar  
- Jincheng Xu  
- Trevor McLaughlin  

## Project Overview

The **Daily Task Scheduler** is a terminal-based C++ application inspired by Google Tasks. Users set a session date and time at launch, then manage tasks from the command line: add and delete tasks, mark them complete, reschedule deadlines, attach notes, star important items, organize work into multiple named lists (for example, Home and School), and attach nested subtasks. The interface is designed to feel like a structured, text-based task manager rather than a raw list of strings.

## Main Functionalities

| Area | Behavior |
|------|----------|
| **Launch & session** | Prompt for current date/time on startup to anchor the session. |
| **Tasks** | Create tasks with title and optional deadline; delete tasks by ID. |
| **Completion** | Mark tasks done or undone. |
| **Scheduling** | Reschedule an existing task to a new due date. |
| **Notes & priority** | Append notes to a task; star or unstar to mark importance. |
| **Lists** | Create, switch between, and delete named task lists. |
| **Subtasks** | Add nested subtasks under a parent task; display shows a small hierarchy. |
| **Display** | View tasks in the active list (or all lists) with status, stars, deadlines, notes, and subtasks. |

## OOP Design Summary

The system is split into four classes. **Task** is the domain object for one item (and recursively for subtasks). **TaskList** owns a collection of tasks and hands out IDs. **Scheduler** owns all lists, tracks the active list and session date, runs the main loop, and delegates work to the current list. **InputHandler** isolates reading and parsing user input so the scheduler stays focused on control flow. Together, this mirrors a common pattern: model (`Task`), collection (`TaskList`), application controller (`Scheduler`), and I/O boundary (`InputHandler`).

## Tools and Technologies

- **Language:** C++  
- **Build:** `g++` (compile `src` sources as your project grows)  
- **Version control:** Git / GitHub  

## Folder Structure

```
task-scheduler-eece2140/
├── README.md                 ← This file
├── src/
│   ├── header.h              Class declarations (Task, TaskList, Scheduler, InputHandler)
│   └── implement.cpp         Method definitions (Task implemented; others to be wired in)
└── pseudocode/
    ├── pseudocode.txt        Algorithmic description of classes
    └── pseudocode.pdf        Pseudocode document
```

## Project Goals

- Deliver a **fully functional** terminal task scheduler in C++.  
- Provide a **Google Tasks–like** experience using menus and clear terminal output.  
- Support **multiple lists**, **subtasks**, **deadlines**, **notes**, and **importance (star)** in one coherent design.  
- Keep the codebase **readable and maintainable** with clear class boundaries and documentation.  

## GitHub Repository Purpose

This repository hosts our **EECE 2140** (or equivalent course) final project: source code, design artifacts, and documentation in one place. GitHub gives us **version history**, **collaboration** across team members, and a **permanent link** for submission and demonstration. It also shows how the object-oriented structure in `header.h` maps to the behaviors described in our iteration and system-design documents.

---

## Class Reference (OOP Structure)

The table below summarizes how each class fits the system. Detailed descriptions follow.

| Class Name | Responsibility | Interacts With | Reason |
|------------|----------------|----------------|--------|
| **Task** | Stores one task’s data and behaviors (complete, note, star, subtasks, display) | **Task** (subtasks), **TaskList** | Represents each task and nested subtasks in the domain |
| **TaskList** | Stores tasks for one named list; assigns IDs; find/delete/display | **Task**, **Scheduler** | Groups tasks so users can separate Home, School, etc. |
| **Scheduler** | Owns lists, active list, session date; main loop and menu | **TaskList**, **InputHandler** | Coordinates the whole application |
| **InputHandler** | Reads commands and typed fields; maps input to action codes | **Scheduler** (caller) | Keeps input logic out of the scheduler’s control flow |

### Task

- **Responsibility:** Represent a single task (and, recursively, subtasks) with all user-visible state changes.  
- **Data stored:** `taskID`, `title`, `deadline`, `isCompleted`, `isStarred`, `notes`, and `subtasks` (`vector<Task>`).  
- **Methods:** `markDone()`, `markUndone()`, `addNote()`, `starTask()`, `addSubtask()`, `reschedule()`, `display()`.  
- **Interacts with:** Other **Task** instances as children in `subtasks`; **TaskList** creates tasks and calls these methods through pointers or references when modifying entries.  
- **Why necessary:** Without **Task**, there is no single place for completion, notes, stars, deadlines, and hierarchy; the rest of the app would be scattered conditionals over raw strings.

### TaskList

- **Responsibility:** Manage one named list: add tasks, delete by ID, find a task for updates, show all tasks.  
- **Data stored:** `listName`, `tasks` (`vector<Task>`), `nextID` for unique IDs within the list.  
- **Methods:** `addTask()`, `deleteTask()`, `findTask()`, `displayAll()`, `getListName()`.  
- **Interacts with:** **Task** (owns and mutates); **Scheduler** selects which list is active and calls these operations.  
- **Why necessary:** Separates “which bucket of tasks” from the global app; supports multiple lists without duplicating scheduler logic.

### Scheduler

- **Responsibility:** Application shell: session date/time, list CRUD, switching lists, main `run()` loop, `displayMenu()`.  
- **Data stored:** `lists` (`vector<TaskList>`), `currentList` (`TaskList*`), `sessionDate`.  
- **Methods:** `createList()`, `switchList()`, `deleteList()`, `run()`, `displayMenu()`.  
- **Interacts with:** **TaskList** for all task operations on the active list; **InputHandler** for commands and prompts.  
- **Why necessary:** One place to orchestrate menus, errors, and delegation so **TaskList** and **Task** stay focused on data and list-level rules.

### InputHandler

- **Responsibility:** Encapsulate console input: last command, task title, dates, and parsing strings into numeric action codes.  
- **Data stored:** (Design may include a buffer for raw input; the public API exposes `getCommand()`, `getTaskInput()`, `getDate()`, `parseCommand()`.)  
- **Methods:** `getCommand()`, `getTaskInput()`, `getDate()`, `parseCommand()`.  
- **Interacts with:** **Scheduler** calls these from `run()` before dispatching to lists/tasks.  
- **Why necessary:** Makes testing and future changes (e.g., different input source) easier and keeps **Scheduler** readable.

---

This structure matches our **system design overview** and **pseudocode**: input is validated and parsed, the scheduler routes to the active **TaskList**, lists own **Task** objects, and tasks can nest further **Task** rows for subtasks—so the OOP layout directly supports the full feature set of the Daily Task Scheduler.
