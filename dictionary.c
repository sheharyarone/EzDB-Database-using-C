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
float query(struct Dictionary *dict, char *key)
{
    struct KeyValuePair *current = dict->head;

    while (current != NULL)
    {
        if (strcmp(current->key, key) == 0)
        {

            return current->value;
        }
        current = current->next;
    }
    return -2;

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
