# Student Management System

A simple C console application for managing student records with file-based persistence. The program stores data in a local binary file, so student information remains available between runs.

## Features

- Add a new student record
- Display all saved student records
- Search for a student by ID
- Update an existing record
- Delete a record safely using a temporary file
- Prevent duplicate student IDs
- Validate GPA input and required fields

## How It Works

The application uses a `Student` structure with these fields:

- ID
- Name
- Course or domain
- GPA

Records are written to and read from `students.txt`. Even though the file has a `.txt` name, the program stores binary data in it for persistence.

## Files

- `student_management.c` - main source code
- `students.txt` - persistent data file created and updated by the program
- `students_tmp.txt` - temporary file used during delete operations

## Build and Run

### Using GCC

```bash
gcc student_management.c -o student_management
```

### Run

```bash
./student_management
```

On Windows, you can run the generated executable directly:

```bash
student_management.exe
```

## Usage

1. Run the program.
2. Choose an option from the menu.
3. Enter the requested student details.
4. The data is saved automatically in the local file.

## Notes

- Keep `students.txt` in the same folder as the executable.
- The file is managed automatically by the program.
- If `students.txt` is empty, the system will behave as if no records exist yet.

## Repository

https://github.com/kansaldhruv/CodeAlpha_Student-Management-System.git