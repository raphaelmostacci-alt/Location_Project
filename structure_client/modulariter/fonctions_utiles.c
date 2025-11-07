#include "librairies_rep.h"
#include "prototypes_utiles.h"

// Function to open the file
void open_file(FILE** clientFile)
{
    fprintf(stdout, "open_file: Begin\n");

    *clientFile=fopen("client_list.txt", "r+b");
    if(*clientFile==NULL)
    {
        fprintf(stdout, "Error opening file r+b\n");

        *clientFile=fopen("client_list.txt", "w+b");
        if(*clientFile==NULL)
        {
            fprintf(stdout, "Error opening file w+b. Exiting program\n");
            exit(-1);
        }
    }
    fprintf(stdout, "open_file: End\n");
}

// Function to search for a client
void search_client(FILE* file, client* who_client)
{
    printf("\nEnter the last name of the client to search: ");
    scanf("%s", who_client->last_name);
    printf("\nEnter the first name of the client to search: ");
    scanf("%s", who_client->first_name);
}

// Function to find a client's number in function of their last and first name
void find_number_client(FILE* file, client who_client)
{
    int read_items=0, found=0;
    client current_client;

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
                found++;
        }while(!feof(file) && read_items!=0 && (strcmp(current_client.last_name, who_client.last_name)!=0 || strcmp(current_client.first_name, who_client.first_name)!=0));

        if(read_items==0 || feof(file))
            printf("\nThe searched client was not found in the file\n");
        else
        {
            printf("Last Name: |%s| \tFirst Name: |%s| \tPhone Number: |%ld|\n", current_client.last_name, current_client.first_name, current_client.phone_number);
        }
    }
}

// Function to change a client's number
void change_number_client(FILE* file, client who_client)
{
    int nb_elements=0;

    find_number_client(file, who_client);

    printf("\nEnter the new phone number: ");
    scanf("%ld", &who_client.phone_number);

    // Move the file pointer to the position of the found client
    fseek(file, (nb_elements-1)*sizeof(client), SEEK_CUR);

    // Update the totality of the client's phone number in the file
    nb_elements=fwrite(&who_client, sizeof(client), 1, file);
    if(nb_elements!=1)
        printf("Error updating phone number\n");
    else
    {
        printf("Phone number updated successfully\n");
        printf("New details - Last Name: |%s| \tFirst Name: |%s| \tPhone Number: |%ld|\n", who_client.last_name, who_client.first_name, who_client.phone_number);
    }
}
