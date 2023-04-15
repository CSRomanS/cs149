#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NAME_LENGTH 100
#define NUMBER_LENGTH 14

typedef struct ContactNode_struct {
    char contactName[NAME_LENGTH];
    char contactPhoneNumber[NUMBER_LENGTH];
    struct ContactNode_struct* nextNodePtr;
} ContactNode;


void InsertAfter(struct ContactNode_struct* current, struct ContactNode_struct* next){
   next->nextNodePtr = current->nextNodePtr;
   current->nextNodePtr = next;
}


struct ContactNode_struct* InitializeContactNode(struct ContactNode_struct* what, char* inName, char* inNumber){
   struct ContactNode_struct* node =(struct ContactNode_struct*) malloc(sizeof(struct ContactNode_struct));
   strcpy(node->contactName, inName);
   strcpy(node->contactPhoneNumber, inNumber);
   if(what != NULL) InsertAfter(what, node);
   return node;
}


char* GetName(struct ContactNode_struct* inNode){return inNode->contactName;}
char* GetPhoneNumber(struct ContactNode_struct* inNode){return inNode->contactPhoneNumber;}
struct ContactNode_struct* GetNext(struct ContactNode_struct* inNode){return inNode->nextNodePtr;}


void PrintContactNode(struct ContactNode_struct* inNode, int number){
   printf("Person %d: %s, %s\n", number, GetName(inNode), GetPhoneNumber(inNode));
}


int main(void) {
   char name[NAME_LENGTH];
   char number[NUMBER_LENGTH];
   fgets(name, NAME_LENGTH , stdin);
   fgets(number, NUMBER_LENGTH , stdin);
   if(name[strlen(name)-1]=='\n') name[strlen(name)-1]='\0';
   if(number[strlen(number)-1]=='\n') number[strlen(number)-1]='\0';
   struct ContactNode_struct* contact1 = InitializeContactNode(NULL, name, number);
   
   fgets(name, NAME_LENGTH , stdin);
   fgets(number, NUMBER_LENGTH , stdin);
   if(name[strlen(name)-1]=='\n') name[strlen(name)-1]='\0';
   if(number[strlen(number)-1]=='\n') number[strlen(number)-1]='\0';
   struct ContactNode_struct* contact2 = InitializeContactNode(contact1, name, number);
   
   fgets(name, NAME_LENGTH , stdin);
   fgets(number, NUMBER_LENGTH , stdin);
   if(name[strlen(name)-1]=='\n') name[strlen(name)-1]='\0';
   if(number[strlen(number)-1]=='\n') number[strlen(number)-1]='\0';
   struct ContactNode_struct* contact3 = InitializeContactNode(contact2, name, number);
   
   struct ContactNode_struct* cursor = contact1;
   int count = 1;
   while(cursor != NULL){
      PrintContactNode(cursor, count);
      cursor = cursor->nextNodePtr;
      count++;
   }
   
   cursor = contact1;
   printf("\nCONTACT LIST\n");
   while(cursor != NULL){
      printf("Name: %s\n", GetName(cursor));
      printf("Phone number: %s\n\n", GetPhoneNumber(cursor));
      cursor = cursor->nextNodePtr;
   }
   
   free(contact1);
   free(contact2);
   free(contact3);
   
   return 0;
}