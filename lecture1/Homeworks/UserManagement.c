#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define buf_clear while(getchar()!='\n');

typedef struct user
{
    char *username;
    char *password;
    int status, error, isLogin;
    struct user *next;
} user;

user *first;

void insertUser(user *newUser)
{
    // If user list is empty
    if (first == NULL)
    {
        first = newUser;
        return;
    }

    // Else, insert new user at the very end of user list
    user *temp = first;
    
    while (temp->next)
        temp = temp->next;
    temp->next = newUser;
}

user *findByUsername(char *username)
{
    user *temp = first;
    while(temp)
    {
        if (strcmp(temp->username, username) == 0)
            break;
        temp = temp->next;
    }
    return temp;
}

void spaceFree()
{
    user *temp = first;
    while(temp)
    {
        first = first->next;
        free(temp);
        temp = first;
    }
}

void loadData()
{
    char username[30], password[30];
    int status;
    user *u;

    FILE *f = fopen("account.txt", "r");
    
    if (f == NULL)
    {
        printf("\n*** Data file not found.\n");
        return;
    }

    while(! feof(f))
    {
        fscanf(f, "%s %s %d ", username, password, &status);
        
        u = (user *) malloc(sizeof(user));
        
        u->username = (char *) malloc((strlen(username)+1) * sizeof(char));
        strcpy(u->username, username);
        
        u->password = (char *) malloc((strlen(password)+1) * sizeof(char));
        strcpy(u->password, password);
        
        u->status = status;

        u->error = 0;

        u->isLogin = 0;
        
        insertUser(u);
    }
    
    printf("\n*** Data loaded.\n");

    fclose(f);
}

void writeData()
{
    FILE *f = fopen("account.txt", "w");
    
    user *temp = first;
    
    while(temp)
    {
        fprintf(f, "%s %s %d\n", temp->username, temp->password, temp->status);
        temp = temp->next;
    }

    printf("\n*** New data saved.\n\n");
    
    fclose(f);
}

int main()
{
    int opt;
    char username[30], password[30];
    user *u;

    loadData();
    
    do
    {
        printf("\n");
        printf("USER MANAGEMENT PROGRAM\n");
        printf("---------------------------------------------\n");
        printf("1. Register\n");
        printf("2. Sign in\n");
        printf("3. Search\n");
        printf("4. Sign out\n");
        printf("0. Exit\n");
        printf("Your choice (1-4, other to quit): ");
        scanf("%d",&opt);
        printf("\n");

        buf_clear;
        switch(opt)
        {
            case 1:
                while (1)
                {
                    printf("\n--- Register ---\n");
                    printf("\nUsername: ");
                    fgets(username, sizeof username, stdin);
                    username[strcspn(username, "\n")] = '\0';

                    if(username[0] == '\0') break;

                    if (findByUsername(username))
                    {
                        printf("\n*** Account already exists.\n");
                        buf_clear;
                        continue;
                    }

                    user *newUser = (user *) malloc(sizeof(user));
                    
                    newUser->username = (char *) malloc((strlen(username)+1) * sizeof(char));
                    strcpy(newUser->username, username);

                    printf("Password: ");
                    fgets(password, sizeof password, stdin);
                    password[strcspn(password, "\n")] = '\0';

                    if(password[0] == '\0') break;
                    
                    newUser->password = (char *) malloc((strlen(password)+1) * sizeof(char));
                    strcpy(newUser->password, password);

                    newUser->status = 1;
                    newUser->error = 0;
                    newUser->isLogin = 0;
                    
                    insertUser(newUser);

                    printf("\n*** Account registered successfully.\n");
                    
                    buf_clear;
                }
                
                break;

            case 2:
                while (1)
                {
                    printf("\n--- Sign in ---\n");
                    printf("\nUsername: ");
                    fgets(username, sizeof username, stdin);
                    username[strcspn(username, "\n")] = '\0';

                    if(username[0] == '\0') break;

                    u = findByUsername(username);
                    
                    if (! u)
                    {
                        printf("\n*** Account does not exist.\n");
                        buf_clear;
                        continue;
                    }

                    if (u->status == 0)
                    {
                        printf("\n*** Account is blocked.\n");
                        buf_clear;
                        continue;
                    }

                    printf("Password: ");
                    fgets(password, sizeof password, stdin);
                    password[strcspn(password, "\n")] = '\0';

                    if(password[0] == '\0') break;

                    if (strcmp(u->password, password) == 0)
                    {
                        u->error = 0;
                        u->isLogin = 1;
                        printf("\n*** Hello, %s.\n", u->username);
                    }
                    else
                    {
                        u->error ++;
                        printf("\n*** Password is incorrect.\n");
                        
                        if (u->error > 3)
                        {
                            u->status = 0;
                            u->isLogin = 0;
                            printf("\n*** You've entered the wrong password 3 times.\n    Your account is blocked.\n");
                        }
                    }

                    buf_clear;
                }
                
                break;

            case 3:
                while (1)
                {
                    printf("\n--- Search ---\n");
                    printf("\nUsername: ");
                    fgets(username, sizeof username, stdin);
                    username[strcspn(username, "\n")] = '\0';

                    if(username[0] == '\0') break;

                    u = findByUsername(username);
                    
                    if (! u)
                    {
                        printf("\n*** Account does not exist.\n");
                        buf_clear;
                        continue;
                    }

                    if (u->status == 0)
                        printf("\n*** Account is blocked.\n");
                    else
                        printf("\n*** Account is active.\n");
                    
                    buf_clear;
                }

                break;

            case 4:
                while (1)
                {
                    printf("\n--- Sign out ---\n");
                    printf("\nUsername: ");
                    fgets(username, sizeof username, stdin);
                    username[strcspn(username, "\n")] = '\0';

                    if(username[0] == '\0') break;

                    u = findByUsername(username);
                    
                    if (! u)
                    {
                        printf("\n*** Account does not exist.\n");
                        buf_clear;
                        continue;
                    }

                    if (u->isLogin == 1)
                    {
                        u->isLogin = 0;
                        printf("\n*** Goodbye, %s.\n", u->username);
                    }
                    else
                        printf("\n*** Account is not signed in yet.\n");

                    buf_clear;
                }

                break;

            case 0:
                printf("\n--- Exit ---\n");
                if (first)
                    writeData();
                spaceFree();
                break;
        }
    }
    while(opt!=0);
    return 0;
}