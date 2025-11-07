#include "librairies_rep.h"
#include "prototypes_principales.h"

// Function to add a client
void add_client(FILE* file)
{
    client new_client;
    int nb_elements=0, read_items=0, i=0;

    if(file==NULL)
        printf("Error creating file\n");
    else
    {
        // Move to the end of the file to append new client
        fseek(file,0,SEEK_SET);
        do
        {
            memset(&new_client,0,sizeof(client));
            read_items=fread(&new_client,sizeof(client),1,file);
            i++;
        }while(!feof(file) && read_items!=0);

        printf("\nLast Name: ");
        scanf("%s", new_client.last_name);
        printf("\nFirst Name: ");
        scanf("%s", new_client.first_name);
        printf("\nPhone Number: ");
        scanf("%ld", &new_client.phone_number);

        nb_elements=fwrite(&new_client,sizeof(client),1,file);
        // Check if write was successful
        if(nb_elements!=1)
            printf("Error writing to file\n");
        else
        {
            printf("Client added successfully\n");
        }
    }
} 

// Function to show the number of clients
void show_number_client(FILE* file)
{
    client current_client;
    int read_items=0;

    if(file==NULL)
        printf("Error opening file\n");
    else
    {
        fseek(file,0,SEEK_SET);
        do
        {
            memset(&current_client,0,sizeof(client));
            read_items=fread(&current_client,sizeof(client),1,file);

            if(read_items!=0)
                printf("Last Name: |%s| \tFirst Name: |%s| \tPhone Number: |%ld|\n", current_client.last_name, current_client.first_name, current_client.phone_number);
        }while(!feof(file) && read_items!=0);
    }
}