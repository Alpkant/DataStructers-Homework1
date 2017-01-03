/* @Author
 * Student Name: Alperen Kantarci
 * Student ID: 150140140
 * Date: 18/09/2016
 */

#include <stdlib.h>
#include <fstream>
#include <cstdio>
#include <iostream>
#include <cstring>
#include <string>
#include <stdio.h>
#include "record.h"
using namespace std;


FILE * database;

int numOfPeople=0;  // How many patient record do we have
void open_file(char *text_name);
void read_file();
void print_menu();
void order_the_list();
void sort_file();
void insert_record();
void remove_record();
void push_one_left(int indis);
bool search_patients(bool search_method);
void push_right(int indis,int counter);
bool make_operation(char choice);
void print_records();
void update_close_textfile();
void delete_all_arrays();

int main()
{
    ar = new patient_record[200];

    open_file((char*) "database.txt");
    read_file();
    sort_file();
    order_the_list();
    bool finish=false;
    char choice;

    while(!finish){
        print_menu();
        cin>>choice;
        finish = make_operation(choice);
    }


    update_close_textfile();
    delete_all_arrays();


    return 0;
}

void print_menu(){
    system("cls");
     cout << endl << endl;
     cout << "Please select the operation to perform and enter the operation code" << endl;
     cout << "(P) Print all of the patient records," << endl;
     cout << "(B) Search the data by the branch name," << endl;
     cout << "(C) Search the data by the polyclinic number," << endl;
     cout << "(I) Insert a new patient record," << endl;
     cout << "(R) Remove the patient record," << endl;
     cout << "(E) Exit the program," << endl;
     cout << "Your selection is:";
}

bool make_operation(char choice){
    bool terminate=false;

    switch(choice){

        case 'P': case 'p' :
            print_records();
            getchar();
            getchar();
            break;

        case 'B': case 'b' :
            if(!search_patients(true) ) //true for search by branch name
            {
                cout << "There is no patient in this branch or there is no such a branch!" << endl ;
            }
            getchar();
            getchar();
            break;

        case 'C': case 'c' :
            search_patients(false); //false for search by polyclinic number
            getchar();
            getchar();
            break;

        case 'I': case 'i' :
           insert_record();


            cout << "Person has been successfully inserted!" << endl ;
            getchar();
            getchar();
            break;

        case 'R': case 'r' :
            remove_record();
            cout << "Person has been successfully deleted!" << endl ;
            break;

        case 'E': case 'e' :
            cout << "Are you sure that you want to terminate the program? (Y/N):";
            cin >> choice;
            if(choice=='Y' || choice=='y')
                terminate=true;
                break;

        default:
            cout << "Error: You have made an invalid choice" << endl;
            cout << "Try again {P, B, C, I, R, E} :" ;
            cin >> choice;
            terminate = make_operation(choice);
            break;
    }

return terminate;
}

void open_file(char *text_name){

    database= fopen(text_name,"r+");
    if(!database){
        cerr<< "File cannot be opened" << endl;
        return;
    }
}

void read_file(){
    int cur=0;
    while(!feof(database)){
            if( fscanf(database , "%[^\t] %[^\t] %[^\t] %d %d %[^\n]\n",   //[^\t] it reads till tab character end at the end of line it ignores new line with \n
                                (ar+cur)->name,(ar+cur)->doctorName,(ar+cur)->diagnosis,&(ar+cur)->patientNumber,&(ar+cur)->polyclinicNumber,(ar+cur)->branchName) == 6) //if all datas is okay
            {

                cur++;
            }
    }
    numOfPeople = cur;
}

void print_records(){
for(int i=0;i<numOfPeople;i++){

  cout << ar[i].name << " " << ar[i].doctorName << " " << ar[i].diagnosis << " " << ar[i].patientNumber << " " << ar[i].polyclinicNumber << " " << ar[i].branchName << endl ;
}
}

void sort_file(){
    tmp=new patient_record[200];
    for(int i=1;i<numOfPeople ;i++){            //In this block it orders with bubble sort according to branchName
        for(int j=0;j<numOfPeople-i;j++){
            if(strcmp( (ar+j) -> branchName,(ar+j+1)->branchName)>0){
                *(tmp+j)=*(ar+j);
                *(ar+j)=*(ar+j+1);
                *(ar+j+1)=*(tmp+j);
               }
        }
    }

    for(int i=1;i<numOfPeople ;i++){            //In this block it orders with bubble sort according to polyclinicNumber
        for(int j=0;j<numOfPeople-i;j++){
            if(strcmp( (ar+j) -> branchName,(ar+j+1)->branchName) == 0 &&  ( (ar+j)->polyclinicNumber >= (ar+j+1)->polyclinicNumber) ){
                *(tmp+j)=*(ar+j);
                *(ar+j)=*(ar+j+1);
                *(ar+j+1)=*(tmp+j);
               }
        }
    }
}

void order_the_list(void){                     //This function trying to arrange every polyclinic for 10 patients,when it finds the appropriate location
int counter=0,temp;                            //it's calling another function.
    for(int i=0;i<numOfPeople;i++){
        if( (ar+i)->polyclinicNumber == 0 && counter!=0)
            break;
        if (counter == 0){
            temp=(ar+i)->polyclinicNumber;
            counter++;
            }
            else if( temp == (ar+i)->polyclinicNumber)
                    counter++;
                 else if(temp != (ar+i+1)->polyclinicNumber && (ar+i+1)->polyclinicNumber !=0){
                    push_right(i,counter);
                    i+=10-counter-1;
                    counter = 0;
                    }
    }
}

void push_right(int indis,int counter){     //According to order_the_list function it places null records

    counter = 10 - counter; //how many null patient will be
    numOfPeople+=counter;
        for(int i=numOfPeople;i>=indis;i--)
        {
            *(ar+i)=*(ar+i-counter);
            if(i<counter+indis)
            {
                (ar+i)->name[0] = '\0';
                (ar+i)->doctorName[0] = '\0';
                (ar+i)->diagnosis[0] = '\0';
                (ar+i)->patientNumber = NULL;
                (ar+i)->polyclinicNumber = NULL;
                (ar+i)->branchName[0] = '\0';
            }
        }

}

void push_one_left(int indis){   //Changing the empty record's place after the removing
    int i = indis;
    while( (ar+i+1)->name[0] != '\0' )
    {
        *(ar+i) =*(ar+i+1);
        i++;
        if((ar+i)->name[0] != '\0')
        {
            (ar+i)->name[0] = '\0';
                (ar+i)->doctorName[0] = '\0';
                (ar+i)->diagnosis[0] = '\0';
                (ar+i)->patientNumber = NULL;
                (ar+i)->polyclinicNumber = NULL;
                (ar+i)->branchName[0] = '\0';
        }



    }

}

bool search_patients(bool search_method){
    char * key = new char[40];
    int key_int;
    int counter=0;
    if(search_method==true) //Search by branch name if it's true
    {
        cout << "Please enter the name of branch which you want to search:";
        cin >> key ;

        for(int i=0;i<numOfPeople;i++)
        {
            if( strcmp( (ar+i)->branchName , key )== 0)
                {
                    cout << ar[i].name << " " << ar[i].doctorName << " " << ar[i].diagnosis << " " << ar[i].patientNumber << " " << ar[i].polyclinicNumber << " " << ar[i].branchName << endl ;
                    counter++;

                }

        }
          if(counter != 0) return true;
            else return false ;
    }
    else                //Search by polyclinicNumber if it's false
        {
            cout << "Please enter the polyclinic number which you want to search:" ;
            cin >> key_int;
             for(int i=0;i<numOfPeople;i++)
            {
                 if(  (ar+i)->polyclinicNumber  == key_int )
                {
                    cout  << ar[i].name << " " << ar[i].doctorName << " " << ar[i].diagnosis << " " << ar[i].patientNumber << " " << ar[i].polyclinicNumber << " " << ar[i].branchName << endl ;
                    counter++;

                }

            }
            if(counter != 0) return true;
            else return false ;
        }


    delete [] key;
}

void insert_record(){
    string tmp_name ,tmp_doctorName , tmp_diagnosis , tmp_branch ;
    char name[40],doctorName[40],diagnosis[40],branch[40];
    int tmp_patientNumber,tmp_polyclinicNumber,name_length;

    cout << "Enter name :";
    getline(cin>>ws,tmp_name);

     name_length= (int) tmp_name.size();
     for(int i=0;i<name_length+1;i++)
     {
         name[i]= tmp_name[i];
         if(i==name_length)
            name[i] = '\0';
     }

    cout << "Enter the doctor name :";
    getline(cin>>ws,tmp_doctorName);

        name_length= (int) tmp_doctorName.size();
     for(int i=0;i<name_length+1;i++)
     {
         doctorName[i]= tmp_doctorName[i];
         if(i==name_length)
            doctorName[i] = '\0';
     }

    cout << "Enter the diagnosis :";
    getline(cin>>ws,tmp_diagnosis);

        name_length= (int) tmp_diagnosis.size();
     for(int i=0;i<name_length+1;i++)
     {
         diagnosis[i]= tmp_diagnosis[i];
         if(i==name_length)
            diagnosis[i] = '\0';
     }

    cout << "Enter the patient number :";
    cin>>tmp_patientNumber;


        while( tmp_patientNumber < 1000 || tmp_patientNumber > 9999 )
        {
            cout << "You have to enter 4 digits number for patient number!" <<endl<< "Please enter patient number :" ;
            cin>>tmp_patientNumber;
        }

    cout << "Enter polyclinic number :";
    cin >> tmp_polyclinicNumber;

    cout << "Enter the branch :";
    getline(cin>>ws,tmp_branch);

    name_length= (int) tmp_branch.size();
     for(int i=0;i<name_length+1;i++)
     {
         branch[i]= tmp_branch[i];
         if(i==name_length)
            branch[i] = '\0';
     }

    int counter=0,location;



    for(int i=0;i<numOfPeople;i++)  //it searchs in every record for find the suitable place
    {



        if( strcmp( branch , (ar+i)->branchName) == 0 &&  tmp_polyclinicNumber == (ar+i)->polyclinicNumber  )  //If we find the same records we increase the counter
        {




                if(counter==0 && tmp_polyclinicNumber == (ar+i)->polyclinicNumber ){
                    location = i;
                    }

              counter ++;
        }
        else if(counter == 10 && tmp_polyclinicNumber != (ar+i)->polyclinicNumber )   //If in one polyclinic there are 10 patient we won't insert.
        {
            cout << "This polyclinic has reached maximum patient number (10).Inserting is canceling. " <<endl ;
            break;
        }

        else if(counter != 10 && counter != 0 && (ar+i)->name[0] == '\0' )      //If there is space to add we are adding and counter resets
            {
                strcpy( (ar+i)->name , name);
                strcpy( (ar+i)->doctorName , doctorName);
                strcpy( (ar+i)->diagnosis , diagnosis);
                (ar+i)->patientNumber =tmp_patientNumber;
                (ar+i)->polyclinicNumber = tmp_polyclinicNumber;
                strcpy( (ar+i)->branchName , branch ) ;
                counter = 0 ;
                break;
            }

    }

                numOfPeople++;
                strcpy( (ar+numOfPeople-1)->name , name );
                strcpy( (ar+numOfPeople-1)->doctorName , doctorName );
                strcpy( (ar+numOfPeople-1)->diagnosis , diagnosis );
                (ar+numOfPeople-1)->patientNumber =tmp_patientNumber;
                (ar+numOfPeople-1)->polyclinicNumber = tmp_polyclinicNumber;
                strcpy( (ar+numOfPeople-1)->branchName , branch ) ;
}

void remove_record(){
    int key;
    cout << "Please enter the patient number for delete the patient:" ;
    cin >> key ;
    for(int i=0;i<numOfPeople;i++)
    {
        if( key == (ar+i)->patientNumber ){             //Finds the patient and placing empty record
                (ar+i)->name[0] = '\0';
                (ar+i)->doctorName[0] = '\0';
                (ar+i)->diagnosis[0] = '\0';
                (ar+i)->patientNumber = NULL;
                (ar+i)->polyclinicNumber = NULL;
                (ar+i)->branchName[0] = '\0';

            if( (ar+i+1)->patientNumber != 0  )
                push_one_left(i);                    //Changing the place of the empty record according to right place

        }
     }
}

void update_close_textfile(){               // writes the records without empty records.
    fseek(database,0,SEEK_SET);

    for(int i=0;i<numOfPeople;i++)
        {
            if(i == numOfPeople-1 && (ar+i)->polyclinicNumber != 0 )
                fprintf(database,"%s\t%s\t%s\t%d\t%d\t%s",(ar+i)->name,(ar+i)->doctorName,(ar+i)->diagnosis,(ar+i)->patientNumber,(ar+i)->polyclinicNumber,(ar+i)->branchName);
           else  if((ar+i)->polyclinicNumber != 0)
                fprintf(database,"%s\t%s\t%s\t%d\t%d\t%s\n",(ar+i)->name,(ar+i)->doctorName,(ar+i)->diagnosis,(ar+i)->patientNumber,(ar+i)->polyclinicNumber,(ar+i)->branchName);

        }

    fclose(database);

}

void delete_all_arrays(){

        delete[] tmp;   //I have created only one block of elements so i have only 1 block elements to delete
        delete[] ar;
}







