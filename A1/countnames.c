/**
 * Description: This file counts the number of names in a given text file
 * Author Name: Roman Shpilberg
 * Author Emails: roman.shpilberg@sjsu.edu
 * Last Modified Date: 2/16/2023
 * Creation Date: 2/16/2023
 */

const int ROWS = 100;
const int COLS = 30

int main(int argc, char *argv[]){
    char names[ROWS][COLS];
    int nameCount[ROWS];

    printf("You have entered: %s", argv[1]);

    FILE *nameFile = fopen(argv[1], "r");

    // file error
    if(nameFile == NULL){
        printf("cannot open file\n");
        exit(1);
    }

    char nameCursor[COLS];
    if(fgets(nameCursor, COLS+1, nameFile)){

    }
}

/**
 *
 * @param list pointer to a 2d char array of names
 * @param name char[] of name to find
 * @return 0 if no name matched, 1 if matched
 */
int inList(char **list, char name[COLS]){
    for(int i=0; i<ROWS; i++){ // go through names
        for(int j=0; i<COLS; j++){

        }
    }
    return 0;
}