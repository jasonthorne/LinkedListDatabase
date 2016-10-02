/*
============================================================================
Name           : Advanced C Project
Author         : Jason Thorne
Student Number : G00317349
Description    : Advanced Procedural Programming Assignment
============================================================================
*/


//LIBRARIES

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

//----------------------------------------------------------
//CONSTANTS

//sizes for member variables
#define MAX_NAME  30
#define MAX_ADDRESS 60
#define MAX_TYPE 1
#define MAX_EMAIL 30
#define MAX_PASSWRD 7

//membership rates
#define MONTHLY_RATE 50
#define YEARLY_RATE 500

//for checking membership length
#define MEMBERSHIP_LENGTH 2

//days in a month
#define DAYS_31 31
#define DAYS_30 30

//visual studio strcasecmp fix
#if defined(WIN32) || defined(WIN64)
#define strcasecmp _stricmp
#endif 

//----------------------------------------------------------
//GLOBALLY DEFINED STRUCTS

//date struct
struct date
{
	int day;
	int month;
	int year;
};

//login struct
struct login
{
	char name[MAX_NAME];
	char passWrd[MAX_PASSWRD];
	struct login *next;
};

struct login *l_first = NULL;;
struct login *l_current = NULL;;
struct login *l_new = NULL;;

//member struct
struct member
{
	int id;
	char name[MAX_NAME];
	char address[MAX_ADDRESS];
	char type[MAX_TYPE];
	int rate;
	struct date doj;
	char email[MAX_EMAIL];
	struct member *next;
};


struct member *m_first = NULL;
struct member *m_current = NULL;
struct member *m_new = NULL;
struct member *next = NULL;

//----------------------------------------------------------
//GLOBALLY DEFINED VARIABLES

//for user logging into database
char usrName[MAX_NAME];
char usrPassWrd[MAX_PASSWRD];

//for checking if methods have been visited.
int visited_MakeLoginList = 0;
int visited_UsrLoginInput = 0;

//for user entering input
int usrIsChoosing = 1;

//for validating user input
int inputNum = 0;
char inputChar[MAX_NAME];

//for node index positions
int newIndex = 0;

//holds current year.
int currentYear = 0;

//for length of feb
int feb = 0;

//member variables for user input
int newId = 0;
char newName[MAX_NAME] = "*";
char newAddress[MAX_ADDRESS] = "*";
char newType[MAX_TYPE] = "*";
int newRate = 0;
int newDay = 0;
int newMonth = 0;
int newYear = 0;
char newEmail[MAX_EMAIL] = "*";

//----------------------------------------------------------
//FUCTION PROTOTYPES

void usrLoginInput(void);
struct login *makeLoginNode(void);
void makeLoginList(void);
void fillLoginNode(struct login *logData, FILE *fp);
int searchLoginList(struct login *first);
void makeMemberList(void);
struct member *makeMemberNode(void);
void fillMemberNode(struct member *individual, FILE *fp);
void menu(void);
struct member *displayMember(struct member *first);
void printMember(struct member individual);
void updateMember(struct member *individual);
void findIndex(void);
void displayAllMembers(struct member *first);
void updateMembersFile(void);
void memberReport(struct member *first);
int findYear(void);
int checkForLeapYear(void);
void deleteMember(struct member *first);
void addMember(void);
void addId(void);
void addName(void);
void addAddress(void);
void addType(void);
void addDate(void);
void addEmail(void);
void insertNode(struct member **first);
void removeNode(struct member **first);
void fillFirstNode(struct member *first);
//----------------------------------------------------------
//METHODS

//main method
int main(void)
{

	printf("\n                       ____________ABC_GYM____________\n\n");
	printf("                              MEMBERS DATABASE\n\n");

	makeLoginList();
	makeMemberList();
	menu();


	return EXIT_SUCCESS;
}//end main

//----------------------------------------------------------
//takes login input
void usrLoginInput(void)
{

	printf("Enter Username and Password\n\n");

	//ask for name
	printf("Name: ");
	fflush(stdout);
	scanf("%s", &usrName);

	printf("\n");

	//ask for password
	printf("Password: ");

	char ch;
	int i;

	//hide password input
	for(i=0;i<MAX_PASSWRD-1;i++)
	{

		fflush(stdout);
		ch = getch();  //get first character entered for password
		usrPassWrd[i] = ch;  //insert into array
		ch = '*' ;  //change value of ch to '*'
		printf("%c",ch);  //print ch to screen

	}//for
	
	printf("\n\n");


	//check if method visited already
	if (visited_UsrLoginInput == 1)
	{

		//return to this method if so
		makeLoginList();

	}//if

	//change value to show method has been visited
	visited_UsrLoginInput = 1;

}//method

//----------------------------------------------------------
//makes login list
void makeLoginList(void)
{

	FILE *fp; // pointer to 'Login.txt'

	if ((fp = fopen("Login.txt", "r")) == NULL) //check for file error
	{
		puts("\nError: Login file could not be found"); //error message
		exit(1); //exit program

	}//if
	else //read data from file
	{

		//check If method visited already
		if (visited_MakeLoginList == 0)
		{
			//change value to show method has been visited
			visited_MakeLoginList = 1;

			//prompt user for name and password.
			usrLoginInput();

			//create first node
			l_first = makeLoginNode();

			//make current node = first node
			l_current = l_first;

			//fill current node
			fillLoginNode(l_current, fp);


			while (!feof(fp)) // keep looping until end of file
			{

				//make new node
				l_new = makeLoginNode();

				//make 'next' in current point to new
				l_current->next = l_new;

				//make current node = new
				l_current = l_new;

				//fill current node
				fillLoginNode(l_current, fp);

			}//while

		}//if

		fclose(fp); //close pointer

		//search list for users input
		searchLoginList(l_first);

	}//else

}//method

//----------------------------------------------------------
//allocates storage for one new login struct
struct login *makeLoginNode(void)
{
	//declare child
	struct login *child;

	//assign memory to child
	child = (struct login*)malloc(sizeof(struct login));

	//check for memory error
	if (child == NULL)
	{
		puts("Memory error\n");
		exit(1);

	}//if

	//return child
	return(child);

}//method

//----------------------------------------------------------
//fills login struct with data from file
void fillLoginNode(struct login *logData, FILE *fp)
{
	fscanf(fp, "%s%s", &logData->name, &logData->passWrd);
	logData->next = NULL;

}//method

//----------------------------------------------------------
//searches login list for username and password
int searchLoginList(struct login *first)
{

	//make temp node
	struct login *temp;

	//make temp a copy of first
	temp = first;

	while (temp != NULL)
	{

		//check if password and username match
		if ((strcmp(temp->name, usrName) == 0) && (strcmp(temp->passWrd, usrPassWrd) == 0))
		{
			printf("Account found\n"); //inform user if found
			return(0); //return to main
		}//if

		//move temp to next
		temp = temp->next;

	}//while

	//inform user that not found
	printf("\nError: Username or Password is incorrect.\n");

	usrLoginInput(); //prompt user to re-enter input

	return(0); //return to main

}//method

//----------------------------------------------------------
//makes members list
void makeMemberList(void)
{

	//pointer to 'Members.txt'
	FILE *fp;

	//check for file error
	if ((fp = fopen("Members.txt", "r")) == NULL)
	{
		puts("Error: Members file could not be found\n"); //error message
		exit(1); //exit program
	}
	else //read data from file
	{
		//create first node
		m_first = makeMemberNode();

		//fill first node
		fillFirstNode(m_first);

		fillMemberNode(m_first, fp);

		//make current = first
		m_current = m_first;


		while (!feof(fp)) // keep looping until end of file
		{

			//make new node
			m_current->next = makeMemberNode();

			//make current node = new
			m_current = m_current->next;

			//fill current node
			fillMemberNode(m_current, fp);

		}//while

		fclose(fp); //close pointer

	}//else

}//method

//----------------------------------------------------------
//allocates storage for one new member struct
struct member *makeMemberNode(void)
{
	//declare child
	struct member *child;

	//assign memory to child
	child = (struct member*)malloc(sizeof(struct member));

	//check for memory error
	if (child == NULL)
	{
		puts("Memory error\n");
		exit(1);

	}//if

	//return child
	return(child);

}//method

//----------------------------------------------------------
//fills first node with default member data
void fillFirstNode(struct member *first)
{

	//add data to first node
	first->id = newId;
	strcpy(first->name, newName);
	strcpy(first->address, newAddress);
	strcpy(first->type, newType);
	first->rate = newRate;
	first->doj.day = newDay;
	first->doj.month = newMonth;
	first->doj.year = newYear;
	strcpy(first->email, newEmail);
	first->next = NULL;

}//method

//----------------------------------------------------------
//fills member struct with data from file
void fillMemberNode(struct member *individual, FILE *fp)
{

	fscanf(fp, "%d%s%s%s%d%d%d%d%s",
		&individual->id,
		&individual->name,
		&individual->address,
		&individual->type,
		&individual->rate,
		&individual->doj.day,
		&individual->doj.month,
		&individual->doj.year,
		&individual->email);

	individual->next = NULL;

}//method

//----------------------------------------------------------
//user menu
void menu(void)
{

	printf("Welcome to menu");

	do
	{
		//show menu
		printf("\n\n");
		printf("Which option would you like?\n\n");
		printf("1 - Add a new Member\n");
		printf("2 - Display Member details\n");
		printf("3 - Update Member\n");
		printf("4 - Delete Member\n");
		printf("5 - Display all Members by membership type\n");
		printf("6 - Member report\n");
		printf("7 - Exit\n");

		do
		{
			//ask for input
			printf("Please enter a number of your choice: \n");
			fflush(stdout);
			scanf("%s", &inputChar);

			//convert input to int
			inputNum = atoi(inputChar);

			//check for invalid input (string or out of range int)
			if (inputNum == 0 || inputNum > 7)
			{
				//inform user
				printf("Error: Invalid entry\n");

			}//if
			else //valid input given
			{
				//leave do while
				break;

			}//else

		}//do
		while (usrIsChoosing == 1);

		//reset variable
		usrIsChoosing = 1;

		printf("\n\n");

		switch (inputNum)
		{
		case 1: //add a new Member
			printf("ADD MEMBER\n\n");
			addMember();
			break;
		case 2: //display Member details
			printf("DISPLAY MEMBER\n\n");
			m_current = displayMember(m_first);
			printMember(*m_current);
			break;
		case 3: //update Member
			printf("UPDATE MEMBER\n\n");
			m_current = displayMember(m_first);
			updateMember(m_current);
			break;
		case 4: //delete Member
			printf("DELETE MEMBER\n\n");
			deleteMember(m_first);
			break;
		case 5: //display all Members by membership type
			displayAllMembers(m_first);
			break;
		case 6: //member report
			printf("MEMBER REPORT\n\n");
			memberReport(m_first);
			break;
		case 7: //exit
			printf("Goodbye\n");
			exit(1);
			break;
		default:
			printf("Error: Invalid entry\n"); //invalid entry
			break;
		}//switch

	}//do
	while (inputNum != 7);

}//method

//----------------------------------------------------------
//adds a new member to database
void addMember(void)
{

	//ask for id
	addId();

	//ask for name
	addName();

	//ask for address
	addAddress();

	//ask for type (also adds rate)
	addType();

	//ask for date of joining
	addDate();

	//ask for email address
	addEmail();

	//find where to insert new node
	findIndex();

	//insert new node
	insertNode(&m_first);

	//inform user
	printf("Member added\n");

	//update file
	updateMembersFile();


}//method

//----------------------------------------------------------
//removes a member from database
void deleteMember(struct member *first)
{

	//make temp node
	struct member *temp;

	//ask user for ID and check if already exists. Ask again if not.
	do
	{
		//reset temp
		temp = first;

		do
		{
			//ask for id
			printf("Enter member id: \n");
			fflush(stdout);
			scanf("%s", &inputChar);

			//convert to int
			newId = atoi(inputChar);

			//check for invalid input (string or '0' was given)
			if (newId == 0)
			{
				//inform user
				printf("Error: Invalid entry\n");

			}//if
			else //valid input given
			{
				//leave do while
				break;

			}//else

		}//do
		while (usrIsChoosing == 1);

		//reset variable
		usrIsChoosing = 1;


		while (temp != NULL)
		{

			//if member found
			if (temp->id == newId)
			{

				//inform user
				printf("Member found\n");

				//break out of do while
				usrIsChoosing = 0;
				break;

			}//if
			else
			{

				//move temp to next node
				temp = temp->next;

				//id not found
				if (temp->next == NULL)
				{

					//inform user
					printf("Error: ID not in use\n");

					//Choose again
					break;

				}//if

			}//else

		}//while

	}//do
	while (usrIsChoosing == 1);

	//reset variable
	usrIsChoosing = 1;

	//find where to remove node
	findIndex();

	//remove node
	removeNode(&m_first);

	//inform user
	printf("Member removed\n");

	//update file
	updateMembersFile();

}//method

//----------------------------------------------------------
//finds index of
void findIndex(void)
{
	//make temp node
	struct member *temp;

	temp = m_first;

	//index of target node
	int index = 0;

	//find index position to insert new node based on id

	while (temp != NULL)
	{

		if (newId > temp->id)
		{
			index++; //add to index

		}//if


		//move temp to next
		temp = temp->next;

	}//while

	//assign value
	newIndex = index;

}//method

//----------------------------------------------------------
//inserts a node into linked list of members
void insertNode(struct member **first)
{

	//make temp node
	struct member *newMember;
	newMember = makeMemberNode();

	//add data to temp
	newMember->id = newId;
	strcpy(newMember->name, newName);
	strcpy(newMember->address, newAddress);
	strcpy(newMember->type, newType);
	newMember->rate = newRate;
	newMember->doj.day = newDay;
	newMember->doj.month = newMonth;
	newMember->doj.year = newYear;
	strcpy(newMember->email, newEmail);
	newMember->next = NULL;

	//if first index need replaced
	if (newIndex == 1)
	{
		newMember->next = *first;

		*first = newMember;
		return;
	}//if

	//Walk through list and retrieve the very last node
	struct member *lastNode = *first;
	for (int i = 0; i<newIndex - 2; i++)
	{
		lastNode = lastNode->next;
	}//for

	//create a temp member, giving it the memory after last node
	struct member *temp;
	temp = lastNode->next;
	//assign  newly populated member to that memory (after last node)
	lastNode->next = newMember;
	//assign the value held in temp to be the next node after newly inserted node
	newMember->next = temp;

}//method

//----------------------------------------------------------
//removes a node from linked list of members
void removeNode(struct member **first)
{
	//temp variable made, pointed to head
	struct member *temp1 = m_first;

	if (newIndex == 1)
	{
		m_first = temp1->next; //m_first now points to second node.
		free(temp1); //free temp1
		return;
	}//if

	//get the node to be removed based on the supplied index
	for (int i = 0; i < newIndex - 2; i++)
	{
		temp1 = temp1->next; //temp points to the node before the node to delete
	}//for

	//get the node to be deleted
	struct member *nodeToDelete = temp1->next; 

	//get the node after the node to be deleted
	struct member *nodeAfterNodeToDelete = nodeToDelete->next;

	free(nodeToDelete); //free nodeToDelete

	//assign the node after temp to the node after the one just removed (re-linking the list)
	temp1->next = nodeAfterNodeToDelete;

}//method

//----------------------------------------------------------
//displays details for a chosen member
struct member *displayMember(struct member *first)
{
	//make temp node
	struct member *temp;

	//make temp a copy of first
	temp = first;

	do
	{
		//ask for input
		printf("Enter Member's Name or ID: ");
		fflush(stdout);
		scanf("%s", &inputChar);

		//convert to int
		int inputNum = atoi(inputChar);

		if (inputNum == 0) //if 0, then a name was given
		{

			while (temp->next != NULL)
			{

				if (strcasecmp(temp->name, inputChar) == 0)//check if name is found
				{
					//return node if found
					return (temp);
				}//if

				temp = temp->next; //temp becomes next node

			}//while

			if (temp->next == NULL) //if name not in list
			{
				//inform user
				printf("Error: Name not found\n");

				//reset temp
				temp = first;

			}//if

		}//if
		else //else a number (id) was given
		{

			while (temp->next != NULL)
			{

				if (temp->id == inputNum) //check if id is found
				{
					//return node if found
					return (temp);
				}//if

				temp = temp->next; //temp becomes next node

			}//while

			if (temp->next == NULL) //if id not in list
			{
				//inform user
				printf("Error: ID not found\n");

				//reset temp
				temp = first;

			}//if

		}//else

	}//do
	while (usrIsChoosing == 1);

	//reset variable
	usrIsChoosing = 1;


	//return
	return(0);

}//method

//----------------------------------------------------------
//prints data of a member to screen
void printMember(struct member individual)
{

	printf("\nMember details:\n\n");
	printf("Id: %d\n", individual.id);
	printf("Name: %s\n", individual.name);
	printf("Address: %s\n", individual.address);
	printf("Type: %s\n", individual.type);
	printf("Join date: %02d%s%02d%s%-5d\n", individual.doj.day, "/", individual.doj.month, "/", individual.doj.year);
	printf("Email: %s\n", individual.email);


}//method

//----------------------------------------------------------
//updates member information
void updateMember(struct member *individual)
{


	do
	{
		//show menu
		printf("\n\n");
		printf("What would you like to update?\n\n");
		printf("1 - Address\n");
		printf("2 - Membership type\n");
		printf("3 - Email Address\n");
		printf("4 - Exit Menu\n");


		do
		{
			//ask for input
			printf("Please enter number of your choice: \n");
			fflush(stdout);
			scanf("%s", &inputChar);

			//convert input to int
			inputNum = atoi(inputChar);

			//check for invalid input (string or out of range int)
			if (inputNum == 0 || inputNum > 4)
			{
				//inform user
				printf("Error: Invalid entry\n");

			}//if
			else //valid input given
			{
				//leave do while
				break;

			}//else

		}//do
		while (usrIsChoosing == 1);

		//reset variable
		usrIsChoosing = 1;


		if (inputNum == 1)
		{
			//ask for address
			printf("Enter new Address: \n");
			fflush(stdout);
			scanf("%s", &individual->address);


			//inform user of change
			printf("Address updated\n");

			//update file with changes
			updateMembersFile();

		}//if
		else if (inputNum == 2)
		{

			do
			{
				//ask for membership type
				printf("Enter new Membership Type\n'M' for monthly, or 'Y' for yearly: \n");
				fflush(stdout);
				scanf("%s", &inputChar);

				//convert to int
				inputNum = atoi(inputChar);

				//check for invalid input (integer was given)
				if (inputNum > 0)
				{
					printf("Error: Invalid entry\n"); //inform user

				}//if
				else //string given
				{

					//check that 'M' was entered
					if (strcasecmp("M", inputChar) == 0) //'strcasecmp' ignores case
					{
						//make uppercase to match file
						strcpy(inputChar, "M");

						//fill node with new type
						strcpy(individual->type, inputChar);

						//fill node with new rate
						individual->rate = MONTHLY_RATE;

						//inform user
						printf("Membership type updated\n");

						//update file with changes
						updateMembersFile();

						//leave do while
						break;

					}//if
					else if (strcasecmp("Y", inputChar) == 0) //check if 'Y' was entered
					{
						//make uppercase to match file
						strcpy(inputChar, "Y");

						//fill node with new type
						strcpy(individual->type, inputChar);

						//fill node with new rate
						individual->rate = YEARLY_RATE;

						//inform user
						printf("Membership type updated\n");

						//update file with changes
						updateMembersFile();

						//leave do while
						break;

					}//else
					else //invalid input
					{
						//inform user
						printf("Error: Invalid entry\n");

					}//else

				}//else

			}//do
			while (usrIsChoosing == 1);

			//reset variable
			usrIsChoosing = 1;

		}//else if
		else if (inputNum == 3)
		{

			do
			{
				//ask for email
				printf("Enter Member Email: \n");
				fflush(stdout);
				scanf("%s", &inputChar);

				//convert to int
				inputNum = atoi(inputChar);

				//check for invalid input (int given)
				if (inputNum > 0)
				{
					//inform user
					printf("Error: Invalid entry\n");

				}//if
				else //string given
				{

					//check if valid email is given (contains '@' and '.com')
					if ((strstr(inputChar, "@") != NULL) && (strstr(inputChar, ".com") != NULL))
					{

						//fill node with new email.
						strcpy(individual->email, inputChar);

						//inform user
						printf("Email Address updated");

						//update file with changes
						updateMembersFile();

						//leave do while
						break;

					}//if
					else //invalid email given
					{
						printf("Error: Invalid email\n"); //inform user

					}//else

				}//else

			}//do
			while (usrIsChoosing == 1);

			//reset variable
			usrIsChoosing = 1;

		}//else if

	}//do
	while (inputNum != 4);


}//method

//----------------------------------------------------------
//displays all employees by membership type
void displayAllMembers(struct member *first)
{

	//make temp node
	struct member *temp;

	//make temp a copy of first
	temp = first;

	///print all monthly members
	printf("MONTHLY MEMBERS:\n\n");

	while (temp != NULL)
	{
		//check membership type is monthly
		if (strcasecmp("M", temp->type) == 0)
		{
			printf("%s\n", temp->name);

		}//if

		//move temp to next node
		temp = temp->next;

	}//while


	//reset temp
	temp = first;

	printf("\n______________\n\n");

	//print all yearly members
	printf("YEARLY MEMBERS:\n\n");

	while (temp != NULL)
	{
		//check membership type is monthly
		if (strcasecmp("Y", temp->type) == 0)
		{
			printf("%s\n", temp->name);
		}//if

		//move temp to next node
		temp = temp->next;

	}//while

}//method

//----------------------------------------------------------
//prints a member report to screen and file (MemberReport.txt)
void memberReport(struct member *first)
{

	//make temp node
	struct member *temp;

	//make temp a copy of first
	temp = first;

	//counters for member types
	int mCount = 0;
	int yCount = 0;


	//get current year
	currentYear = findYear();

	//counter for members from target year
	int targetYear = 0;


	while (temp->next != NULL)
	{
		if (strcasecmp("M", temp->type) == 0) //if type is monthly
		{
			mCount++; //add to monthly counter

		}//if
		else if (strcasecmp("Y", temp->type) == 0) //if type is yearly
		{
			yCount++; //add to yearly counter

		}//else

		//check if membership has been at least target membership length
		if ((currentYear - temp->doj.year) >= MEMBERSHIP_LENGTH)
		{
			targetYear++; //add to counter
		}//if

		//move temp to next node
		temp = temp->next;

	}//while


	//totals from each member type per annum
	int mAnnum = ((MONTHLY_RATE *mCount) * 12);
	int yAnnum = (YEARLY_RATE *yCount);


	FILE *fp; //pointer to MemberReport.txt

	//check for file error
	if ((fp = fopen("MemberReport.txt", "wb")) == NULL)
	{
		puts("\nError: MemberReport file could not be found"); //error message
		exit(1); //exit program

	}//if
	else //print report to file
	{

		fprintf(fp, "MEMBER REPORT\n\n");
		fprintf(fp, "Total Monthly members: %d\n", mCount);
		fprintf(fp, "Total Yearly members: %d\n", yCount);
		fprintf(fp, "Total Monthly income per annum: %d\n", mAnnum);
		fprintf(fp, "Total Yearly income per annum: %d\n", yAnnum);
		fprintf(fp, "Memberships greater than 2 years: %d\n", targetYear);

		//close pointer
		fclose(fp);

	}//else


	//print report to screen
	printf("Total Monthly members: %d\n", mCount);
	printf("Total Yearly members: %d\n", yCount);
	printf("Total Monthly income per annum: %d\n", mAnnum);
	printf("Total Yearly income per annum: %d\n", yAnnum);
	printf("Memberships greater than 2 years: %d\n", targetYear);



}//method
//----------------------------------------------------------
//updates file holding data of members (Members.txt)
void updateMembersFile(void)
{
	FILE *fp; //pointer to Members.txt

	//check for file error
	if ((fp = fopen("Members.txt", "w")) == NULL)
	{
		puts("\nError: Members file could not be found"); //error message
		exit(1); //exit program

	}//if
	else
	{

		//make temp node
		struct member *temp;
		//temp = makeMemberNode();

		//make temp a copy of first
		temp = m_first;


		while (temp->next != NULL)
		{

			//print struct data to file
			fprintf(fp, "%d ", temp->id);
			fprintf(fp, "%s ", temp->name);
			fprintf(fp, "%s ", temp->address);
			fprintf(fp, "%s ", temp->type);
			fprintf(fp, "%d ", temp->rate);
			fprintf(fp, "%02d ", temp->doj.day);
			fprintf(fp, "%02d ", temp->doj.month);
			fprintf(fp, "%d ", temp->doj.year);
			fprintf(fp, "%s\n", temp->email);

			//move temp to next node
			temp = temp->next;

		}//while

		//close pointer
		fclose(fp);

	}//else

}//method

//----------------------------------------------------------
//finds the current year
int findYear(void)
{
	//find current time
	time_t now;
	time(&now);

	//find local current time
	struct tm *localNow;
	localNow = localtime(&now);

	//take current year from current time.
	char year[5];
	strftime(year, 5, "%Y", localNow);

	//convert year to int
	int currentYear = atoi(year);

	//return value of currentYear
	return currentYear;

}//method

//----------------------------------------------------------
//finds value of feb based on the current year
int checkForLeapYear(void)
{

	//check for leap year for feb
	if (currentYear % 400 == 0)
	{
		feb = 29;
	}//if
	else if (currentYear % 100 == 0)
	{
		feb = 28;
	}//else
	else if (currentYear % 4 == 0)
	{
		feb = 29;
	}//else
	else
	{
		feb = 28;
	}//else

	//return value of feb
	return feb;

}//method

//----------------------------------------------------------
//takes in id for new member
void addId(void)
{
	//integers valid, strings invalid

	//make temp node
	struct member *temp;

	//ask user for ID and check if already exists. Ask again if so.
	do
	{
		//reset temp
		temp = m_first;

		do
		{
			//ask for id
			printf("Enter member id: \n");
			fflush(stdout);
			scanf("%s", &inputChar);

			//convert to int
			newId = atoi(inputChar);

			//check for invalid input (string or '0' was given)
			if (newId == 0)
			{
				//inform user
				printf("Error: Invalid entry\n");

			}//if
			else //valid input given
			{
				//leave do while
				break;

			}//else

		}//do
		while (usrIsChoosing == 1);

		//reset variable
		usrIsChoosing = 1;

		while (temp != NULL)
		{

			//check if found
			if (temp->id == newId)
			{
				//inform user
				printf("Error: Id already in use\n");

				//Choose again
				break;
			}//if
			else
			{
				//id not found
				if (temp->next == NULL)
				{
					//break out of do while
					usrIsChoosing = 0;
					break;
				}//if

				//move temp to next node
				temp = temp->next;

			}//else

		}//while

	}//do
	while (usrIsChoosing == 1);

	//reset variable
	usrIsChoosing = 1;

}//method

//----------------------------------------------------------
//takes in name for new member
void addName(void)
{
	//strings valid, integers invalid

	do
	{
		//ask for name
		printf("Enter Member Name: \n");
		fflush(stdout);
		scanf("%s", &inputChar);

		//convert to int
		inputNum = atoi(inputChar);

		//check for invalid input (int was given)
		if (inputNum > 0)
		{
			printf("Error: Invalid entry\n"); //inform user

		}//if
		else //valid input given
		{
			strcpy(newName, inputChar); //assign input as name
			break; //leave do while

		}//else

	}//do
	while (usrIsChoosing == 1);

	//reset variable
	usrIsChoosing = 1;


}//method

//----------------------------------------------------------
//takes in address for new member
void addAddress(void)
{

	//strings valid, integers valid

	//ask for address
	printf("Enter Member Address: \n");
	fflush(stdout);
	scanf("%s", &newAddress);

}//method

//----------------------------------------------------------
//takes in membership type for new member
void addType(void)
{

	//strings 'm' or 'y' valid, integers invalid

	do
	{
		//ask for membership type
		printf("Enter Membership Type. (M)onthly or (Y)early: \n");
		fflush(stdout);
		scanf("%s", &inputChar);

		//convert to int
		inputNum = atoi(inputChar);

		//check for invalid input
		if (inputNum > 0)
		{
			printf("Error: Invalid entry\n"); //inform user

		}//if
		else //valid input given
		{

			strcpy(newType, inputChar); //assign input as type

			//check that 'M' was entered
			if (strcasecmp("M", newType) == 0) //'strcasecmp' ignores case
			{

				strcpy(newType, "M"); //make uppercase to match file

				newRate = MONTHLY_RATE; //assign rate as monthly
				break; //leave do while

			}//if
			else if (strcasecmp("Y", newType) == 0) //check if 'Y' was entered
			{

				strcpy(newType, "Y"); //make uppercase to match file

				newRate = YEARLY_RATE; //assign rate as yearly
				break; //leave do while

			}//else
			else //invalid input
			{
				printf("Error: Invalid entry\n"); //inform user

			}//else

		}//else

	}//do
	while (usrIsChoosing == 1);

	//reset variable
	usrIsChoosing = 1;

}//method

//----------------------------------------------------------
//takes in date of joining for new member
void addDate(void)
{

	//integers valid, strings invalid

	//find current year
	currentYear = findYear();

	//check for leap year
	feb = checkForLeapYear();

	//variables for user entering input
	int usrIsChoosingDay = 1;
	int usrIsChoosingMnth = 1;
	int usrCanChooseYr = 0;
	int usrIsChoosingYr = 1;


	do
	{
		//reset variable
		usrIsChoosingMnth = 1;

		//ask for day of join
		printf("Enter Day: \n");
		fflush(stdout);
		scanf("%s", &inputChar);

		//convert to int
		newDay = atoi(inputChar);

		//check for invalid input (string or out range int)
		if (newDay == 0 || newDay > DAYS_31)
		{
			printf("Error: Invalid entry\n"); //inform user

		}//if
		else //valid input given (for now!)
		{

			do
			{
				//ask for month of join
				printf("Enter Month: \n");
				fflush(stdout);
				scanf("%s", &inputChar);

				//convert to int
				newMonth = atoi(inputChar);

				//check for invalid input (string or out of range int)
				if (newMonth == 0 || newMonth > 12)
				{
					printf("Error: Invalid entry\n"); //inform user

				}//if
				else
				{

					switch (newMonth)
					{

					case 2: //feb

						if (newDay > feb) //check feb for valid day input
						{
							printf("Error: Day entry was to high\n"); //inform user
							usrIsChoosingMnth = 0; //break back to entering day.
						}//if
						else
						{
							usrCanChooseYr = 1; //allow user to choose year
						}//else
						break; //break from switch

					case 4: //april
					case 6:	//jun
					case 9:	//sep
					case 11: //nov

						if (newDay > DAYS_30) //check 30 day months for valid input
						{
							printf("Error: Day entry was to high\n"); //inform user
							usrIsChoosingMnth = 0; //break back to entering day.
						}//if
						else
						{
							usrCanChooseYr = 1; //allow user to choose year
						}//else
						break; //break from switch

					default: //other days of the month were chosen

						usrCanChooseYr = 1; //allow user to choose year (day already confirmed as valid)
						break; //break from switch

					}//switch


					if (usrCanChooseYr == 1) //if day and month are valid
					{
						do
						{
							//ask for year of join
							printf("Enter Year: \n");
							fflush(stdout);
							scanf("%s", &inputChar);

							//convert to int
							newYear = atoi(inputChar);

							//check for invalid input (string or out of range int)
							if (newYear < currentYear)
							{
								printf("Error: Invalid entry\n"); //inform user

							}//if
							else //input is valid
							{
								//break from all whiles
								usrIsChoosingDay = 0;
								usrIsChoosingMnth = 0;
								usrIsChoosingYr = 0;

							}//else

						}//do
						while (usrIsChoosingYr == 1);

					}//if

				}//else

			}//do
			while (usrIsChoosingMnth == 1);

		}//else

	}//do
	while (usrIsChoosingDay == 1);

}//method

//----------------------------------------------------------
//takes in email address for new member
void addEmail(void)
{

	//strings invalid, integers valid

	do
	{
		//ask for email
		printf("Enter Member Email: \n");
		fflush(stdout);
		scanf("%s", &inputChar);

		//convert to int
		inputNum = atoi(inputChar);

		//check for invalid input (int given)
		if (inputNum > 0)
		{
			printf("Error: Invalid entry\n"); //inform user

		}//if
		else //string given
		{
			strcpy(newEmail, inputChar); //assign input as email

			//check if valid email is given (contains '@' and '.com')
			if ((strstr(newEmail, "@") != NULL) && (strstr(newEmail, ".com") != NULL))
			{
				break; //leave do while

			}//if
			else //invalid email given
			{
				printf("Error: Invalid email\n"); //inform user

			}//else

		}//else

	}//do
	while (usrIsChoosing == 1);

	//reset variable
	usrIsChoosing = 1;

}//method

//----------------------------------------------------------

//-------------END OF PROGRAM-------------

