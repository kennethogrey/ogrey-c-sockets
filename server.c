#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>//for sockets
#include <netinet/in.h> //to store internet address family
#include <sys/types.h>//defines data types used
#include <netdb.h> //netnet structure eg char *n_name 
#include <arpa/inet.h>//INET_ADDRSTRLEN & INET6_ADDRSTRLEN 
#include <fcntl.h> // for open
#include <unistd.h> // for close

int main(){
    int server_socket, client_size, client_socket, n;
    char buffer[1024];
    struct sockaddr_in server_addr, client_addr;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);//create socket
    if (server_socket < 0)
     {
        printf("\nSERVER - Unable to create socket\n");
        return -1;
     }else{
        printf("\nSERVER - Socket created successfully\n");
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2000);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind to the set port and IP:
    if(bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr))<0){
        printf("\nSERVER - Couldn't bind to the port\n");
        return -1;
    }else{
        printf("\nSERVER - Binding successful\n");
    }

     // Listen for clients:
    if(listen(server_socket, 1) < 0){
        printf("\nSERVER - Error while listening\n");
        return -1;
    }else{
    printf("\nSERVER - Listening for incoming connections.....\n\n");
    }

    // Accept an incoming connection:
    client_size = sizeof(client_addr);
    client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_size);
    
    if (client_socket < 0){
        printf("\nSERVER - Can't accept\n");
        return -1;
    }else{
        printf("\nSERVER - Connected successfully\n\n");
    }

    char username[255];
    char password[255];
    char district[255];

    read(client_socket, username, 255);
    printf("\nSERVER - %s is logging in!\n", username );

    read(client_socket, password, 255);
    printf("\nSERVER - Password provided by %s has been verified and is correct\n", username);
    printf("\nSERVER - Login in session for %s has been initiated and the user has been granted acces to the system!!!!!\n", username);

    read(client_socket, district, 255);
    printf("\nSERVER - %s has logged in from %s\n", username, district);

    char patient_fname[255];
    char patient_lname[255];
    char date[255];
    char gender[255];
    char category[255];

    char file_name[255];
    if(read(client_socket, file_name,255)){
        printf("\nSERVER - Name for the file to be created has recieved from client succefully!!!!!\n");
    }

    FILE *filePointer = fopen(file_name, "w");
    printf("\nSERVER - %s has been created successfully!!!!\n", file_name);
    fclose(filePointer);

    B: if(read(client_socket, patient_fname, 255)){
        printf("\nSERVER - New patient's firstname has been recieved successfully.\n");
        printf("PATIENT'S FIRSTNAME - %s\n", patient_fname);
    }else{
        printf("\nSERVER - ERROR reading new patient's firstname from the client!!!!!\n");
    }

    if(read(client_socket, patient_lname, 255)){
        printf("\nSERVER - New patient's lastname has been recieved successfully.\n");
        printf("PATIENT'S LASTNAME - %s\n", patient_lname);
    }else{
        printf("\nSERVER - ERROR reading new patient's lastname from the client!!!!!\n");
    }

    if(read(client_socket, date, 255)){
        printf("\nSERVER - New patient's identification date has been recieved successfully.\n");
        printf("PATIENT'S IDENTIFICATION DATE - %s\n", date);
    }else{
        printf("\nSERVER - ERROR reading new patient's identification date from the client!!!!!\n");
    }

    if(read(client_socket, gender, 255)){
        printf("\nSERVER - New patient's gender has been recieved successfully.\n");
        printf("PATIENT'S GENDER - %s\n", gender);
    }else{
        printf("\nSERVER - ERROR reading new patient's gender from the client!!!!!\n");
    }

    if(read(client_socket, category, 255)){
        printf("\nSERVER - New patient's category has been recieved successfully.\n");
        printf("PATIENT'S CATEGORY - %s\n", category);
    }else{
        printf("\nSERVER - ERROR reading new patient's category from the client!!!!!\n");
    }

    char patient[255];

    void addPatient(){
        strcat(patient, patient_lname);
        strcat(patient, " ");
        strcat(patient, patient_fname);
        strcat(patient, "\t\t");
        strcat(patient, date);
        strcat(patient, " ");
        strcat(patient, gender);
        strcat(patient, " ");
        strcat(patient, username);
    }

    addPatient();
    printf("\n------------------------------------------------------------------------\n");
    printf("\nNEW PATIENT'S RECORD - %s\n",patient);
    printf("\n------------------------------------------------------------------------\n");

    if(write(client_socket,patient,255)){
        printf("\nSERVER - New patient's record has been sent to the client for verification!!!!\n");
    }

    int recordTrue;
    if(read(client_socket, &recordTrue, sizeof(int))){
        printf("\nSERVER - Record verification answer has been recieved and now proceeding accordingly!!!!\n");
    }

    void AddpatientList(){
        int successFile;
        printf("\nSERVER - Patient record is correct!!!!\n");
        filePointer = fopen(file_name, "a");
        if(fprintf(filePointer,"%s\n",patient )){
            printf("\nPATIENT RECORD FOR - %s \nhas been added to %s successfully\n",patient, file_name);
            successFile = 1;
            write(client_socket, &successFile, sizeof(int));
        }
        fclose(filePointer);   
    }
    if (recordTrue == 1)
    { 
        AddpatientList();
        goto A;
    }else{
        printf("\nSERVER - Patient record is incorrect and waiting for correct patient information!!!!\n");
        int i = 0;
        for (int i = 0; i < strlen(patient); ++i)
        {
            patient[i] = 0;
        }
        goto B;
    }

    A: printf("\nSERVER - Waiting for user's next command!!!\n");
    int newPatient;
    if(read(client_socket, &newPatient, sizeof(int))){
        printf("\nSERVER - User's next instruction recieved successfully!!!\n");
    }

    if (newPatient == 1)
    {
        printf("\nSERVER - User wants to add a new patient!!!\n");
        for (int i = 0; i < strlen(patient); ++i)
        {
            patient[i] = 0;
        }
        goto B;
    }else{
        printf("\nSERVER - User doesnt want to add a new patient!!!\n");
        goto C;
    }

    C: printf("\nSERVER - waiting for the user's next command!!!\n");
    int readChoice;
    if(read(client_socket, &readChoice, sizeof(int))){
        printf("\nSERVER - User's next instruction recieved successfully!!!\n");
    }

    switch(readChoice){
        case 1:
        printf("\nSERVER - User wants to find out how many patients are in the %s file!!!\n", file_name);
        void Check_status(){
            int lines = 0;
            char n_char;
            filePointer = fopen(file_name, "r");
            //extract character from file and store in chr
            n_char = getc(filePointer);
            while (n_char != EOF){
                //Count whenever new line is encountered
                if (n_char == '\n')
                {
                    lines = lines + 1;
                }
                //take next character from file.
                n_char = getc(filePointer);
            }
            fclose(filePointer); //close file.
            
            if(lines == 1){
            printf("\n---------------------------------------------------------------------\n");
            printf("\nThere is %d patient in %s file!!!\n", lines, file_name);
            printf("\n---------------------------------------------------------------------\n");
            }else{
                printf("\n---------------------------------------------------------------------\n");
                printf("\nThere are %d patients in %s file!!!\n", lines, file_name);
                printf("\n---------------------------------------------------------------------\n");
            }

            if(write(client_socket, &lines, sizeof(int))){
                printf("\nSERVER - Number of patients in the %s file has been sent to the client successfully!!\n", file_name);
            }
        }

        Check_status();
        break;

        case 2:
        printf("\nSERVER - User wants to search for a patient record by the patient name!!!\n");
        char firstName[255];
        char lastName[255];
        char fullname[255];

        if(read(client_socket, firstName, 255)){
            printf("\nPATIENT'S FIRSTNAME - %s\n", firstName);
        }

        if(read(client_socket, lastName, 255)){
            printf("\nPATIENT'S LASTNAME - %s\n", lastName);
        }

        strcat(fullname, lastName);
        strcat(fullname, " ");
        strcat(fullname, firstName);

        printf("\nPATIENT'S NAME AS SAVED IN THE %s file - %s\n", file_name, fullname);

        char line[255];
        char records[1024];
        void SearchName(){
            filePointer = fopen(file_name, "r");
            while(fgets(line, 255, filePointer)){
                char *compare = strstr(line, fullname);
                if (compare != NULL)
                {
                    strcat(records, line);
                    strcat(records, "\n");
                }
            }
            fclose(filePointer);
        }

        SearchName();
        printf("\nSERVER - Patient records found for %s in the %s file!!!!\n", fullname, file_name);
        printf("\n------------------------------------------------------------------------\n");
        printf("\n%s",records);
        printf("\n------------------------------------------------------------------------\n");
        if(write(client_socket, records, 1024)){
            printf("\nSERVER - The patient records have been sent to the client successfully!!\n");
            for (int i = 0; i < strlen(fullname); i++)
            {
                fullname[i] = 0;
            }
            for (int i = 0; i < strlen(records); i++)
            {
                records[i] = 0;
            }
        }
        break;
        
        case 3:
        printf("\nSERVER - User wants to search for a patient record by date of identification!!!\n");
        char date[255];
        if(read(client_socket, date, 255)){
            printf("\nSERVER - The date has been recieved from the client successfully!!!!");
            printf("\nDATE - %s", date);
        }
        char line2[255];
        char records2[1024];
        void SearchDate(){
            filePointer = fopen(file_name, "r");
            while(fgets(line2, 255, filePointer)){
                char *compare = strstr(line2, date);
                if (compare != NULL)
                {
                    strcat(records2, line2);
                    strcat(records2, "\n");
                }
            }
            fclose(filePointer);
        }
        SearchDate();
        printf("\nSERVER - Patient records found for %s in the %s file!!!!", date, file_name);
        printf("\n------------------------------------------------------------------------\n");
        printf("\n%s",records2);
        printf("\n------------------------------------------------------------------------\n");
        if(write(client_socket, records2, 1024)){
            printf("\nSERVER - The patient records have been sent to the client successfully!!\n");
            for (int i = 0; i < strlen(records2); i++)
            {
                records2[i] = 0;
            }
        }
    }

    int progControl;
    if(read(client_socket, &progControl, sizeof(int))){
        printf("\nSERVER -  Program control instruction recieved from the client successfully.\n");
        switch(progControl){
            case 1:
            //clearing the patient string variable
            for (int i = 0; i < strlen(patient); i++)
            {
                patient[i] = 0;
            }
            goto B;
            break;

            case 2:
            goto C;
            break;

            case 3:
            printf("\n\t\tGOOD BYE!!!\t\t\n\n");
            goto K;
            break;
        }
    }else{
        printf("\nSERVER - ERROR reading from the client.\n");
    }
    K: close(client_socket);
    close(server_socket);
    return 0;
}