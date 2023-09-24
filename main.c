/******************************************************************************
*******************************************************************************
********************* Online Doctor Appointment System ************************
*******************************************************************************
*******************************************************************************
*********** Developed By : Sushil Kumar ***************************************
*******************************************************************************
*******************************************************************************
********** This Program deals with different Data Structures        ***********
**********   1. Heap Priority Queue implemented with Array          ***********
**********   2. Doubly Linked list, for Patient's details           ***********
*******************************************************************************
*******************************************************************************
*******************************************************************************
*** Dr. Kumar runs a hospital for senior citizens which sees a large number ***
*** of patients coming in everyday. In order to avoid inconvenience to the  ***
*** aged patients, the rule is that the oldest patient is seen first by     ***
*** the doctor. As the patients keep coming and registering, they are added ***
*** to a priority list from which patient names are taken out for           ***
*** consultation; an appointment software has to be developed. This software***
*** will use Heaps as data structures to keep track of incoming patients and***
*** prioritizing them.                                                      ***
*******************************************************************************
*** 1.Take the name of the patient along with his/her age and create a      ***
*** patient ID for the patient.                                             ***
*** 2.Insert the patient id in the priority list based on the age of patient***
*** 3.Display the next patient ID in line to meet the doctor. Remove this   ***
*** patient from the priority list.                                         ***
*******************************************************************************
*******************************************************************************
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <libgen.h>
#include <errno.h>
#include <string.h>
#include <getopt.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>
#include <string.h>


// A Doubly Linked List Element Data Structure to store a Patient Record
struct DoubleListElementType
{
  int PatientID; // Patient ID, that is auto generated
  char PatientName[50]; // Patient Name
  int Age; // Age of the Patient
  int EnrolledOrder; // Time at which the Patient is enrolled
  unsigned long int Priority; // Overall priority of the patient
  struct DoubleListElementType *PreviousElement; // Pointer to the previous element in the list
  struct DoubleListElementType *NextElement; // Pointer to the next element in the list
};

// A Max Heap Priority Queue Data Structure to store a Patient record with priority
struct MaxHeapElementType
{
  int PatientID; // Patient ID, that is auto generated
  char PatientName[50]; // Patient Name
  int Age; // Age of the Patient
  int EnrolledOrder; // Time at which the Patient is enrolled
  unsigned long int Priority; // Overall priority of the patient
};

/**************************************************************************************
************************ Function Declaration Section Starts **************************
***************************************************************************************/
// Doubly Linked List Handling Related Function Definitions
// Doubly Linked List has a List_Head Node
struct DoubleListElementType *Build_DLList_From_File (struct DoubleListElementType *List_Head,
					    char File_Name[]);
struct DoubleListElementType *Insert_Element_DLList (struct DoubleListElementType *List_Head,
					   struct DoubleListElementType *User_Input_record);
struct DoubleListElementType *Delete_Element_DLList (struct DoubleListElementType *List_Head);
bool Find_Element_DLList (struct DoubleListElementType *List_Head,
		      int New_Element_Value);
void Display_Elements_DLList (struct DoubleListElementType *List_Head);
void Display_Current_DLList_Element (struct DoubleListElementType* Display_Element);

void Display_Single_Record (struct MaxHeapElementType *Display_Element);
void Display_Current_Record (struct MaxHeapElementType *Display_Element);

// Function definitions related to Max Heap implemented using Array
void Swap_Heap_Elements (struct MaxHeapElementType *ElementOne, struct MaxHeapElementType *ElementTwo);
void Display_Heap (struct MaxHeapElementType *MaxHeapArray, int HeapSize);
void Heapify_TopDown_Patient_Records (struct MaxHeapElementType *MaxHeapArray, int TotalRecords, int CurrentHeapRoot);
void Heapify_BottomUp_Patient_Records (struct MaxHeapElementType *MaxHeapArray, int TotalRecords, int CurrentHeapRoot);
void Build_MaxHeap_From_File ( char File_Name[]);
void Insert_Element_MaxHeap (struct DoubleListElementType* User_Input_Record);
void Delete_Element_MaxHeap (void);

/* 
Takes a string as input
Returns true if it is a number else false
If Null, then returns false
If it is a decimal with '.', still returns false
*/
bool isIntegerNumber (char *StringNumber) {
  int i = 0;			// Temp variable for the 'For' loop
  bool IsNumber = false;	// Boolean variable, to store 'true' if it is a number

  if (StringNumber[0] == '-')
    {				// If negative number, move to next character
      i++;
    }

  for (; StringNumber[i] != '\0'; i++)
    {				// Go till the end of the string, reach each character
      if (StringNumber[i] == '.')
	{			// If it has a '.' decimal, as part of the string
	  IsNumber = false;	// Return 'false' as this is not an integer number
	  break;		// Exit the for loop
	}
      if (isdigit (StringNumber[i]) == false)
	{			// Is this character is a proper digit ?
	  IsNumber = false;	// If not a digit, not a number
	  break;		// Exit the loop
	}
      else
	{
	  IsNumber = true;	// If this is a digit, continue
	}
    }
  return IsNumber;		// Return true or false, based on our result
}

/* Global Variables Declaration */
unsigned long int PatientIDGbl=0, PatientIDGblMaxHeap=0 ; // PatientID generated using this variable, 
                                                         //though we have 2 variables, both are in sync
int TotalPatientsWaiting=0; // Total Patients in the Queue at this moment; this also gives the size of the Heap
int HeapSize=128; // Size of the Heap, initialize to 128
struct DoubleListElementType *List_Head = NULL;	// Head Pointer to the Doubly Linked List
struct DoubleListElementType *User_Input_Record = NULL; // To temporarily store Patient record Values
struct MaxHeapElementType *MaxHeapPatientRecords;

/**************************************************************************************
************ Main funciton has the key User Options, about the next action ************
***************************************************************************************
* Step-1 : Get User Input, on the next operation on the Patient Data base  ************
* Step-2 : Get User input, either 'File Name' or 'Element Value',          ************
* based on the user option                                                 ************
* Step-3 : Perform the user requested operation                            ************
* Step-4 : Continue in a while loop, till the user chooses the exit option ************
* *************************************************************************************
***************************************************************************************/
int main ()
{
  /* Variable Declarations */
  int User_Option = 0;		// For the user to choose his next Option
  int User_Input_Element = 0, Queue_Rear_Position = -1, Queue_Front_Position = 0;	// User Input Element, Array Queue Rear and Front position
  char File_Name[250], User_Input_String[30];	// String to get user input for the file name
  bool Present = false;		// Boolean variable to identify if the element is present in the list
 
  MaxHeapPatientRecords = realloc (MaxHeapPatientRecords,(sizeof(struct MaxHeapElementType)*128));
  User_Input_Record = (struct DoubleListElementType*)malloc(sizeof(struct DoubleListElementType)); // Allocate memory for the new element
  User_Input_Record->NextElement = NULL; // Temporarily make the next of the new element as 'NULL'
  User_Input_Record->PreviousElement = NULL; // Temporarily make the next of the new element as 'NULL'

  printf ("\nThe Program has Started\n");

  while (User_Option != 16)	// Continue the execution in loop, till the Exit option is chosen by the user
    {
      /* Display all the available options to the user */
      printf
	("\n 1. Enroll a new patient from File \n2. Enroll patient details in command line\n3. Get the Next Patient ID in the List \n4. Remove a patient Record \n5. Display the Patient List\n9: Quit\n");
      printf ("Enter your choice : ");
      scanf ("%d", &User_Option);
      switch (User_Option)
	  {
	case 1:
	  printf ("\n1. Input Patient List from File (Multiple elements).\n");
	  printf ("\n Enter the File Name :\n");
	  scanf ("%s", File_Name);
      List_Head = Build_DLList_From_File (List_Head, File_Name);	// Call the Insert function for all the Patient Records in a file, for insertion in to the doubly Linked list
//      Build_MaxHeap_From_File(MaxHeapArrayPtr, File_Name);
      Build_MaxHeap_From_File(File_Name);
	  break;
	case 2:
	  printf ("2. Input Patient Details from command line (Single)\n");
	  printf ("\n Enter the Patient Name :\n");
	  scanf ("%s",User_Input_Record->PatientName);
	  printf ("\n Enter the Patient's Age :\n");
	  scanf ("%d",&User_Input_Record->Age);

	  List_Head = Insert_Element_DLList (List_Head, User_Input_Record);	// Call the Insert function for the element, for insertion in to the doubly linked list
      Insert_Element_MaxHeap (User_Input_Record); // Call the Insert function for the element, for insertion in to the Max Heap
	  break;
	case 3:
	  printf ("3. Get the next Patient Record\n");
	  Display_Single_Record (MaxHeapPatientRecords);	// Display the Top priority Patient Record, from the Heap
	  break;
	case 4:
	  printf ("4. Remove the Patient Record \n");
	  if (TotalPatientsWaiting == 0) {
	      printf("\nThe Waiting List is Empty\n");
	  }
	  List_Head = Delete_Element_DLList (List_Head);	// Call the Delete function for the element, from the Doubly Linked List
      Delete_Element_MaxHeap(); // Call the Delete, in a Max Heap, it would delete the top priority element or patient record; as the patient is just serviced
	  break;
	  case 5:
	  printf ("5. Display the Patients Records in the right order of visit\n");
	      Display_Elements_DLList (List_Head);	// Display the Patient Records, in the order
	      Display_Heap (MaxHeapPatientRecords,TotalPatientsWaiting); // Display the Max Heap Patient records
	  break;
	case 9:
	  printf ("9: Quit\n");
	  goto clean_out;
	  break;
	default:
	  printf ("Please enter correct option\n");	// Display this, if the user enters a wrong option
	  break;
	}
    }
clean_out:
  return 0;
}

/*************************** Main Function Ends **************************************/

/*
Goal of the procedure is to swap 2 elements.
Step 1 : Copy the first element to the Temp
Step 2 : Copy the second element to the first
Step 3 : Copy the Temp, which has the original first element details, to the Second element
*/
void Swap_Heap_Elements(struct MaxHeapElementType *ElementOne, struct MaxHeapElementType *ElementTwo) {
  struct MaxHeapElementType ElementTemp;
  
   printf("\n Swap Heap Elements : Child : %d \n", ElementOne->PatientID);
  printf("Parent : %d \n", ElementTwo->PatientID);
  ElementTemp.PatientID = ElementOne->PatientID;
  strcpy(ElementTemp.PatientName,ElementOne->PatientName);
  ElementTemp.Age = ElementOne->Age;
  ElementTemp.EnrolledOrder = ElementOne->EnrolledOrder;
  ElementTemp.Priority = ElementOne->Priority;
  
  ElementOne->PatientID = ElementTwo->PatientID;
  strcpy(ElementOne->PatientName,ElementTwo->PatientName);
  ElementOne->Age = ElementTwo->Age;
  ElementOne->EnrolledOrder = ElementTwo->EnrolledOrder;
  ElementOne->Priority = ElementTwo->Priority;
  
  ElementTwo->PatientID = ElementTemp.PatientID;
  strcpy(ElementTwo->PatientName,ElementTemp.PatientName);
  ElementTwo->Age = ElementTemp.Age;
  ElementTwo->EnrolledOrder = ElementTemp.EnrolledOrder;
  ElementTwo->Priority = ElementTemp.Priority;
  printf("\n Child : %d, %d \n", ElementOne->PatientID, ElementOne->Age);
  printf("Parent : %d, %d \n", ElementTwo->PatientID, ElementOne->Age);
}

/*
Goal of the procedure is to Heapify, from Top Down [Starting from the Root of the given Tree]
Step 1 : Find the children of the given element
Step 2 : Check, whether the given element priority is lower than the left child, 
                If so, choose it as a 'candidate for a swap'
Step 3 : Check, whether the given element priority is lower than the right child, 
                If so, choose it as a 'candidate for a swap'
Step 4 : Swap the choosen child element, with the Parent Element
Step 5 : Recursively, Heapify the new child Element ' Top Down'
*/
void Heapify_TopDown_Patient_Records (struct MaxHeapElementType *MaxHeapArray, int TotalRecords, int CurrentHeapRoot) {
  int largest = CurrentHeapRoot; // Initialize largest as root
  int leftChild = 2 * CurrentHeapRoot + 1; // left child = 2*i + 1
  int rightChild = 2 * CurrentHeapRoot + 2; // right child = 2*i + 2
  struct DoubleListElementType *ElementTemp;

  // If left child is greater than root

  if (leftChild < TotalRecords && MaxHeapArray[leftChild].Priority > MaxHeapArray[largest].Priority)
    largest = leftChild;

  // If right child is greater than new largest

  if (rightChild < TotalRecords && MaxHeapArray[rightChild].Priority > MaxHeapArray[largest].Priority)
    largest = rightChild;

  // If largest is not the root

  if (largest != CurrentHeapRoot) { // swap root with the new largest
    Swap_Heap_Elements(&MaxHeapArray[CurrentHeapRoot], &MaxHeapArray[largest]);

    // Recursively heapify the affected sub-tree i.e, subtree with root as largest
    Heapify_TopDown_Patient_Records(MaxHeapArray, TotalRecords, largest);
  }
}

/*
Goal of the procedure is to Heapify, from Bottom up [Starting from the given element]
Step 1 : Find the parent of the given element
Step 2 : Check, whether the given element priority is bigger than the parent priority
Step 3 : If the child Element priority is higher, swap it with the Parent Element
Step 4 : Recursively, Heapify the Parent Element
*/
void Heapify_BottomUp_Patient_Records (struct MaxHeapElementType *MaxHeapArray, int TotalRecords, int CurrentHeapRoot) {
    int Parent; // Initialize largest as root

    if (CurrentHeapRoot > 0) {
       Parent = (CurrentHeapRoot-1)/2; // Identify the Parent Position
/*      printf("\n Heapify_BottomUp : Child : %lu & Position : %d\n", MaxHeapArray[CurrentHeapRoot].Priority, CurrentHeapRoot);
       printf("Parent : %lu & Position : %d\n", MaxHeapArray[Parent].Priority, Parent); */
       if (MaxHeapArray[Parent].Priority < MaxHeapArray[CurrentHeapRoot].Priority) { // Check if the Parent Priority is lower
//           printf("\nParent is Smaller\n");
           Swap_Heap_Elements(&MaxHeapArray[CurrentHeapRoot], &MaxHeapArray[Parent]); // swap root with the new largest
 
           // Recursively heapify the affected sub-tree i.e, subtree with root as largest
           Heapify_BottomUp_Patient_Records (MaxHeapArray, TotalRecords, Parent);
       }
    }
}

/*
Step 1 : Heap Array & New_Element are received as input parameters
Step 2 : If the Heap is full, increase the size to accomodate the next level elements (records)
Step 3 : Properly fill each field of the record
Step 4 : If the new element is the first element, no additional steps needed
Step 5 : Otherwise, Heapify the tree from the new element inserted
Step 6 : In turn, Heapify up to the root recursively

*/
void Insert_Element_MaxHeap (struct DoubleListElementType* User_Input_Record) {
 unsigned long Memory_Need=0;
 
    if (HeapSize == TotalPatientsWaiting) { // Increase the Heap Size, based on the need
        HeapSize = TotalPatientsWaiting * 2;
        Memory_Need = sizeof(struct MaxHeapElementType) * HeapSize;
        printf("\n Memory Resizing Happening Heap Size : %d & Memory Size : %lu\n",HeapSize,Memory_Need);
        MaxHeapPatientRecords = realloc (MaxHeapPatientRecords,Memory_Need);
    }
    MaxHeapPatientRecords[TotalPatientsWaiting].Age = User_Input_Record->Age; // Copy the new Patient age
    if (strcpy(MaxHeapPatientRecords[TotalPatientsWaiting].PatientName,User_Input_Record->PatientName) != NULL) {
        MaxHeapPatientRecords[TotalPatientsWaiting].PatientID = ++PatientIDGblMaxHeap; // Copy the Patient ID	  
	    MaxHeapPatientRecords[TotalPatientsWaiting].EnrolledOrder = PatientIDGblMaxHeap; // Copy the Enrolled Order
	    MaxHeapPatientRecords[TotalPatientsWaiting].Priority = User_Input_Record->Age*1000+MaxHeapPatientRecords[TotalPatientsWaiting].EnrolledOrder; // Calculate the Priority based on age
//	    printf("\n *** New Patient Name : %s (%d)   Priority : %lu ***",MaxHeapPatientRecords[TotalPatientsWaiting].PatientName, 
//	          MaxHeapPatientRecords[TotalPatientsWaiting].EnrolledOrder, MaxHeapPatientRecords[TotalPatientsWaiting].Priority);
    }
//    printf("\n New Patient Record  %s(%d) is getting added",MaxHeapArray[TotalPatientsWaiting].PatientName,MaxHeapArray[TotalPatientsWaiting].PatientID);
    TotalPatientsWaiting++;
//    printf("\n TotalPatientsWaiting  :%d \n",TotalPatientsWaiting);
    if (TotalPatientsWaiting > 1) { // Recursively Heapify the Tree, starting from the new element bottom up
        Heapify_BottomUp_Patient_Records (MaxHeapPatientRecords, TotalPatientsWaiting, TotalPatientsWaiting-1);
    } else { // If this is the first element, no need to Heapify
        printf("\n First Patient Record : Adding to the Heap \n");
    }
    return; // End of the function
}

/*
Step 1 : Max Heap Array and File_Name are received as input parameters
Step 2 : The corresponding File is opened
Step 3 : Read line by line from the file
Step 4 : Split the line to get the Name and Age of the Patient 
Step 5 : Call function “Insert_Element_MaxHeap” to Insert the new element to the Heap
Step 6 : Once the complete file is read, close the file
Step 7 : Return the latest ‘List Head’ Pointer, after adding all the new elements
*/
void Build_MaxHeap_From_File (char File_Name[]) {
    int New_Element_Value = 0; // To hold the current element value
	char New_Row[250]; // To hold the current word read from the file
	char *Current_Word;
    FILE *File_Pointer; // File pointer to open the file
 
    if (File_Pointer = fopen(File_Name,"r")) { // If the file is valid and opened successfully, move forward with the next steps
        
        printf ("\n This is a .txt File\n");
        while (fgets(New_Row,250,File_Pointer) != NULL) { // If the current word is read successfully
     
          Current_Word = strtok (New_Row," ");
 	      
 	      Current_Word = strtok(NULL," ");
          strcpy(User_Input_Record->PatientName,Current_Word);
          // printf ("\n Patient Name :%s\n", User_Input_Record->PatientName);
	      Current_Word = strtok(NULL, " ");
	      User_Input_Record->Age = atoi(Current_Word);
	      // printf ("\n Patient's Age :%d\n", User_Input_Record->Age);
	      
	      Insert_Element_MaxHeap (User_Input_Record);	// Call the Insert function for the element, for insertion in to a sorted list
        }
        fclose(File_Pointer); // Close the file after completion of reading
    } else { // Intimate the user, if the given file doesn't exist or not able to open it
        printf("\nNo Such File Exists \n");
    }
    return; // Return for the end of the function
}

/*
Step 1 : Heap Array is received as input parameters
Step 2 : If the Heap is empty, report the same
Step 3 : If the Heap level is reducing, free up the space and reduce the size of the Array (records)
Step 4 : Delete the first Patient record (top priority record)
Step 5 : Copy the last element to the first position
Step 6 : Heapify from the root "Top to Bottom"
*/
void Delete_Element_MaxHeap (void) {
    unsigned long Memory_Need=0;
    
    printf("\n Delete Patient Record From Max Heap\n");
    if (TotalPatientsWaiting == 0) { // If the List is empty, intimate the user
        printf("\n List is Empty \n");
	} else { // If the element to be deleted is the first element
	    TotalPatientsWaiting--;
	    if ((HeapSize/2) == TotalPatientsWaiting) { // reduce the Heap size, based on the need
            HeapSize = HeapSize / 2;
            if (HeapSize <128) {
                HeapSize = 128;
            }
            Memory_Need = sizeof(struct MaxHeapElementType) * HeapSize;
            MaxHeapPatientRecords = realloc (MaxHeapPatientRecords,Memory_Need);
        }
	    printf("\n Patient Record identified is the First Record: %s (%lu) \n", MaxHeapPatientRecords[0].PatientName,MaxHeapPatientRecords[0].Priority);
	    // MaxHeapArray[0] = MaxHeapArray[TotalPatientsWaiting]; // Copy each value of the last Patient record to the first record being deleted
	    MaxHeapPatientRecords[0].PatientID = MaxHeapPatientRecords[TotalPatientsWaiting].PatientID;
        strcpy(MaxHeapPatientRecords[0].PatientName,MaxHeapPatientRecords[TotalPatientsWaiting].PatientName);
        MaxHeapPatientRecords[0].Age = MaxHeapPatientRecords[TotalPatientsWaiting].Age;
        MaxHeapPatientRecords[0].EnrolledOrder = MaxHeapPatientRecords[TotalPatientsWaiting].EnrolledOrder;
        MaxHeapPatientRecords[0].Priority = MaxHeapPatientRecords[TotalPatientsWaiting].Priority;
	    Heapify_TopDown_Patient_Records(MaxHeapPatientRecords,TotalPatientsWaiting,0);
	}
    return; // return for end of the function
}

/*
Step 1 : List_Head Pointer and File_Name are received as input parameters
Step 2 : The corresponding File is opened
Step 3 : Read word by word from the file
Step 4 : Convert the word to an integer
Step 5 : Call function “Insert_Element” to Insert the new element to the 'Doubly Linked List'
Step 6 : If this element is inserted and the first element of the 'Doubly Linked List' changes,
            the new ‘List Head’ is returned back from the “Insert_Element” function
Step 7 : Once the complete file is read, close the file
Step 8 : Return the latest ‘List Head’ Pointer, after adding all the new elements
*/
struct DoubleListElementType* Build_DLList_From_File (struct DoubleListElementType* List_Head, char File_Name[]) {
	int New_Element_Value = 0; // To hold the current element value
	char New_Row[250]; // To hold the current word read from the file
	char *Current_Word;
    FILE *File_Pointer; // File pointer to open the file
 
    if (File_Pointer = fopen(File_Name,"r")) { // If the file is valid and opened successfully, move forward with the next steps
        
        printf ("\n This is a .txt File\n");
        while (fgets(New_Row,250,File_Pointer) != NULL) { // If the current row is read successfully
          printf ("\n Read Line : %s\n", New_Row);
     
          Current_Word = strtok (New_Row," ");
 	      
 	      Current_Word = strtok(NULL," "); // Get the Name of the Patient from the current row
          strcpy(User_Input_Record->PatientName,Current_Word); // Copy the Name of the Patient to our temp record
 //         printf ("\n Patient Name :%s\n", User_Input_Record->PatientName);
	      Current_Word = strtok(NULL, " "); // Get the Age of the Patient from the current row
	      User_Input_Record->Age = atoi(Current_Word); // Copy the Age of the Patient to our temp record
//	      printf ("\n Patient's Age :%d\n", User_Input_Record->Age);
	      
	      List_Head = Insert_Element_DLList (List_Head, User_Input_Record);	// Call the Insert function for the element, for insertion in to the Doubly Linked list
        }
        fclose(File_Pointer); // Close the file after completion of reading
    } else { // Intimate the user, if the given file doesn't exist or not able to open it
        printf("\nNo Such File Exists \n");
    }
    return List_Head; // Return the new 'List_Head'
}

/*
Step 1 : List_Head Pointer, New_Element_Value are received as input parameters
Step 2 : Create a new_Element; allocate memory
Step 3 : If the input record name is valid, mvoe forward, if not valid delete free the memory and exit the function
Step 4 : Assign proper values for name, Age, Priority, PatientID & Enrolled Order
Step 5 : If the list is empty, consider this as the first element
Step 6 : Go till the last element in the list
Step 7 : Insert this new element as the last element
Step 8 : Assign this new element as the 'next' to the last element; assign 'previous' of this new element to the last element
Step 9 : Return the new ‘Head’ of the Doubly Linked List
*/
struct DoubleListElementType* Insert_Element_DLList (struct DoubleListElementType* List_Head, struct DoubleListElementType* User_Input_Record) {
    struct DoubleListElementType *New_Element, *Current_Element; // Pointers for the List elements, new element and the current element from the list

 	New_Element = (struct DoubleListElementType*)malloc(sizeof(struct DoubleListElementType)); // Allocate memory for the new element
    New_Element->NextElement = NULL; // Temporarily make the next of the new element as 'NULL'
    New_Element->PreviousElement = NULL; // Temporarily make the next of the new element as 'NULL'
	
    if (strcpy(New_Element->PatientName,User_Input_Record->PatientName) != NULL) { // Valid Patient name exists, then proceed
        New_Element->Age = User_Input_Record->Age; // Copy the new element value
	    New_Element->PatientID = ++PatientIDGbl; // Copy the new element value
	    New_Element->EnrolledOrder = New_Element->PatientID; // Assign the Enrollment order
        New_Element->Priority = New_Element->Age*1000 + New_Element->EnrolledOrder; // Calculate the Priority
  	    printf("\nNew Patient Name : %s (%d)   Priority : %lu", New_Element->PatientName, New_Element->PatientID, New_Element->Priority);
    } else { // If valid Patient name doesn't exist
        free (New_Element); // Free the memory allocated to the new Patient record
        return List_Head;
    }
    
	if (List_Head == NULL) { // If the list is empty, consider this as the first element
	    printf("\n First Element \n");
	    List_Head = New_Element; // Assign the new element address to 'List_Head'
	    return List_Head; // return the 'List_Head', the new element pointer
	}
	Current_Element = List_Head; // Start from the first element in the list
	while (Current_Element->NextElement != NULL) { // Till the last element in the list continue
	    Current_Element = Current_Element->NextElement;
	}
    printf("\n New Patient Record  %s(%d) is getting added",New_Element->PatientName,New_Element->PatientID);
    Current_Element->NextElement = New_Element; // Assign the new element as the 'next' to the last element
    New_Element->PreviousElement = Current_Element; // Assign the last element as the 'previous' to the new element

    return List_Head; // Send the new Header element Pointer back
}

/*
Step 1 : List Header pointer is received as input parameters
Step 2 : Create Pointers for the New Head, Current Element and High Priority Element
            also the next and previous elements (to the current element)
Step 3 : If the list is empty intimate the user
Step 4 : Search the entire list from first to last, for the high Priority Element
    "If you find a current element Priority to be bigger than the previously identified 'high Priority Elemt' priority, 
    Make the current element as the new Hgih Priority Element"
        move to the next element
Step 5 : If the first element is to be deleted, Change the 'List_Head' pointer; Free the old Head Node
Step 6 : Delete the High Priority element
        Before this, 
            a) Update the 'next' pointer in the previous element to the next element
            b) Update the 'Previous' pointer in the next element to the previous element
        delete the element; free the memory
Step 7 : Return the updated List_Head pointer
*/
struct DoubleListElementType* Delete_Element_DLList (struct DoubleListElementType* List_Head) {
    struct DoubleListElementType *Current_Element=NULL, *New_Head=NULL, *Previous_Element=NULL, *Next_Element=NULL, *High_Priority_Element=NULL; // Pointer Declarations for the First / Header List element, Current and previous List elements
    
    printf("\n Delete Patient record From Doubly Linked List\n");
    if (List_Head == NULL) { // If the List is empty, intimate the user
        printf("\n List is Empty \n");
	    return List_Head; // return the same NULL
	}

	Current_Element = List_Head;
	High_Priority_Element = List_Head;
	while (Current_Element->NextElement != NULL) { // Search till the last element in the list
	    if (Current_Element->Priority > High_Priority_Element->Priority) { // Check if the current element priority is bigger than the already identified 'High Priority Element'
//            printf("\n Patient ID : %d\n",Current_Element->PatientID);
            High_Priority_Element = Current_Element; // Assign the Current element as the high priority element
	    }
	    Current_Element = Current_Element->NextElement; // Move to the next element
	}

	if (High_Priority_Element != NULL) { // Wolaah, we found the element to be deleted
		if (List_Head == High_Priority_Element) { // If the element to be deleted is the first element
	        printf("\n Patient Record identified is the First Record: %s (%lu) \n", List_Head->PatientName,List_Head->Priority);
	        New_Head = List_Head->NextElement; // Change the Head Node pointer
	        free (List_Head); // Free the memory allocated to the deleted Head node
	        return New_Head; // return the new head pointer
	    }
	   printf("\n Patient Record Identified : %s (%lu) \n", High_Priority_Element->PatientName,High_Priority_Element->Priority);
	   Previous_Element = High_Priority_Element->PreviousElement;
	   Next_Element = High_Priority_Element->NextElement;
	   if (Previous_Element != NULL) {
	      Previous_Element->NextElement = High_Priority_Element->NextElement; // Update the 'next' pointer in the previous element to the new next element
	   }
	   if (Next_Element != NULL) {
          Next_Element->PreviousElement = High_Priority_Element->PreviousElement; // Update the 'next' pointer in the previous element to the new next element
	   }
	   free (High_Priority_Element); // Free the memory allocated to the deleted element
	}
    return List_Head; // return the new Head Node, of the sorted list
}

/*
Step 1 : List Header Pointer and the 'to be Found' element value are received as input parameters
Step 2 : Create Pointer for the Current Element ;
        Create a boolean variable for 'Present' : If the given element is present in the sorted list or not
Step 3 : If the list is empty intimate the user
Step 4 : If the 'to be found' element is the first element return 'true'
Step 5 : Search the entire list from first to last, for the element 'to be deleted'
    If you find a current element value to be bigger than the given element value, exit the loop
        as this is a sorted list, you can't find your element post this.
Step 6 : If you found your element
        Update the 'next' pointer in the previous element to the new next element
        delete the element; free the memory
Step 7 : If the element given by the user is found, intimate the user; if not found, intimate the user as well
Step 8 : Return the result, whether the given element is found in the 'Sorted List' or not
*/
bool Find_Element_DLList (struct DoubleListElementType* List_Head,int Element_Value) {
	struct DoubleListElementType* Current_Element=NULL; // Create Pointer for the Current Element
	bool Present=false; // Create a boolean variable for 'Present' : If the given element is present in the sorted list or not
    
    if (List_Head == NULL) { // If the list is empty intimate the user
        printf("\n List is Empty \n");
	    return false;
	} else if (List_Head->PatientID == Element_Value) { // If the 'to be found' element is the first element return 'true'
	    printf("\n Element Identified is the Head Node: %d \n", Element_Value);
	    return true;
	}
	Current_Element = List_Head; // Start from the first element
	while (Current_Element->NextElement != NULL) { // Search till the end of the list
	    if (Current_Element->PatientID != Element_Value) { /*If you find the current patient ID is not the same as requested search element value,
                                                            Move to the next element */
	        Current_Element = Current_Element->NextElement; // Move to the next element
	    } else { // Come out of the loop, if the current element is bigger than the given element value
	        break;
	    }
	}
	if (Current_Element->PatientID == Element_Value) { // If the given Patient ID is found, intimate the user
	   printf("\n Patient Record Identified : %d \n", Element_Value);
	   Present=true;
	} else { // If the given Patient ID is not found, intimate the user
	        printf("\n Patient Record Not Found : %d \n", Element_Value);
	        Present=false;
	}
	return Present; // Return the result, whether the element is found in the 'Sorted List' or not
}

/*
Step 1 : List Header Pointer is received as input parameters
Step 2 : Create Pointer for the Current Element
Step 3 : If the list is empty intimate the user
Step 4 : Display the entire list from first to last
*/
void Display_Elements_DLList (struct DoubleListElementType* List_Head) {
	struct DoubleListElementType* Display_Element; // Create Pointer for the Current Element to be displayed

    printf("\n The Patient Records : \n");	
	Display_Element = List_Head; // Start from the first element
	if (List_Head == NULL) { // If the list is empty intimate the user
	    printf("\n List is Empty\n");
	}
	while (Display_Element != NULL ) { // Display the entire list from first to last
		Display_Current_DLList_Element (Display_Element);
		Display_Element = Display_Element->NextElement; // Move to the next element
	}
	return;
}

/*
Step 1 : List Element Pointer is received as input parameters
Step 2 : If the list is empty intimate the user
Step 3 : Display the Element
*/
void Display_Current_DLList_Element (struct DoubleListElementType* Display_Element) {

	if (Display_Element == NULL) { // If the list is empty intimate the user
	    printf("\n No Element\n");
	} else {
		printf("\n %d       ", Display_Element->PatientID); // Display the Patient ID
		printf(" %s       ", Display_Element->PatientName); // Display the Name
		printf(" %d       ", Display_Element->Age); // Display the Age
		printf(" %lu \n", Display_Element->Priority); // Display the element
	}
	return;
}

/*
Step 1 : Heap Pointer is received as input parameters
Step 2 : Display the entire Heap from first to last
*/
void Display_Heap (struct MaxHeapElementType *MaxHeapArray, int HeapSize) {
  printf("\nPrint The Heap : \n ");
  printf("\nPatientID      Patient Name    Age        Priority : \n ");
  for (int i = 0; i < HeapSize; ++i)
  {
    Display_Current_Record (&MaxHeapArray[i]); // Display_Current_Record
  }

  printf("\n");
}

/*
Step 1 : Heap Element Pointer is received as input parameters
Step 2 : If the list is empty intimate the user
Step 3 : Display the Element
*/
void Display_Single_Record (struct MaxHeapElementType* Display_Element) {

    printf("\n The Patient Record : \n");	
	if (Display_Element == NULL) { // If the list is empty intimate the user
	    printf("\n Element Not Present\n");
	} else  { // Display the entire list from first to last
		printf("Patient ID : %d    ", Display_Element->PatientID); // Display the Patient ID
		printf("Patient Name : %s    ", Display_Element->PatientName); // Display the Name
		printf("Age : %d    ", Display_Element->Age); // Display the Age
		printf("Patient Priority : %lu \n", Display_Element->Priority); // Display the element
	}
	return;
}

/*
Step 1 : List Header Pointer is received as input parameters
Step 2 : Create Pointer for the Current Element
Step 3 : If the list is empty intimate the user
Step 4 : Display the entire list from first to last
*/
void Display_Current_Record (struct MaxHeapElementType* Display_Element) {

	if (Display_Element == NULL) { // If the list is empty intimate the user
	    printf("\n No Element\n");
	} else {
		printf("\n %d       ", Display_Element->PatientID); // Display the Patient ID
		printf(" %s       ", Display_Element->PatientName); // Display the Name
		printf(" %d       ", Display_Element->Age); // Display the Age
		printf(" %lu \n", Display_Element->Priority); // Display the element
	}
	return;
}



/**************************************************************************************
******************* Queue and Sorted List Function Definitions Start ******************
***************************************************************************************/


/*
void
Find_operation_time_for_different_data_Structures ()
{
  int array[10] =
    { 5268, 6389, 3621, 8879, 6961, 4282, 2258, 1732, 0576, 5938 };
  int Queue_Array[1000];
  int User_Input_Element = 0, Queue_Rear_Position = -1, Queue_Front_Position =
    0;
  struct ListElementType *List_Head = NULL;
  int i = 0;
  double total_time_BST = 0, total_time_SL = 0, total_time_Array_Q =
    0, total_time_linked_list = 0;
  struct node *root = NULL;
  clock_t t;
  printf ("=================================================\n");
  for (i = 0; i < 10; i++)
    {
      t = clock ();
      insert (root, array[i]);
      t = clock () - t;
      total_time_BST = total_time_BST + ((double) t) / CLOCKS_PER_SEC;
    }
  printf ("Average Insertion time in BST: %lf\n", total_time_BST);
  total_time_BST = 0;
  for (i = 0; i < 10; i++)
    {
      t = clock ();
      search (root, array[i]);
      t = clock () - t;
      total_time_BST = total_time_BST + ((double) t) / CLOCKS_PER_SEC;
    }
  printf ("Average Searching time in BST: %lf\n", total_time_BST);
  total_time_BST = 0;
  for (i = 0; i < 10; i++)
    {
      t = clock ();
      deleteNode (root, array[i]);
      t = clock () - t;
      total_time_BST = total_time_BST + ((double) t) / CLOCKS_PER_SEC;
    }
  printf ("Average Deletion time in BST: %lf\n", total_time_BST);
  total_time_BST = 0;
  printf ("=================================================\n");
  for (i = 0; i < 10; i++)
    {
      t = clock ();
      Queue_Rear_Position =
	ENQUEUE_Element_Array (Queue_Array, Queue_Rear_Position, array[i]);
      t = clock () - t;
      total_time_Array_Q = total_time_Array_Q + ((double) t) / CLOCKS_PER_SEC;
    }
  printf ("Average Insertion time in QUEUE: %lf\n", total_time_Array_Q);
  total_time_Array_Q = 0;
  for (i = 0; i < 10; i++)
    {
      t = clock ();
      Queue_Rear_Position =
	DEQUEUE_Element_Array (Queue_Array, Queue_Rear_Position,
			       Queue_Front_Position);
      t = clock () - t;
      total_time_Array_Q = total_time_Array_Q + ((double) t) / CLOCKS_PER_SEC;
    }
  printf ("Average Deletion time in QUEUE: %lf\n", total_time_Array_Q);
  printf ("=================================================\n");
  for (i = 0; i < 10; i++)
    {
      t = clock ();
      List_Head = Insert_Element_SL (List_Head, array[i]);
      t = clock () - t;
      total_time_linked_list =
	total_time_linked_list + ((double) t) / CLOCKS_PER_SEC;
    }
  printf ("Average Insertion time for Sorted List: %lf\n",
	  total_time_linked_list);
  total_time_linked_list = 0;
  for (i = 0; i < 10; i++)
    {
      t = clock ();
      Find_Element_SL (List_Head, array[i]);
      t = clock () - t;
      total_time_linked_list =
	total_time_linked_list + ((double) t) / CLOCKS_PER_SEC;
    }
  printf ("Average Searching time for Sorted List: %lf\n",
	  total_time_linked_list);
  total_time_linked_list = 0;
  for (i = 0; i < 10; i++)
    {
      t = clock ();
      List_Head = Delete_Element_SL (List_Head, array[i]);
      t = clock () - t;
      total_time_linked_list =
	total_time_linked_list + ((double) t) / CLOCKS_PER_SEC;
    }
  printf ("Average Deletion time for Sorted List: %lf\n",
	  total_time_linked_list);
  total_time_linked_list = 0;
  printf ("=================================================\n");
}

/*    strcpy(New_Row,"42 ThisGuy 32 10");
      
      Current_Word = strtok(New_Row," ");
      printf("Current_Word : %s \n",Current_Word);
      printf("New_Row : %s \n",New_Row);
      Current_Word = strtok(NULL, " ");
      
      User_Input_Record->PatientID = ++PatientIDGbl;
      strcpy(User_Input_Record->PatientName,Current_Word);
      printf ("\n Patient Name :%s\n", User_Input_Record->PatientName);
	  Current_Word = strtok(NULL, " ");
	  User_Input_Record->Age = atoi(Current_Word);
	  printf ("\n Patient's Age :%d\n", User_Input_Record->Age);
	  
	  User_Input_Record->EnrolledOrder = PatientIDGbl;
	  User_Input_Record->Priority = User_Input_Record->Age<<16+User_Input_Record->EnrolledOrder;
	  printf("\nPriority : %d",User_Input_Record->Priority); */
/**************************************************************************************
********************************** End of the Program *********************************
***************************************************************************************/
