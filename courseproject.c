#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the Patient structure to store patient information
struct Patient {
    char name[50];
    int age;
    char gender[10];
    char medical_history[100];
    char diagnosis[100];
    char prescription[100];
    struct Patient* left;
    struct Patient* right;
};

struct AgeNode {
    int age;
    struct Patient* patients;  // Linked list of patients with the same age
    struct AgeNode* left;
    struct AgeNode* right;
};

// Function prototypes
struct Patient* create_patient(const char* name, int age, const char* gender, const char* medical_history, const char* diagnosis, const char* prescription);
struct Patient* add_patient(struct Patient* root, struct Patient* new_patient);
struct Patient* search_patient(struct Patient* root, const char* name);
struct Patient* find_min(struct Patient* root);
struct Patient* delete_patient_record(struct Patient* root, const char* name);
void display_all_records(struct Patient* root);
void save_records_to_file(struct Patient* root, const char* filename);
struct Patient* load_records_from_file(const char* filename);

// Function prototypes for age-based trees
struct AgeNode* create_age_node(int age);
struct AgeNode* add_patient_by_age(struct AgeNode* root, struct Patient* new_patient);
void search_patients_by_age_range(struct AgeNode* root, int min_age, int max_age);

// Function prototype for save_patient_records
void save_patient_records(struct Patient* patient, FILE* file);

// Function prototype for load_records_from_file_internal
struct Patient* load_records_from_file_internal(FILE* file);

// Add patient by age to the age-based tree
struct AgeNode* add_patient_by_age(struct AgeNode* root, struct Patient* new_patient) {
    if (root == NULL) {
        root = create_age_node(new_patient->age);
        root->patients = new_patient;
        return root;
    }

    if (new_patient->age < root->age) {
        root->left = add_patient_by_age(root->left, new_patient);
    } else if (new_patient->age > root->age) {
        root->right = add_patient_by_age(root->right, new_patient);
    } else {
        // Patients with the same age are stored in a linked list
        new_patient->right = root->patients;
        root->patients = new_patient;
    }

    return root;
}

// Function to print patient details
void print_patient_details(struct Patient* patient) {
    printf("Name: %s\n", patient->name);
    printf("Age: %d\n", patient->age);
    printf("Gender: %s\n", patient->gender);
    printf("Medical History: %s\n", patient->medical_history);
    printf("Diagnosis: %s\n", patient->diagnosis);
    printf("Prescription: %s\n", patient->prescription);
    printf("-----------------------------\n");
}

// Search for patients by age range in the age-based tree
void search_patients_by_age_range(struct AgeNode* root, int min_age, int max_age) {
    if (root == NULL) {
        return;
    }

    // Recursively search in the left subtree if needed
    if (root->age > min_age) {
        search_patients_by_age_range(root->left, min_age, max_age);
    }

    // Print patient details for each patient within the specified range
    struct Patient* current_patient = root->patients;
    while (current_patient != NULL) {
        if (current_patient->age >= min_age && current_patient->age <= max_age) {
            print_patient_details(current_patient);
        }
        current_patient = current_patient->right;
    }

    // Recursively search in the right subtree if needed
    if (root->age < max_age) {
        search_patients_by_age_range(root->right, min_age, max_age);
    }
}

// Create a new AgeNode
struct AgeNode* create_age_node(int age) {
    struct AgeNode* new_age_node = (struct AgeNode*)malloc(sizeof(struct AgeNode));
    new_age_node->age = age;
    new_age_node->patients = NULL;
    new_age_node->left = NULL;
    new_age_node->right = NULL;
    return new_age_node;
}

// Function to create a new patient node
struct Patient* create_patient(const char* name, int age, const char* gender, const char* medical_history, const char* diagnosis, const char* prescription) {
    struct Patient* new_patient = (struct Patient*)malloc(sizeof(struct Patient));
    strcpy(new_patient->name, name);
    new_patient->age = age;
    strcpy(new_patient->gender, gender);
    strcpy(new_patient->medical_history, medical_history);
    strcpy(new_patient->diagnosis, diagnosis);
    strcpy(new_patient->prescription, prescription);
    new_patient->left = NULL;
    new_patient->right = NULL;
    return new_patient;
}

// Function to add a new patient to the binary search tree
struct Patient* add_patient(struct Patient* root, struct Patient* new_patient) {
    if (root == NULL) {
        return new_patient;
    }

    int cmp = strcmp(new_patient->name, root->name);
    if (cmp < 0) {
        root->left = add_patient(root->left, new_patient);
    } else if (cmp > 0) {
        root->right = add_patient(root->right, new_patient);
    }

    return root;
}

// Function to search for a patient by name in the binary search tree
struct Patient* search_patient(struct Patient* root, const char* name) {
    if (root == NULL || strcmp(root->name, name) == 0) {
        return root;
    }

    int cmp = strcmp(name, root->name);
    if (cmp < 0) {
        return search_patient(root->left, name);
    } else {
        return search_patient(root->right, name);
    }
}

// Function to find the minimum node (leftmost) in the binary search tree
struct Patient* find_min(struct Patient* root) {
    if (root == NULL) {
        return NULL;
    }

    while (root->left != NULL) {
        root = root->left;
    }
    return root;
}

// Function to delete a patient's record from the binary search tree
struct Patient* delete_patient_record(struct Patient* root, const char* name) {
    if (root == NULL) {
        return root;
    }

    int cmp = strcmp(name, root->name);
    if (cmp < 0) {
        root->left = delete_patient_record(root->left, name);
    } else if (cmp > 0) {
        root->right = delete_patient_record(root->right, name);
    } else {
        if (root->left == NULL) {
            struct Patient* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            struct Patient* temp = root->left;
            free(root);
            return temp;
        }

        struct Patient* temp = find_min(root->right);
        strcpy(root->name, temp->name);
        root->right = delete_patient_record(root->right, temp->name);
    }

    return root;
}

// Function to display all patient records (in-order traversal of the binary search tree)
void display_patient(struct Patient* patient) 
{
    if (patient != NULL) 
    {
        printf("Name: %s\n", patient->name);
        printf("Age: %d\n", patient->age);
        printf("Gender: %s\n", patient->gender);
        printf("Medical History: %s\n", patient->medical_history);
        printf("Diagnosis: %s\n", patient->diagnosis);
        printf("Prescription: %s\n", patient->prescription);
        printf("-----------------------------\n");
        
        display_patient(patient->left); // Display patients in the left subtree
        display_patient(patient->right); // Display patients in the right subtree
    }
}

void display_all_records(struct Patient* root) {
    if (root != NULL) {
        printf("Patient Records:\n");
        display_patient(root);
    } else {
        printf("No patient records found.\n");
    }
}


void save_patient_records(struct Patient *patient, FILE *file) {
    if (patient != NULL) {
        fprintf(file, "%s %d %s %s %s %s\n", patient->name, patient->age, patient->gender,
                patient->medical_history, patient->diagnosis, patient->prescription);
        save_patient_records(patient->left, file);
        save_patient_records(patient->right, file);
    }
}

void save_records_to_file(struct Patient *root, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    save_patient_records(root, file);

    fclose(file);
    printf("Patient records saved to %s successfully.\n", filename);
}


struct Patient *load_records_from_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file for reading.\n");
        return NULL;
    }

    struct Patient *root = NULL;

    while (!feof(file)) {
        char name[50];
        int age;
        char gender[10];
        char medical_history[100];
        char diagnosis[100];
        char prescription[100];

        if (fscanf(file, "%s %d %s %s %s %s\n", name, &age, gender, medical_history, diagnosis, prescription) == 6) {
            struct Patient *new_patient = create_patient(name, age, gender, medical_history, diagnosis, prescription);
            root = add_patient(root, new_patient);
        }
    }

    fclose(file);
    printf("Patient records loaded from %s successfully.\n", filename);

    return root;
}
int main() {
    struct Patient* root = NULL;
    struct AgeNode* age_tree = NULL;

    char name[50];
    int age, min_age, max_age;
    char gender[10];
    char medical_history[100];
    char diagnosis[100];
    char prescription[100];
    char filename[100];

    int choice = 0;
    while (choice != 9) {
        printf("Medical Records Management System\n");
        printf("1. Add Patient\n");
        printf("2. Search Patient\n");
        printf("3. Update Patient Record\n");
        printf("4. Delete Patient Record\n");
        printf("5. Display All Records\n"); 
        printf("6. Search by Age Range\n"); 
        printf("7. Save Records to File\n");
        printf("8. Load Records from File\n");
        printf("9. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter patient name: ");
                scanf("%s", name);
                printf("Enter patient age: ");
                scanf("%d", &age);
                printf("Enter patient gender: ");
                scanf("%s", gender);
                printf("Enter medical history: ");
                scanf(" %99[^\n]s", medical_history);
                printf("Enter diagnosis: ");
                scanf(" %99[^\n]s", diagnosis);
                printf("Enter prescription: ");
                scanf(" %99[^\n]s", prescription);

                // Create a new patient
                struct Patient* new_patient = create_patient(name, age, gender, medical_history, diagnosis, prescription);

                // Add the new patient to the binary search tree
                root = add_patient(root, new_patient);

                // Add the new patient to the age-based tree
                age_tree = add_patient_by_age(age_tree, new_patient);
                break;

            case 2:
                printf("Enter patient name to search: ");
                scanf("%s", name);
                struct Patient* patient = search_patient(root, name);
                if (patient != NULL) {
                    printf("Found patient:\n");
                    printf("Name: %s\n", patient->name);
                    printf("Age: %d\n", patient->age);
                    printf("Gender: %s\n", patient->gender);
                    printf("Medical History: %s\n", patient->medical_history);
                    printf("Diagnosis: %s\n", patient->diagnosis);
                    printf("Prescription: %s\n", patient->prescription);
                } else {
                    printf("Patient not found.\n");
                }
                break;

            case 3:
                printf("Enter patient name to update: ");
                scanf("%s", name);
                patient = search_patient(root, name);
                if (patient != NULL) {
                    printf("Enter updated medical history: ");
                    scanf(" %[^\n]s", medical_history);
                    printf("Enter updated diagnosis: ");
                    scanf(" %[^\n]s", diagnosis);
                    printf("Enter updated prescription: ");
                    scanf(" %[^\n]s", prescription);
                    strcpy(patient->medical_history, medical_history);
                    strcpy(patient->diagnosis, diagnosis);
                    strcpy(patient->prescription, prescription);
                    printf("Patient record updated successfully.\n");
                } else {
                    printf("Patient not found.\n");
                }
                break;

            case 4:
                printf("Enter patient name to delete: ");
                scanf("%s", name);
                root = delete_patient_record(root, name);
                printf("Patient record deleted successfully.\n");
                break;

            case 5:
                display_all_records(root);
                break;

            case 6:
                printf("Enter minimum age: ");
                scanf("%d", &min_age);
                printf("Enter maximum age: ");
                scanf("%d", &max_age);

                printf("Patients within the age range %d-%d:\n", min_age, max_age);
                search_patients_by_age_range(age_tree, min_age, max_age);
                break;

            case 7:
            printf("Enter file name to save patient records: ");
            scanf("%s", filename);
            save_records_to_file(root, filename);
            printf("Patient records saved to file successfully.\n");
            break;

            case 8:
                printf("Enter file name to load patient records: ");
                scanf("%s", filename);
                // Free existing memory before loading new records
                free(root);
                root = load_records_from_file(filename);
                printf("Patient records loaded from file successfully.\n");
                break;

            case 9:
                printf("Exiting the program.\n");
                break;

            default:
                    printf("Invalid choice. Please try again.\n");
                    break;
            }

        printf("\n");
    }

    // Clean up: Free memory used by the patient and age trees
    // You can add this code if you want to free the memory before exiting.
    // free(root);
    // free(age_tree);

    return 0;
}