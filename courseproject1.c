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

// Function prototypes
struct Patient* create_patient(const char* name, int age, const char* gender, const char* medical_history, const char* diagnosis, const char* prescription);
struct Patient* add_patient(struct Patient* root, struct Patient* new_patient);
struct Patient* search_patient(struct Patient* root, const char* name);
struct Patient* find_min(struct Patient* root);
struct Patient* delete_patient_record(struct Patient* root, const char* name);
void display_all_records(struct Patient* root);

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
void display_all_records(struct Patient* root) {
    if (root != NULL) {
        display_all_records(root->left);
        printf("Name: %s\n", root->name);
        printf("Age: %d\n", root->age);
        printf("Gender: %s\n", root->gender);
        printf("Medical History: %s\n", root->medical_history);
        printf("Diagnosis: %s\n", root->diagnosis);
        printf("Prescription: %s\n", root->prescription);
        printf("-----------------------------\n");
        display_all_records(root->right);
    }
}

int main() {
    struct Patient* root = NULL;

    char name[50];
    int age;
    char gender[10];
    char medical_history[100];
    char diagnosis[100];
    char prescription[100];

    int choice = 0;
    while (choice != 5) {
        printf("Medical Records Management System\n");
        printf("1. Add Patient\n");
        printf("2. Search Patient\n");
        printf("3. Update Patient Record\n");
        printf("4. Delete Patient Record\n");
        printf("5. Exit\n");
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
            scanf(" %[^\n]s", medical_history);
            printf("Enter diagnosis: ");
            scanf(" %[^\n]s", diagnosis);
            printf("Enter prescription: ");
            scanf(" %[^\n]s", prescription);
            root = add_patient(root, create_patient(name, age, gender, medical_history, diagnosis, prescription));
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
            printf("Exiting the program.\n");
            break;

        default:
            printf("Invalid choice. Please try again.\n");
            break;
        }

        printf("\n");
    }

    return 0;
}
