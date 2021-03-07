#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h> //for time
#include <sys/socket.h> //for sockets
#include <netinet/in.h> //to store internet address family
#include <sys/types.h> //defines data types used
#include <netdb.h>  //netnet structure eg char *n_name 
#include <arpa/inet.h> //INET_ADDRSTRLEN & INET6_ADDRSTRLEN 
#include <fcntl.h> // for open
#include <unistd.h> // for close

int main(){
    int server_socket;
    char buffer[1024]; 
    struct sockaddr_in server_addr;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);//create socket
    if (server_socket < 0)
     {
        printf("\nCLIENT - Unable to create socket\n");
        return -1;
     }else{
        printf("\nCLIENT - Socket created successfully\n");
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2000);
    server_addr.sin_addr.s_addr = INADDR_ANY;

     // Send connection request to server:
    if(connect(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        printf("\nCLIENT - Unable to connect\n");
        return -1;
    }else{
    printf("\nCLIENT - Connected with server successfully\n\n");
    }

    int usernameExist = 0;
    int passwordExist = 0;
    int districtExist = 0;
    char username[255];
    char password[255];
    char district[255];

    //function to verify the username 
    void verifyUsername(){
        FILE *log;// file pointer
        int bufferLength = 255;
        D: printf("\nCLIENT - Please enter your username (MAKE SURE ALL LETTERS ARE SMALL): ");
        scanf("%s",username);
        char line1[bufferLength];
        log=fopen("officials.txt","r");

        while(fgets(line1, bufferLength, log)){
            char *ptr1 = strstr(line1, username);
            if(ptr1 != NULL)
            {
                usernameExist = 1;
                break;
            }
        }
        fclose(log);
        if (usernameExist == 1)
        {
            printf("\nCLIENT - Username found procced and enter the correct password to login!!!\n");
        }else{
            printf("\nCLIENT - Username not found. Make sure you have been registered by the admin then try again!!!!\n");
            goto D;
        }
    }

    //function to verify the password
    void verifyPassword(){
        FILE *log;
        int bufferLength = 255;
        E: printf("\nCLIENT - Hello %s please enter your password (AS PROVIDED BY THE SYSTEM ADMIN): ", username);
        scanf("%s",password);
        char line2[bufferLength];
        log=fopen("officials.txt","r");

        while(fgets(line2, bufferLength, log)){
            char *ptr2 = strstr(line2, password);
            if(ptr2 != NULL)
            {
                passwordExist = 1;
                break;
            }
        }
        fclose(log);
        if (passwordExist == 1)
        {
            printf("\nCLIENT - Password found and verified successfully\n");
        }else{
            printf("\nCLIENT - Incorrect password entered. Please enter the correct password (AS PROVIDED BY THE SYSTEM ADMIN)\n");
            goto E;
        }
    }

    //calling the username verification function
    X: verifyUsername();

    //calling the password verification function
    verifyPassword();

    //logging in the user and sending their info to the server
    if ((usernameExist == 1) && (passwordExist == 1))
    {
        write(server_socket, username, 255);
        write(server_socket, password, 255);
        printf("\nCLIENT - You have logged in successfully and your information has been successfully sent to the server\n");
    }else{
        int choice;
        printf("CLIENT - Invalid username/password\n");
        printf("\nCLIENT - Enter 1 to try again / Enter 2 to quit: ");
        scanf("%d",&choice);
        switch(choice){
        case 1:
            goto X;
            break;
        case 2:
            printf("\n\nCLIENT - Quitting the program!!!!!!!!!\n");
            goto Y;
            break;
        }
    }
    
    //function to verify the district
    void verifyDistrict(){
        FILE *log;
        int bufferLength = 255;
        Z: printf("\nCLIENT - Please enter your district to continue (MAKE SURE FIRST LETTER IS CAPITAL): ");
        scanf("%s",district);
        char line3[bufferLength];
        log=fopen("districts.txt","r");

        while(fgets(line3, bufferLength, log)){
            char *ptr2 = strstr(line3, district);
            if(ptr2 != NULL)
            {
                districtExist = 1;
                break;
            }
        }
        fclose(log);
        if (districtExist == 1)
        {
            printf("\nCLIENT - District found and verified successfully\n");
        }else{
            printf("\nCLIENT - District not found. Please enter a correct distict (MAKE SURE FIRST LETTER IS CAPITAL)\n");
            goto Z;
        }
    }

    //calling the function to verify the district
    verifyDistrict();

    if (districtExist == 1){
        write(server_socket, district, 255);// sending the district to the server
        printf("\nCLIENT - User's district information has been sent to the server successfully!!!!\n");
    }else{
        printf("\nCLIENT - ERROR sending user's district to the server.\n");
    }

    char patient_fname[255];
    char patient_lname[255];
    char date[255];
    char gender[255];
    char category[255];
    char file_name[255];

    //getting the file name and sending it to the server
    printf("\nCLIENT - Type in name of text file to be created for saving the patients information in the server(eg file.txt): ");
    scanf("%s", file_name);
    write(server_socket, file_name,255);

    printf("\nCLIENT - Enter new patient information below\n");

    //getting the patient's first name and sending it to the server
    B: printf("\nCLIENT - Enter patient's firstname (FIRST LETTER OF THE NAME SHOULD BE CAPITAL): ");
    scanf("%s", patient_fname);
    if(write(server_socket, patient_fname, 255)){
        printf("CLIENT - Patient's firstname has been sent to the server successfully and stored!!\n");
    }

    //getting the patient's lastname and sending it to the server
    printf("\nCLIENT - Enter patient's lastname (FIRST LETTER OF THE NAME SHOULD BE CAPITAL): ");
    scanf("%s", patient_lname);
    if(write(server_socket, patient_lname, 255)){
        printf("CLIENT - Patient's lastname has been sent to the server successfully and stored!!\n");
    }

    //getting the current date and storing it in a string variable
    time_t t;    
    struct tm* tm;
    char currentDate[255];
    time(&t);
    tm = localtime(&t);
    strftime(currentDate, sizeof currentDate, "%Y-%m-%d", tm);

    //getting the patient's identification date
    I: printf("\nCLIENT - Enter the patient identification date. (YYYY-MM-DD): ");
    scanf("%s", date);
    //verifying the date and sending it to the server
    if(strcmp(date, currentDate) == 0){
        if(write(server_socket, date, 255)){
            printf("CLIENT - Patient's date of identification has been sent to the server successfully and stored!!\n");
        }
    }else{
        printf("\nPlease enter today's date in the format (YYYY-MM-DD)\n");
        goto I;
    }

    ////getting the patient's and sending it to the server
    F: printf("\nCLIENT - Enter patient's gender (F / M): ");
    scanf("%s", gender);
    //verifying the gender
    if(strcmp(gender, "F") == 0 || strcmp(gender, "M") == 0){
        if(write(server_socket, gender, 255)){
            printf("CLIENT - Patient's gender has been sent to the server successfully and stored!!\n");
        }
    }else{
        printf("\nCLIENT - Please enter a valid gender.\n");
        goto F;
    }

    //getting the patient's category and sending it to the server
    G: printf("\nCLIENT - Enter patient's category (asymptomatic / symptomatic): ");
    scanf("%s", category);
    //verifying the category
    if(strcmp(category, "asymptomatic") == 0 || strcmp(category, "symptomatic") == 0){
        if(write(server_socket, category, 255)){
            printf("CLIENT - Patient's category has been sent to the server successfully and stored!!\n\n");
        }
    }else{
        printf("\nCLIENT -  Please enter a valid category.\n");
        goto G;
    }

    char patient[255];
    if (read(server_socket, patient,255))//recieving the patient record from the server
    {
        printf("\n-------------------------------------------------------------------------\n");
        printf("\nThis is the patient record that has been generated by the server for the information you submitted: \n\n%s\n", patient);
        printf("\n-------------------------------------------------------------------------\n"); 
    }

    printf("\nCLIENT - Is the record above correct / incorrect???\n");

    //verifying the patient record and sending it to the server
    int recordTrue;
    H: printf("\n\tOption 1: Enter 1 if patient record above is correct.\n\tOption 2:Enter 2 to edit it if its incorrect: ");
    scanf("%d", &recordTrue);
    write(server_socket, &recordTrue, sizeof(int));
    switch(recordTrue){
        case 1:
        printf("\nCLIENT: The record above is correct. Server proceeding to save it to %s!!!\n", file_name);
        int successFile;
        read(server_socket, &successFile, sizeof(int));//recieving confimation that the record has been saved to the file or not
        if (successFile == 1)
        {
            printf("\nCLIENT - PATIENT RECORD - %s \nhas been saved to %s successfully!!!\n", patient, file_name);
        }else{
            printf("CLIENT - ERROR saving PATIENT RECORD - %s to %s\n", patient, file_name);
        }
        goto A;
        break;
        case 2:
        printf("\nCLIENT - The record above is incorrect please enter the correct patient information!!!!\n");
        //clearing the patient string variable
        for (int i = 0; i < strlen(patient); i++)
        {
            patient[i] = 0;
        }
        goto B;
        break;
        default:
        printf("\nCLIENT - Please enter a valid option.");
        goto H;
    }

    A: printf("\n\t\t---------Do you want to add another patient?-----------\n");
    int newPatient;
    D: printf("\n\tOption 1: Enter 1 to add another patient.\n\tOption 2: Enter 2 to quit adding another patient: ");
    scanf("%d", &newPatient);
    write(server_socket, &newPatient, sizeof(int));//sending the response to the server
    switch(newPatient){
        case 1:
        printf("\nCLIENT - User has asked to add a new patient!!!\n");
        //clearing the patient string variable
        for (int i = 0; i < strlen(patient); i++)
        {
            patient[i] = 0;
        }
        goto B;
        break;
        case 2:
        printf("\nCLIENT - User has chosen not to add a new patient!!!\n");
        goto C;
        break;

        default:
        printf("\nCLIENT - Please enter a valid option!!!\n");
        goto D;
    }

    C: printf("\n\t\t----------%s is ready for reading!!!----------\n", file_name);
    int readChoice;
    E: printf("\n\tOPtion 1: Enter 1 to get number of patients in %s file.\n\tOption 2: Enter 2 to get patient record in %s file by name. \n\tOption 3: Enter 3 to get patient record in %s file by date of identification: ", file_name, file_name, file_name);
    scanf("%d", &readChoice);
    write(server_socket,&readChoice, sizeof(int));//sending the user's choice to the server
    switch(readChoice){
        case 1:
        printf("\nCLIENT - Requested server to return the number of patients from %s file!!!\n", file_name);
        int lines;
        read(server_socket, &lines, sizeof(int));//reading the number of patients from the server
        if(lines == 1){
            printf("\n---------------------------------------------------------------------\n");
            printf("\nThere is %d patient in %s file!!!\n", lines, file_name);
            printf("\n---------------------------------------------------------------------\n");
        }else{
            printf("\n---------------------------------------------------------------------\n");
            printf("\nThere are %d patients in %s file!!!\n", lines, file_name);
            printf("\n---------------------------------------------------------------------\n");
        }
        break;

        case 2:
        printf("\nCLIENT - Requested server to return patient records by name!!!\n");
        char firstName[255];
        char lastName[255];
        printf("\nCLIENT - Enter patient's firstname: ");
        scanf("%s", firstName);
        printf("\nCLIENT - Enter patient's lastname: ");
        scanf("%s", lastName);
        if(write(server_socket, firstName, 255)){//sending the first name to the server
            printf("\nCLIENT - Patient's firstname has been sent to the server successfully!!!\n");
        }
        if(write(server_socket, lastName, 255)){//sending the lastname to the server
            printf("\nCLIENT - Patient's lastname has been sent to the server successfully!!!\n");
        }
        char records[1024];
        if(read(server_socket, records, 1024)){//reading the patient records from the server
            printf("\n\tThese are the patient Records as saved in the %s file.\n", file_name);
            printf("\n---------------------------------------------------------------------\n");
            printf("\n%s", records);
            printf("\n---------------------------------------------------------------------\n");
            for (int i = 0; i < strlen(records); i++)
            {
                records[i] = 0;
            }
        }
        break;

        case 3:
        printf("\nCLIENT - Requested server to return patient records by identification date!!!\n");
        char date[255];
        printf("\nCLIENT - Enter patient's identification date: ");
        scanf("%s", date);
        if(write(server_socket, date, 255)){//sending the date to the server
            printf("\nCLIENT - Patient identification date has been sent to the server!!\n");
        }
        char records2[1024];
        if(read(server_socket, records2, 1024)){//reding patient recors fro the server
            printf("\n\tThese are the patient Records as saved in the %s file.\n", file_name);
            printf("\n---------------------------------------------------------------------\n");
            printf("\n%s", records2);
            printf("\n---------------------------------------------------------------------\n");
            for (int i = 0; i < strlen(records2); i++)
            {
                records2[i] = 0;
            }
        }
        break;

        default:
        printf("\n-------Please enter a valid option.--------");
        goto E;
    }
    K: printf("\n------------PROGRAM CONTROL------------\n\n");
    printf("\t\tOption 1: Enter 1 to add a new patient.\n");
    printf("\t\tOption 2: Enter 2 to check out the file.\n");
    printf("\t\tOption 3: Enter 3 to quit the program.\n");
    int progControl;
    printf("\nCLIENT- Enter your desired option from the above: ");
    scanf("%d", &progControl);
    if(write(server_socket, &progControl, sizeof(int))){
        printf("\nCLIENT - user's desired option sent to the server succesfully.\n");
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
            goto E;
            break;

            case 3:
            printf("\n\t\tGOOD BYE!!!\t\t\n\n");
            goto Y;
            break;

            default:
            printf("\nCLIENT - Please enter a avalid option.\n");
            goto K; 
            break;
        }
    }else{
        printf("\nCLIENT - ERROR sending user's option to the server.\n");
    }
    //closing the connection  
    Y: close(server_socket);
    return 0;
}