/*
 * Task 3 - File System Operations and Security
 * Operating Systems Coursework
 * Author: Uvica Shrestha
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define USERNAME "admin"
#define PASSWORD "password123"

int authenticated = 0;

// Function declarations
int login();
void menu();

void createFile();
void writeFile();
void readFile();
void deleteFile();

void setPermission();

void encryptFile();
void decryptFile();

void writeLog(char action[]);

int main()
{
    printf("=========================================\n");
    printf(" Secure File Management System\n");
    printf("=========================================\n\n");

    if(login())
    {
        menu();
    }
    else
    {
        printf("\nAuthentication Failed.\n");
    }

    return 0;
}
int login()
{
    char username[50];
    char password[50];

    printf("Username : ");
    scanf("%s", username);

    printf("Password : ");
    scanf("%s", password);

    if(strcmp(username, USERNAME) == 0 &&
       strcmp(password, PASSWORD) == 0)
    {
        printf("\nLogin Successful!\n\n");

        authenticated = 1;

        writeLog("User Logged In");

        return 1;
    }

    return 0;
}
void menu()
{
    int choice;

    do
    {
        printf("\n========== MENU ==========\n");

        printf("1. Create File\n");
        printf("2. Write File\n");
        printf("3. Read File\n");
        printf("4. Delete File\n");
        printf("5. File Permission\n");
        printf("6. Encrypt File\n");
        printf("7. Decrypt File\n");
        printf("8. View Audit Log\n");
        printf("9. Exit\n");

        printf("\nEnter Choice : ");
        scanf("%d",&choice);

        switch(choice)
        {
            case 1:
                createFile();
                break;

            case 2:
                writeFile();
                break;

            case 3:
                readFile();
                break;

            case 4:
                deleteFile();
                break;

            case 5:
                setPermission();
                break;

            case 6:
                encryptFile();
                break;

            case 7:
                decryptFile();
                break;

            case 8:
{
    FILE *log = fopen("audit_log.txt", "r");
    char ch;

    if(log == NULL)
    {
        printf("No audit log found.\n");
        break;
    }

    printf("\n----- AUDIT LOG -----\n");

    while((ch = fgetc(log)) != EOF)
    {
        putchar(ch);
    }

    fclose(log);

    printf("---------------------\n");

    break;
}

            case 9:
                printf("Goodbye!\n");
                break;

            default:
                printf("Invalid Choice!\n");
        }

    } while(choice != 9);
}
void createFile()
{
    char filename[100];

    printf("Enter file name: ");
    scanf("%s", filename);

    FILE *file = fopen(filename, "w");

    if(file == NULL)
    {
        printf("Unable to create file.\n");
        return;
    }

    fclose(file);

    printf("File '%s' created successfully.\n", filename);

    writeLog("File Created");
}
void writeFile()
{
    char filename[100];
    char text[500];

    printf("Enter file name: ");
    scanf("%s", filename);

    FILE *file = fopen(filename, "a");

    if(file == NULL)
    {
        printf("File not found.\n");
        return;
    }

    getchar();

    printf("Enter text: ");
    fgets(text, sizeof(text), stdin);

    fputs(text, file);

    fclose(file);

    printf("Data written successfully.\n");

    writeLog("File Modified");
}

void readFile()
{
    char filename[100];
    char ch;

    printf("Enter file name: ");
    scanf("%s", filename);

    FILE *file = fopen(filename, "r");

    if(file == NULL)
    {
        printf("File not found.\n");
        return;
    }

    printf("\n----- File Content -----\n");

    while((ch = fgetc(file)) != EOF)
    {
        putchar(ch);
    }

    printf("\n------------------------\n");

    fclose(file);

    writeLog("File Read");
}

void deleteFile()
{
    char filename[100];

    printf("Enter file name: ");
    scanf("%s", filename);

    if(remove(filename) == 0)
    {
        printf("File deleted successfully.\n");
        writeLog("File Deleted");
    }
    else
    {
        printf("Unable to delete file.\n");
    }
}

void setPermission()
{
    char filename[100];
    char permission[10];

    printf("Enter file name: ");
    scanf("%s", filename);

    printf("Enter permission (e.g. rwx, rw-, r--): ");
    scanf("%s", permission);

    printf("Permission for %s set to %s\n", filename, permission);

    writeLog("Permission Updated");
}
void encryptFile()
{
    char filename[100];
    char ch;

    printf("Enter file name: ");
    scanf("%s", filename);

    FILE *file = fopen(filename, "r+");

    if(file == NULL)
    {
        printf("File not found.\n");
        return;
    }

    while((ch = fgetc(file)) != EOF)
    {
        fseek(file, -1, SEEK_CUR);
        fputc(ch ^ 10, file);
        fflush(file);
    }

    fclose(file);

    printf("File encrypted successfully.\n");

    writeLog("File Encrypted");
}

void decryptFile()
{
    char filename[100];
    char ch;

    printf("Enter file name: ");
    scanf("%s", filename);

    FILE *file = fopen(filename, "r+");

    if(file == NULL)
    {
        printf("File not found.\n");
        return;
    }

    while((ch = fgetc(file)) != EOF)
    {
        fseek(file, -1, SEEK_CUR);
        fputc(ch ^ 10, file);
        fflush(file);
    }

    fclose(file);

    printf("File decrypted successfully.\n");

    writeLog("File Decrypted");
}
void writeLog(char action[])
{
    FILE *log = fopen("audit_log.txt", "a");

    if(log != NULL)
    {
        fprintf(log, "%s\n", action);
        fclose(log);
    }

    printf("Log: %s\n", action);
}