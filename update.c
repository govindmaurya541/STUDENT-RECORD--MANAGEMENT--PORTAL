#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_SIZE 10  // Initial size for dynamic memory allocation

// Structure to hold student information
struct Student {
    int id;
    char name[50];
    int age;
    float gpa;
};

// Function prototypes
void addStudent(struct Student **students, int *count, int *size);
void viewStudents(struct Student *students, int count);
void updateStudent(struct Student *students, int count);
void deleteStudent(struct Student **students, int *count, int *size);
void displayMenu();
void saveToFile(struct Student *students, int count);
void loadFromFile(struct Student **students, int *count, int *size);
void sortStudentsByGPA(struct Student *students, int count);
int isValidIntInput(const char *prompt);
float isValidFloatInput(const char *prompt);

int main() {
    struct Student *students = NULL;
    int count = 0;
    int size = INITIAL_SIZE;
    students = (struct Student *)malloc(size * sizeof(struct Student));  // Allocate memory for students

    if (students == NULL) {
        printf("Memory allocation failed. Exiting.\n");
        return 1;
    }

    // Load existing data from file (if any)
    loadFromFile(&students, &count, &size);

    int choice;
    while (1) {
        displayMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addStudent(&students, &count, &size);
                break;
            case 2:
                viewStudents(students, count);
                break;
            case 3:
                updateStudent(students, count);
                break;
            case 4:
                deleteStudent(&students, &count, &size);
                break;
            case 5:
                saveToFile(students, count);  // Save data before exiting
                printf("Exiting the program.\n");
                free(students);  // Free allocated memory
                exit(0);
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }

    return 0;
}

void displayMenu() {
    printf("\n--- Student Record Management System ---\n");
    printf("1. Add Student\n");
    printf("2. View Students\n");
    printf("3. Update Student\n");
    printf("4. Delete Student\n");
    printf("5. Exit\n");
}

void addStudent(struct Student **students, int *count, int *size) {
    if (*count >= *size) {
        *size *= 2;  // Double the size of the array when it reaches its limit
        *students = realloc(*students, *size * sizeof(struct Student));

        if (*students == NULL) {
            printf("Memory allocation failed. Cannot add more students.\n");
            return;
        }
    }

    struct Student newStudent;
    printf("Enter student ID: ");
    newStudent.id = isValidIntInput("Enter a valid integer ID: ");
    printf("Enter student name: ");
    scanf(" %[^\n]", newStudent.name);  // Read string with spaces
    printf("Enter student age: ");
    newStudent.age = isValidIntInput("Enter a valid integer age: ");
    printf("Enter student GPA: ");
    newStudent.gpa = isValidFloatInput("Enter a valid GPA: ");

    (*students)[*count] = newStudent;
    (*count)++;
    printf("Student added successfully!\n");
}

void viewStudents(struct Student *students, int count) {
    if (count == 0) {
        printf("No student records available.\n");
        return;
    }

    printf("\n--- Student Records ---\n");
    for (int i = 0; i < count; i++) {
        printf("ID: %d, Name: %s, Age: %d, GPA: %.2f\n",
               students[i].id, students[i].name, students[i].age, students[i].gpa);
    }
}

void updateStudent(struct Student *students, int count) {
    int id, found = 0;
    printf("Enter student ID to update: ");
    id = isValidIntInput("Enter a valid ID to update: ");

    for (int i = 0; i < count; i++) {
        if (students[i].id == id) {
            found = 1;
            printf("Updating record for %s:\n", students[i].name);
            printf("Enter new name: ");
            scanf(" %[^\n]", students[i].name);
            printf("Enter new age: ");
            students[i].age = isValidIntInput("Enter a valid new age: ");
            printf("Enter new GPA: ");
            students[i].gpa = isValidFloatInput("Enter a valid new GPA: ");
            printf("Record updated successfully!\n");
            break;
        }
    }

    if (!found) {
        printf("Student with ID %d not found.\n", id);
    }
}

void deleteStudent(struct Student **students, int *count, int *size) {
    int id, found = 0;
    printf("Enter student ID to delete: ");
    id = isValidIntInput("Enter a valid ID to delete: ");

    for (int i = 0; i < *count; i++) {
        if ((*students)[i].id == id) {
            found = 1;
            for (int j = i; j < *count - 1; j++) {
                (*students)[j] = (*students)[j + 1];  // Shift records left
            }
            (*count)--;

            if (*count <= *size / 4) {
                *size /= 2;  // Shrink the array if it's too large
                *students = realloc(*students, *size * sizeof(struct Student));
            }

            printf("Record deleted successfully!\n");
            break;
        }
    }

    if (!found) {
        printf("Student with ID %d not found.\n", id);
    }
}

void saveToFile(struct Student *students, int count) {
    FILE *file = fopen("students.dat", "wb");
    if (file == NULL) {
        printf("Error saving data to file.\n");
        return;
    }
    fwrite(&count, sizeof(int), 1, file);  // Save the count of students
    fwrite(students, sizeof(struct Student), count, file);  // Save student records
    fclose(file);
}

void loadFromFile(struct Student **students, int *count, int *size) {
    FILE *file = fopen("students.dat", "rb");
    if (file == NULL) {
        return;  // No file exists, continue with an empty list
    }

    fread(count, sizeof(int), 1, file);
    *size = *count > INITIAL_SIZE ? *count : INITIAL_SIZE;
    *students = realloc(*students, *size * sizeof(struct Student));
    fread(*students, sizeof(struct Student), *count, file);
    fclose(file);
}

void sortStudentsByGPA(struct Student *students, int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (students[i].gpa < students[j].gpa) {
                struct Student temp = students[i];
                students[i] = students[j];
                students[j] = temp;
            }
        }
    }
}

int isValidIntInput(const char *prompt) {
    int value;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &value) == 1) {
            return value;
        } else {
            printf("Invalid input. Please enter an integer value.\n");
            while (getchar() != '\n'); // clear the buffer
        }
    }
}

float isValidFloatInput(const char *prompt) {
    float value;
    while (1) {
        printf("%s", prompt);
        if (scanf("%f", &value) == 1) {
            return value;
        } else {
            printf("Invalid input. Please enter a float value.\n");
            while (getchar() != '\n'); // clear the buffer
        }
    }
}
