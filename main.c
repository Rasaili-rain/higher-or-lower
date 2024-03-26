#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

#define MAX_NAME_LENGTH 30
#define MAX_PERSONS 100
#define FILE_NAME "data.txt"
#define HIGH_SCORE "high_score.txt"

/**
 * structure contains name and followers 
*/
typedef struct{
    char name[MAX_NAME_LENGTH];
    int followers; // in millions
}Person;

bool foo_mode = false ; //only for demonstration


//Function Prototypes

int countQuestions();
void loadStoredQues(int, Person*);
void showQuestions(int, Person*);
void addQuestions(int *, Person*);
void playGame(int , Person*);
void replaceSpaceToUnderscore(char *);
void replaceUnderscoreToSpace(char *);
void printAscii(int);
int updateHighscore(int);


/**
 * switch between modes play, add questions , view question, exit
*/
int main() {
    int mode;
    Person s[MAX_PERSONS];
    int num_ques = countQuestions();
    loadStoredQues(num_ques, s);
    srand(time(NULL)); // Seed the random number generator

    // Main menu loop
    do {
        printAscii(0);
        printf("\n\nEnter:\n1 to play\n2 to add a question\n3 to view all questions\n4 to exit\n--> ");
        scanf("%d", &mode);
        system("clear"); // Clear console screen (for Windows, use "cls")

        switch (mode) {
            case 1:
                playGame(num_ques, s); // number of questions dont need any modification
                break;
            case 2:
                addQuestions(&num_ques, s);  //number of question needs modification
                break;
            case 3:
                showQuestions(num_ques, s);  // number of questions dont need any modification
                break;
            case 4:
                printf("Program terminated sucessfully.\n");
                exit(0);
            default:
                printf("Invalid mode. Please try again.\n");
        }
    } while (mode != 4); // Repeat until user chooses to exit

}


/**
 * woks by counting no of new line "/n" in the file
 * @returns no_of_questions 
*/
int countQuestions(){
    FILE *fp;
    char c;
    int count = 0;
    fp = fopen(FILE_NAME,"r");
    if (fp == NULL){
        printf("error oppening the file");
        exit(0);
    }
    while((c = fgetc(fp)) != EOF){
        if (c =='\n'){
            count += 1;
        }
    }

    fclose(fp);
    return count;
}


/**
 * reads the existing questions from a text file and writes in People structure
 * @param total_questions total no of questions
 * @param s refrence to the structure
*/
void loadStoredQues(int total_questions, Person* s) {
    FILE *fp;
    int curr_ques_no = 0;
    fp = fopen(FILE_NAME, "r");

    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }

    // Read data from the file line by line
    while (curr_ques_no < total_questions && fscanf(fp, "%s %d", s[curr_ques_no].name, &s[curr_ques_no].followers)) {
        curr_ques_no++;
    }

    // Close the file
    fclose(fp);

    // Process the loaded questions if needed
}


/**
 * shows all questions from the structure
 * @param num_ques.no of questions
 * @param s.refrence of the structure
*/
void showQuestions(int num_ques, Person* s) {
    if (num_ques == 0) {
        printf("No questions available.\n");
        return;
    }

    printf("Current Questions:\n");
    // Loop to display each question
    for (int i = 0; i < num_ques; i++) {
        replaceUnderscoreToSpace(s[i].name);
        printf("%d. Name: %s,   Followers: %d M\n", i + 1, s[i].name, s[i].followers);
    }
}


/**
 * appends more questions to file
 * appended questions are also added to the structure
 * @param num_ques. refrence to no of question 
 * @param s. reference to structure
*/
void addQuestions(int *num_ques, Person* s){
    int num;
    printf("Enter the number of questions you want to add: ");
    scanf("%d", &num);

    FILE *fp = fopen(FILE_NAME, "a"); // Open file for appending
    if (fp == NULL) {  
        printf("Error opening file.\n");
        return;
    }

    // Loop to input questions and write them to file
    for (int i = 0; i < num; i++) {
        printf("\nEnter the name of the person : ");
        scanf(" %[^\n]s", s[*num_ques].name);
        replaceSpaceToUnderscore(s[*num_ques].name);

        printf("Enter the number of Instagram followers (in millions): ");
        scanf("%d", &s[*num_ques].followers);
        (*num_ques)++;    //note numQuestions is a pointer and (*numQuestions)++ increases the actual value that the pointer points

        // Write data to file in a readable format
        fprintf(fp, "%s %d\n", s[*num_ques - 1].name, s[*num_ques - 1].followers);
    }

    fclose(fp); // Close the file
}


/**
 * replaces all " "(space) to "_"(underscore) in a string
 * @param str refrence of the string
*/
void replaceSpaceToUnderscore(char *str) {
    int length = strlen(str);
    for (int i = 0; i < length; i++) {
        if (str[i] == ' ') {
            str[i] = '_';
        }
    }
}


/**
 * replaces all "_"(underscore) to " "(space) in a string
 * @param str refrence of the string
*/
void replaceUnderscoreToSpace(char *str) {
    // Iterate through each character in the string
    for (int i = 0; str[i] != '\0' ; i++) {
        if (str[i] == '_') {
            str[i] = ' ';
        }
    }
}


/**
 * auctual mechanics of game
 * works by randomly selecting two elements from structure
 * if user guess whose followers is more score is increased
 * else game is over with a single wrong answer
 * @param no_of_ques total no of questions in structure
 * @param s refrence to the structure
*/
void playGame(int no_of_ques, Person* s) {
    // Check if there are questions available
    if (no_of_ques == 0) {
        printf("No questions available.\n");
        return;
    }

    bool game_is_on = true;
    int score = 0;

    while (game_is_on) {
        int high_score = updateHighscore(score);
        printf("\nCurrent score: %d\tall time high score : %d\n", score,high_score);

        // Generate two random indices for selecting questions
        int index1 = rand() % no_of_ques;
        int index2;
        do {
            index2 = rand() % no_of_ques;
        } while (index1 == index2); // Ensure index2 is different from index1

        // Determine the index of the winner based on followers count
        int winner_index = (s[index1].followers > s[index2].followers) ? index1 : index2;

        // Display the comparison between the two persons
        printf("\n\nWho has more followers?\n");
        replaceUnderscoreToSpace(s[index1].name);
        replaceUnderscoreToSpace(s[index2].name);
        if (foo_mode){
            printf("\n1. %s (Followers: %d M)\n", s[index1].name, s[index1].followers);
            printf("2. %s (Followers: %d M)\n", s[index2].name, s[index2].followers);
        }
        else{
            printf("1. %s \n", s[index1].name);
            printf("2. %s \n", s[index2].name);
        }

        int user_choice;
        do {
            printf("press 1 or 2 to answer\n9 to quit--> ");
            scanf("%d", &user_choice);

            if (user_choice == 9){
                game_is_on = false;
                break; //terminate game if user wishes
            }

            // Check if user input is valid
            if (user_choice != 1 && user_choice != 2) 
                printf("Invalid input. Please enter 1 or 2.\n");
            else {
                system("clear");
                //let user see the auctual no of followers
                printf("\nyou chose : %s \n",s[winner_index].name);
                printf("\n1. %s (Followers: %d M)\n", s[index1].name, s[index1].followers);
                printf("2. %s (Followers: %d M)\n", s[index2].name, s[index2].followers);
                
                // Check if user's choice matches the winner
                if ((user_choice == 1 && winner_index == index1) || (user_choice == 2 && winner_index == index2)) {
                    // Correct answer
                    printAscii(2);
                    score++;
                } 
                else {
                    // Incorrect answer
                    game_is_on = false;
                    printAscii(1);
                }
            }
        } while (user_choice != 1 && user_choice != 2); // Repeat until valid input

        updateHighscore(score);
    } // closing of while(game_is_on)

    // Display final score
    printf("\nGame over. Your final score is: %d\n", score);
}// closing of playGame()


/**
 * prints ascii give the mode
 * @param art 0->welcome, 1->game over, 2->correct
*/
void printAscii(int art){
    switch (art){
        case 0: //welcome
            printf("              _                          \n");
            printf("             | |                         \n");
            printf("__      _____| | ___ ___  _ __ ___   ___ \n");
            printf("\\ \\ /\\ / / _ \\ |/ __/ _ \\| '_ ` _ \\ / _ \\\n");
            printf(" \\ V  V /  __/ | (_| (_) | | | | | |  __/\n");
            printf("  \\_/\\_/ \\___|_|\\___\\___/|_| |_| |_|\\___|\n");
            return;

        case 1: //Game Over
            printf("  __ _  __ _ _ __ ___   ___    _____   _____ _ __ \n");
            printf(" / _` |/ _` | '_ ` _ \\ / _ \\  / _ \\ \\ / / _ \\ '__|\n");
            printf("| (_| | (_| | | | | | |  __/ | (_) \\ V /  __/ |   \n");
            printf(" \\__, |\\__,_|_| |_| |_|\\___|  \\___/ \\_/ \\___|_|   \n");
            printf("  __/ |                                            \n");
            printf(" |___/                                             \n");
            return;

        case 2: // correct
            printf("                              _    \n");
            printf("                             | |   \n");
            printf("  ___ ___  _ __ _ __ ___  ___| |_  \n");
            printf(" / __/ _ \\| '__| '__/ _ \\/ __| __| \n");
            printf("| (_| (_) | |  | | |  __/ (__| |_  \n");
            printf(" \\___\\___/|_|  |_|  \\___|\\___|\\__| \n");
            return;
    }

}


/**
 * updates the highscore in HIGH_SCORE file
 * @param current_score
 * @returns high_score
*/
int updateHighscore(int current_score){
    FILE *fp ;
    fp = fopen(HIGH_SCORE,"r");
    int high_score;
    fscanf(fp, "%d", &high_score);
    fclose(fp);
    if (current_score > high_score){
        high_score = current_score; 
        fp =fopen(HIGH_SCORE,"w");
        fprintf(fp,"%d", high_score);
        fclose(fp);
    }
    return high_score;
}

