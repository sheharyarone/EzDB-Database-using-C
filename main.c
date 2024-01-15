#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Define the structure for key-value pairs in the linked list
struct KeyValuePair
{
    char *key;
    float value;
    struct KeyValuePair *next;
};

// Define the structure for the dictionary
struct Dictionary
{
    int count;                 // Number of items in the dictionary
    struct KeyValuePair *head; // Pointer to the head of the linked list
    struct KeyValuePair *tail; // Pointer to the tail of the linked list
};

// Function to create a new dictionary
struct Dictionary *createDictionary()
{
    struct Dictionary *dict = (struct Dictionary *)malloc(sizeof(struct Dictionary));
    if (dict == NULL)
    {
        return NULL;
    }

    dict->count = 0;
    dict->head = NULL;
    dict->tail = NULL;

    return dict;
}

// Function to insert a key-value pair into the dictionary
int insert(struct Dictionary *dict, char *key, float value)
{
    // Check if the key already exists
    struct KeyValuePair *current = dict->head;
    while (current != NULL)
    {
        if (strcmp(current->key, key) == 0)
        {
            // Key already exists
            return -2;
        }
        current = current->next;
    }
    // Create a new key-value pair
    struct KeyValuePair *newPair = (struct KeyValuePair *)malloc(sizeof(struct KeyValuePair));

    newPair->key = strdup(key);
    newPair->value = value;
    newPair->next = NULL;

    if (dict->count == 0)
    {
        // First element in the dictionary
        dict->head = newPair;
        dict->tail = newPair;
    }
    else
    {
        // Add the new pair to the tail of the linked list
        dict->tail->next = newPair;
        dict->tail = newPair;
    }

    dict->count++;
    return -1;
}

// Function to retrieve a value by key
void query(struct Dictionary *dict, char *key)
{
    struct KeyValuePair *current = dict->head;

    while (current != NULL)
    {
        if (strcmp(current->key, key) == 0)
        {

            printf("A record of Key=%s, Value=%f is found in the database.\n", current->key, current->value);
            return;
        }
        current = current->next;
    }
    printf("There is no record with Key=%s found in the database.\n", key);

    // Key not found
    printf("There is no record with Key=%s found in the database.\n", key);
}

// Function to update the value of a key if it exists
int update(struct Dictionary *dict, char *key, float value)
{
    struct KeyValuePair *current = dict->head;
    while (current != NULL)
    {
        if (strcmp(current->key, key) == 0)
        {
            current->value = value;
            return -1;
        }
        current = current->next;
    }

    // Key not found
    return -2;
}

// Function to delete a key from the dictionary
int delete(struct Dictionary *dict, char *key)
{
    struct KeyValuePair *prev = NULL;
    struct KeyValuePair *current = dict->head;

    while (current != NULL)
    {
        if (strcmp(current->key, key) == 0)
        {
            if (prev != NULL)
            {
                prev->next = current->next;
            }
            else
            {
                // Deleting the head
                dict->head = current->next;
            }

            if (current == dict->tail)
            {
                // Deleting the tail
                dict->tail = prev;
            }

            free(current->key);
            free(current);
            dict->count--;
            return -1;
        }

        prev = current;
        current = current->next;
    }

    // Key not found
    return -2;
}
void showAll(struct Dictionary *dict)
{
    printf("There are in total %d records found:\n", dict->count);

    struct KeyValuePair *current = dict->head;
    while (current != NULL)
    {
        printf("%s %f\n", current->key, current->value);
        current = current->next;
    }
}



// READ DATA FROM THE FILE AND RETURN DICTIONARY POINTER
struct Dictionary *readFile(const char *filename)
{
    struct Dictionary *dict = createDictionary();

    FILE *file = fopen(filename, "r");

    char line[256]; // Adjust buffer size as needed
    while (fgets(line, sizeof(line), file))
    {
        // Remove the newline character at the end
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n')
        {
            line[len - 1] = '\0';
        }

        // Parse the line to extract key and value
        char *token = strtok(line, ":"); // Change the delimiter as needed
        if (token)
        {
            char *key = strdup(token);
            token = strtok(NULL, ":"); // Change the delimiter as needed
            if (token)
            {
                float value = atof(token);
                insert(dict, key, value);
            }
        }
    }

    fclose(file);
    return dict;
}

int saveDataToFile(struct Dictionary *dict, const char *fileName)
{
    FILE *file = fopen(fileName, "w");
    if (file == NULL)
    {
        return -1; // Failed to open the file for writing
    }

    struct KeyValuePair *current = dict->head;
    while (current != NULL)
    {
        fprintf(file, "%s:%f\n", current->key, current->value);

        struct KeyValuePair *temp = current;
        current = current->next;

        // Free the node after saving data
        free(temp->key);
        free(temp);
        dict->count--;
    }

    fclose(file);
    return 0; // Data saved successfully
}

void handleUserRequests(struct Dictionary *dict, const char *fileName)
{
    char command[256]; // Adjust buffer size as needed
    char key[256];
    char filenameCommand[256];
    float value;
    float newValue;

    while (1)
    {
        printf("COMMAND : ");
        if (fgets(command, sizeof(command), stdin) == NULL)
        {
            continue;
        }

        // Remove the newline character at the end
        size_t len = strlen(command);
        if (len > 0 && command[len - 1] == '\n')
        {
            command[len - 1] = '\0';
        }

        if (strcmp(command, "SHOW ALL") == 0)
        {
            // Show all data in the dictionary
            showAll(dict);
        }
        else if (sscanf(command, "INSERT %255s %f", key, &value) == 2)
        {
            // Insert a new key-value pair
            int response = insert(dict, key, value);
            if (response == -1)
            {
                printf("A new record of Key=%s, Value= %f is successfully inserted.\n", key, value);
            }
            else
            {
                printf("The record with Key=%s already exists in the database.\n", key);
            }
        }
        else if (sscanf(command, "UPDATE %255s %f", key, &newValue) == 2)
        {
            // Update the value associated with a key
            int response = update(dict, key, newValue);
            if (response == -1)
            {
                printf("The value for the record of Key=%s is successfully updated.\n", key);
            }
            else
            {
                printf("There is no record with Key=%s found in the database.\n", key);
            }
        }
        else if (sscanf(command, "QUERY %255s", key) == 1)
        {
            // Query the value associated with a key
            query(dict, key);
        }

        else if (sscanf(command, "DELETE %255s", key) == 1)
        {
            // Delete a key from the dictionary
            int response = delete (dict, key);
            if (response == -1)
            {
                printf("The record of Key=%s is successfully deleted.\n", key);
            }
            else
            {
                printf("There is no record with Key=%s found in the database.\n", key);
            }
        }
        else if (sscanf(command, "SAVE %s", filenameCommand) == 1)
        {
            // Add code here to save the data to the specified file (filename)
            if (strcmp(fileName, filenameCommand) != 0)
            {
                printf("WRONG FILE REFERENCE !\n");
                continue;
            }
            int saveStatus = saveDataToFile(dict, fileName);
            if (saveStatus == 0)
            {
                printf("Data saved to file '%s' successfully.\n", filenameCommand);
                break;
            }
        }
        else
        {
            printf("Invalid command. Please use 'SHOW ALL', 'INSERT', 'QUERY', 'UPDATE', 'DELETE', OR 'SAVE FILENAME.txt' .\n");
        }
    }
}

int main()
{
    struct Dictionary *dict = NULL;
    char fileName[50];
    while (1)
    {
        // Prompt the user to enter a command
        char command[256]; // Adjust buffer size as needed
        printf("COMMAND : ");
        if (fgets(command, sizeof(command), stdin) == NULL)
        {
            continue;
        }

        // Remove the newline character at the end
        size_t len = strlen(command);
        if (len > 0 && command[len - 1] == '\n')
        {
            command[len - 1] = '\0';
        }

        if (strncmp(command, "OPEN ", 5) == 0)
        {
            // Extract the filename after "OPEN "
            strcpy(fileName, command + 5);
            // Call readFile to open the file and create the dictionary
            if (fopen(fileName, "r") == NULL)
            {
                printf("File Not Found!. \n");
                continue;
            }
            dict = readFile(fileName);
            if (dict != NULL)
            {
                printf("File opened and data loaded successfully.\n");
                break;
            }
            else
            {
                printf("Failed to open or read data from the file.\n");
            }
        }
    }

    handleUserRequests(dict, fileName);

    return 0;
}
