
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_LEN 50

// Structure to store user information
struct User
{
    char username[MAX_LEN];
    char password[MAX_LEN];
} user, tempUser;

// Structure for inventory items
struct Inventory
{
    char itemName[MAX_LEN];
    int quantity;
    char unit[MAX_LEN];
    float price;
} inventoryItem;

int main()
{
    char choice[10];
    FILE *file;
    int loggedIn = 0; // Track if the user is logged in

    while (1)
    {
        printf("\t\t   =================================\n");
        printf("\t\t      Inventory Management System\n");
        printf("\t\t   =================================\n\n\n\n");
        printf("Type: 'register' to create a new account'login' to access your account: ");
        scanf("%s", choice);
        strlwr(choice);

        // ================= REGISTER ====================
        if (strcmp(choice, "register") == 0)
        {
            file = fopen("users.txt", "a+");
            if (file == NULL)
            {
                printf("\t\t   Error: Unable to open the file.\n\n\n");
                return 1;
            }

            printf("\nEnter new username (max 50 characters): ");
            scanf("%s", user.username);
            printf("Enter new password (max 50 characters): ");
            scanf("%s", user.password);

            // Check for duplicate username
            int duplicate = 0; // Initialize a flag to check if the username is already taken

            // Read existing usernames and passwords from the file line by line
            while (fscanf(file, "%s %s", tempUser.username, tempUser.password) != EOF)
            {
                if (strcmp(user.username, tempUser.username) == 0) // Compare the entered username with each existing username
                {
                    printf("\n\t\t   Username already exists. Try another.\n\n\n");
                    duplicate = 1; // Set the flag to 1 indicating duplicate found
                    break;         // Exit the loop since we already found a match
                }
            }

            // If no duplicate username was found
            if (duplicate == 0)
            {
                // Write the new username and password to the file
                fprintf(file, "%s %s\n", user.username, user.password);
                printf("\n\t\t   Registration successful!\n\n\n");
            }

            fclose(file);
        }
        // ================= LOGIN ====================
        else if (strcmp(choice, "login") == 0)
        {
            while (1)
            {
                file = fopen("users.txt", "r");
                if (file == NULL)
                {
                    printf("\t\t   Error opening file.\n");
                    return 1;
                }

                printf("\nEnter username: ");
                scanf("%s", user.username);
                printf("Enter password: ");
                scanf("%s", user.password);

                int found = 0;
                while (fscanf(file, "%s %s", tempUser.username, tempUser.password) != EOF)
                {
                    if (strcmp(user.username, tempUser.username) == 0 &&
                        strcmp(user.password, tempUser.password) == 0)
                    {
                        found = 1;
                        loggedIn = 1;
                        break;
                    }
                }
                fclose(file);

                if (found)
                {
                    printf("\n\t\t   Login successful! Welcome %s.\n\n\n", user.username);
                    break;
                }
                else
                {
                    printf("\n\t\t   Wrong username or password. Try again.\n\n\n");
                }
            }

            // ================= INVENTORY MENU ====================
            int menuChoice;

            while (1)
            {
                printf("\t\t            Main Menu\n");
                printf("[1]. Add Inventory\n");
                printf("[2]. View Inventory\n");
                printf("[3]. Update Inventory\n");
                printf("[4]. Delete Inventory\n");
                printf("[5]. Exit\n");
                printf("\nEnter your choice:");
                scanf("%d", &menuChoice);

                switch (menuChoice)
                {
                case 1:
                    // Add Inventory
                    file = fopen("inventory.dat", "ab");
                    if (file == NULL)
                    {
                        printf("\t\t   Error opening inventory file!\n");
                        break;
                    }

                    printf("\nEnter item name: ");
                    scanf("%s", inventoryItem.itemName);
                    printf("Enter quantity: ");
                    scanf("%d", &inventoryItem.quantity);
                    printf("Enter unit: ");
                    scanf("%s", &inventoryItem.unit);
                    printf("Enter price: ");
                    scanf("%f", &inventoryItem.price);

                    fwrite(&inventoryItem, sizeof(struct Inventory), 1, file);
                    fclose(file);
                    printf("\n\t\t   Item added successfully!\n\n\n");
                    break;

                case 2:
                    // View Inventory
                    file = fopen("inventory.dat", "rb");
                    if (file == NULL)
                    {
                        printf("\n\t\t   No inventory available.\n");
                        break;
                    }

                    int found = 0;
                    while (fread(&inventoryItem, sizeof(struct Inventory), 1, file))
                    {
                        printf("--------------------------------------------\n");
                        printf("Item::%s, Quantity::%d%s, Price::%.f\n", inventoryItem.itemName, inventoryItem.quantity, inventoryItem.unit, inventoryItem.price);
                        printf("--------------------------------------------\n");
                        found = 1;
                    }
                    fclose(file);
                    if (!found)
                        printf("\n\t\t   Inventory is empty.\n\n\n");
                    break;

                case 3:
                    // Update Inventory
                    file = fopen("inventory.dat", "rb"); // Open the inventory file for reading
                    if (file == NULL)
                    {
                        printf("\n\t\t   No inventory available.\n");
                        break;
                    }

                    char searchName[MAX_LEN];
                    found = 0; // Flag to check if item was found

                    printf("\nEnter item name to update: ");
                    scanf("%s", searchName);

                    FILE *temp = fopen("temp.dat", "wb"); // Create a temporary file for updated records
                    // Loop through each item in the original file
                    while (fread(&inventoryItem, sizeof(struct Inventory), 1, file))
                    {
                        // If the item name matches, update its details
                        if (strcmp(inventoryItem.itemName, searchName) == 0)
                        {
                            found = 1;
                            printf("Enter new quantity: ");
                            scanf("%d", &inventoryItem.quantity);
                            printf("Enter new unit: ");
                            scanf("%s", &inventoryItem.unit);
                            printf("Enter new price: ");
                            scanf("%f", &inventoryItem.price);
                        }
                        // Write the updated item to the temp file
                        fwrite(&inventoryItem, sizeof(struct Inventory), 1, temp);
                    }

                    fclose(file);
                    fclose(temp);

                    // Replace the old inventory file with the new updated file
                    remove("inventory.dat");
                    rename("temp.dat", "inventory.dat");

                    if (found)
                        printf("\n\t\t   Item updated successfully!\n\n\n");
                    else
                        printf("\n\t\t   Item not found.\n\n\n");
                    break;

                case 4:
                    // Delete Inventory
                    file = fopen("inventory.dat", "rb");
                    if (file == NULL)
                    {
                        printf("\n\t\t   No inventory available.\n");
                        break;
                    }

                    found = 0;
                    printf("\nEnter item name to delete: ");
                    scanf("%s", searchName);

                    temp = fopen("temp.dat", "wb"); // Open a temp file to write remaining items

                    // Read every item one by one
                    while (fread(&inventoryItem, sizeof(struct Inventory), 1, file))
                    {
                        // If current item is NOT the one to delete, keep it
                        if (strcmp(inventoryItem.itemName, searchName) != 0)
                        {
                            fwrite(&inventoryItem, sizeof(struct Inventory), 1, temp);
                        }
                        else
                        {
                            found = 1; // Found the item to delete
                        }
                    }

                    fclose(file); // Close original file
                    fclose(temp); // Close temp file

                    remove("inventory.dat");             // Delete old file
                    rename("temp.dat", "inventory.dat"); // Rename temp file to inventory.dat

                    if (found)
                        printf("\n\t\t   Item deleted successfully!\n\n\n");
                    else
                        printf("\n\t\t   Item not found.\n\n\n");
                    break;

                case 5:
                    // Exit the program

                    printf("\t\t\t         Thank you for using our system!\n");
                    printf("\t\t    We hope it helped you stay organized and efficient.\n");
                    printf("\t\t\t           Goodbye and best wishes!\n");

                    exit(0);

                default:
                    printf("Invalid option. Please try again.\n");
                    break;
                }
            }
        }
    }
    return 0;
}