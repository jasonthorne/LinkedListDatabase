# Advanced C project

## Jason Thorne. G00317349.

A database implemented as a linked list. Allowing users to retreive, add, delete, update and manipulate member accounts.

Each member stores the following details in a member structure:

* Member ID
* Member Name 
* Member Address
* Membership Type (Monthly or Yearly)
* Membership rate (Monthly - €50 Yearly - €500)
* Date of Joining (day/month/year)
* Email Address 


The database is password protected and only the correct username and password details will allow the user access. The username and password are read in from Login.txt. Password input is hidden from the screen by showing an * for each character typed.


The program provides the following menu:

1. Add member
2. Display member details
3. Update member details
4. Delete member
5. Display all member by membership type
6. Member Report


#### Add member ####
This will add a new member at the correct position based on their ID number in the
sorted linked list. 

#### Display member Details ####
Allows the user to input either a Member ID or a name and display the
details for that member.

#### Update member Details ####
Allows the user to update the address, membership type or email address
of a member based on either a name or ID being entered.

#### Delete member ####
Allows the user to delete a member from the list by ID number.

#### Display all members ####
Displays all employees by membership type.

#### Member Report ####
Displays a member report which outputs the following information by membership
type:

- Membership Type
- Number of members
- Total income from these types of members per annum
- How many of these members have been for more than 2 years.

This report also writes to MemberReport.txt for future reading.
