#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <netdb.h>
#include <sys/unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int isIP(char const *address)
{
    char temp[100], *token;

    strcpy(temp, address);
    token = strtok(temp, ".");

    while (token)
    {
        for (int i = 0; i < strlen(token); i++)
        {
            if (!isdigit(token[i]))
                return 0;
        }

        if (atoi(token) < 0 || atoi(token) > 255)
            return 0;
        
        token = strtok(NULL, ".");
    }

    return 1;
}

void checkHostEntry(struct hostent *hostentry)
{
    if (hostentry == NULL)
    {
        printf("Not found information\n");
        exit(1);
    }
}

int main(int argc, char const *argv[])
{
    struct in_addr **addr_list, ipv4addr;
    struct hostent *host_entry;

    int i = 0;
    
    if (argc != 2 || ! strchr(argv[1], '.'))
    {
        printf("\nInvalid input.\n\n");
        return 1;
    }
    
    if (isIP(argv[1]))
    {
        inet_pton(AF_INET, argv[1], &ipv4addr);
        host_entry = gethostbyaddr(&ipv4addr, sizeof ipv4addr, AF_INET);

        checkHostEntry(host_entry);

        printf("\nOfficial name: %s", host_entry->h_name);

        char ** aliases = host_entry->h_aliases;
        
        while (aliases[i])
        {
            printf("\nAlias name:\n");
            printf("\t%s\n", aliases[i]);
            i++;
        }
        
        printf("\n");
    }
    
    else
    {
        host_entry = gethostbyname(argv[1]); 
        checkHostEntry(host_entry); 

        addr_list = (struct in_addr **) host_entry->h_addr_list;
        
        while (addr_list[i])
        {
            if (i == 0) printf("\nOfficial IP: ");
            else printf("\nAlias IP: \n");
            printf("\t%s\n", inet_ntoa(*addr_list[i]));
            i++;
        }

        printf("\n");
    }
    
    return 0;
}