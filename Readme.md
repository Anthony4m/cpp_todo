# Tasks Management System

Tasks Management System is a simple command-line application for managing tasks. It allows users to create, update, delete, and view tasks, providing a basic interface for task organization.

## Features

- **Task Creation**: Easily create new tasks with a title, description, and status.
- **Task Deletion**: Remove tasks by title or ID.
- **Task Listing**: View a list of all tasks along with their details.
- **Task Status Update**: Change the status of a task (TODO, IN_PROGRESS, DONE).

## Requirements

- C++ Compiler
- CMake
- cereal (C++11 library for serialization)

## Installation

1. Clone the repository:

    ```bash
    git clone https://github.com/your-username/tasks-management.git
    ```

2. Navigate to the project directory:

    ```bash
    cd tasks-management
    ```

3. Build the project:

    ```bash
    cmake --build build
    ```

4. Run the application:

    ```bash
    ./tasks-management
    ```

## Usage

### Adding a Task:

```bash
./tasks-management add "Task Title" "Task Description" TODO