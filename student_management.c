#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "students.txt"
#define TEMP_FILE_NAME "students_tmp.txt"

// Structure to define student records
typedef struct {
    int id;
    char name[50];
    char course[50];
    float gpa;
} Student;

// Function prototypes
void addStudent();
void displayStudents();
void searchStudent();
void updateStudent();
void deleteStudent();
int getIntInput(const char *prompt, int *value);
int getFloatInput(const char *prompt, float *value);
void getStringInput(const char *prompt, char *buffer, size_t size);
int studentIdExists(int id);

int main() {
    int choice;

    while (1) {
        printf("\n=========================================\n");
        printf("       STUDENT MANAGEMENT SYSTEM         \n");
        printf("=========================================\n");
        printf(" 1. Add Student Record\n");
        printf(" 2. Display All Student Records\n");
        printf(" 3. Search Student Record by ID\n");
        printf(" 4. Update Student Record\n");
        printf(" 5. Delete Student Record\n");
        printf(" 6. Exit\n");
        printf("-----------------------------------------\n");
        if (!getIntInput("Enter your choice (1-6): ", &choice)) {
            continue;
        }

        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6:
                printf("\nThank you for using the Student Management System. Goodbye!\n");
                return 0;
            default:
                printf("\n[!] Invalid choice. Please try again.\n");
        }
    }
}

// 1. ADD: Appends a student record to the file
void addStudent() {
    FILE *file = fopen(FILE_NAME, "ab");
    if (file == NULL) {
        printf("\n[!] Error opening data file.\n");
        return;
    }

    Student s;
    printf("\n--- Add Student ---\n");
    while (1) {
        if (!getIntInput("Enter Student ID: ", &s.id)) {
            fclose(file);
            return;
        }
        if (studentIdExists(s.id)) {
            printf("[!] ID already exists. Please enter a unique ID.\n");
            continue;
        }
        break;
    }

    getStringInput("Enter Student Name: ", s.name, sizeof(s.name));
    getStringInput("Enter Course/Domain: ", s.course, sizeof(s.course));

    while (1) {
        if (!getFloatInput("Enter GPA (0.0 to 10.0): ", &s.gpa)) {
            fclose(file);
            return;
        }
        if (s.gpa < 0.0f || s.gpa > 10.0f) {
            printf("[!] GPA must be between 0.0 and 10.0.\n");
            continue;
        }
        break;
    }

    if (fwrite(&s, sizeof(Student), 1, file) != 1) {
        printf("\n[!] Failed to save record.\n");
    } else {
        printf("\n[+] Student record added successfully.\n");
    }
    fclose(file);
}

// 2. DISPLAY: Reads and lists all records from the file
void displayStudents() {
    FILE *file = fopen(FILE_NAME, "rb");
    if (file == NULL) {
        printf("\n[!] No records found yet.\n");
        return;
    }

    Student s;
    int count = 0;
    printf("\n--- All Student Records ---\n");
    printf("\n%-10s %-20s %-20s %-5s\n", "ID", "Name", "Course", "GPA");
    printf("-------------------------------------------------------------\n");

    while (fread(&s, sizeof(Student), 1, file)) {
        printf("%-10d %-20s %-20s %-5.2f\n", s.id, s.name, s.course, s.gpa);
        count++;
    }

    if (count == 0) {
        printf("No records available.\n");
    } else {
        printf("-------------------------------------------------------------\n");
        printf("Total records: %d\n", count);
    }

    fclose(file);
}

// 3. SEARCH: Finds a student record by matching ID
void searchStudent() {
    FILE *file = fopen(FILE_NAME, "rb");
    if (file == NULL) {
        printf("\n[!] No records found.\n");
        return;
    }

    int searchId, found = 0;
    printf("\n--- Search Student ---\n");
    if (!getIntInput("Enter Student ID to search: ", &searchId)) {
        fclose(file);
        return;
    }

    Student s;
    while (fread(&s, sizeof(Student), 1, file)) {
        if (s.id == searchId) {
            printf("\n[ Record Found ]\n");
            printf("ID: %d\nName: %s\nCourse: %s\nGPA: %.2f\n", s.id, s.name, s.course, s.gpa);
            found = 1;
            break;
        }
    }
    if (!found) printf("\n[!] Record with ID %d not found.\n", searchId);
    fclose(file);
}

// 4. UPDATE: Modifies details of a matching ID
void updateStudent() {
    FILE *file = fopen(FILE_NAME, "rb+"); // Open for read/write binary
    if (file == NULL) {
        printf("\n[!] No records found.\n");
        return;
    }

    int targetId, found = 0;
    printf("\n--- Update Student ---\n");
    if (!getIntInput("Enter Student ID to update: ", &targetId)) {
        fclose(file);
        return;
    }

    Student s;
    while (fread(&s, sizeof(Student), 1, file)) {
        if (s.id == targetId) {
            found = 1;
            getStringInput("Enter New Name: ", s.name, sizeof(s.name));
            getStringInput("Enter New Course: ", s.course, sizeof(s.course));
            while (1) {
                if (!getFloatInput("Enter New GPA (0.0 to 10.0): ", &s.gpa)) {
                    fclose(file);
                    return;
                }
                if (s.gpa < 0.0f || s.gpa > 10.0f) {
                    printf("[!] GPA must be between 0.0 and 10.0.\n");
                    continue;
                }
                break;
            }

            // Move file pointer back to rewrite this specific record
            fseek(file, -sizeof(Student), SEEK_CUR);
            if (fwrite(&s, sizeof(Student), 1, file) != 1) {
                printf("\n[!] Failed to update the record.\n");
            } else {
                printf("\n[+] Record updated successfully.\n");
            }
            break;
        }
    }
    if (!found) printf("\n[!] Record with ID %d not found.\n", targetId);
    fclose(file);
}

// 5. DELETE: Removes a student record using a temporary file transfer
void deleteStudent() {
    FILE *file = fopen(FILE_NAME, "rb");
    if (file == NULL) {
        printf("\n[!] No records found.\n");
        return;
    }

    FILE *tempFile = fopen(TEMP_FILE_NAME, "wb");
    if (tempFile == NULL) {
        printf("\n[!] Error creating temporary file.\n");
        fclose(file);
        return;
    }

    int deleteId, found = 0;
    printf("\n--- Delete Student ---\n");
    if (!getIntInput("Enter Student ID to delete: ", &deleteId)) {
        fclose(file);
        fclose(tempFile);
        remove(TEMP_FILE_NAME);
        return;
    }

    Student s;
    while (fread(&s, sizeof(Student), 1, file)) {
        if (s.id == deleteId) {
            found = 1;
        } else {
            fwrite(&s, sizeof(Student), 1, tempFile);
        }
    }

    fclose(file);
    fclose(tempFile);

    if (!found) {
        remove(TEMP_FILE_NAME);
        printf("\n[!] Record with ID %d not found.\n", deleteId);
        return;
    }

    if (remove(FILE_NAME) != 0) {
        printf("\n[!] Could not update data file.\n");
        remove(TEMP_FILE_NAME);
        return;
    }

    if (rename(TEMP_FILE_NAME, FILE_NAME) != 0) {
        printf("\n[!] Could not finalize delete operation.\n");
        return;
    }

    printf("\n[-] Record deleted successfully.\n");
}

int getIntInput(const char *prompt, int *value) {
    char line[128];
    char *endPtr;
    long num;

    printf("%s", prompt);
    if (fgets(line, sizeof(line), stdin) == NULL) {
        printf("\n[!] Input error.\n");
        return 0;
    }

    num = strtol(line, &endPtr, 10);
    if (endPtr == line) {
        printf("[!] Invalid input. Please enter a number.\n");
        return 0;
    }

    while (*endPtr == ' ' || *endPtr == '\t') {
        endPtr++;
    }

    if (*endPtr != '\n' && *endPtr != '\0') {
        printf("[!] Invalid characters found. Please enter a valid number.\n");
        return 0;
    }

    *value = (int)num;
    return 1;
}

int getFloatInput(const char *prompt, float *value) {
    char line[128];
    char *endPtr;
    float num;

    printf("%s", prompt);
    if (fgets(line, sizeof(line), stdin) == NULL) {
        printf("\n[!] Input error.\n");
        return 0;
    }

    num = strtof(line, &endPtr);
    if (endPtr == line) {
        printf("[!] Invalid input. Please enter a decimal number.\n");
        return 0;
    }

    while (*endPtr == ' ' || *endPtr == '\t') {
        endPtr++;
    }

    if (*endPtr != '\n' && *endPtr != '\0') {
        printf("[!] Invalid characters found. Please enter a valid decimal number.\n");
        return 0;
    }

    *value = num;
    return 1;
}

void getStringInput(const char *prompt, char *buffer, size_t size) {
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, size, stdin) == NULL) {
            buffer[0] = '\0';
            return;
        }

        buffer[strcspn(buffer, "\n")] = '\0';

        if (strlen(buffer) == 0) {
            printf("[!] Field cannot be empty. Please try again.\n");
            continue;
        }

        return;
    }
}

int studentIdExists(int id) {
    FILE *file = fopen(FILE_NAME, "rb");
    Student s;

    if (file == NULL) {
        return 0;
    }

    while (fread(&s, sizeof(Student), 1, file)) {
        if (s.id == id) {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}
