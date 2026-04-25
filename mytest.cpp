#include <iostream>
#include "centcom.h"

const char * FAIL_STATEMENT = "*****TEST FAILED: ";
const char * PASS_STATEMENT = "     TEST PASSED: ";

using namespace std;

class Tester{
public:
bool testConstructor();
bool testaddElevator();
bool testsetUP();
bool testinsertFloor();
bool testpushButton();
bool testprocessNextRequest();
};

bool Tester::testConstructor(){
//Function: Centcom Constructor
//Normal case: Creating a valid centcom object
//Expected output: Should be creted with initialized objects
bool allPassed=true;
CentCom cent(4,3);
if (cent.m_id!=3 && cent.m_numElevators!=4)
{
    allPassed=false;
}
//Error case: Creating a CentCom objects with invalid values
//Expected ouput: Constructor should detect this behavior and initialize them to default values
CentCom cent2(-1,-5);
if (cent2.m_id!=0 && cent2.m_numElevators!=1)
{
    allPassed=false;
}
return allPassed;

}

bool Tester::testaddElevator(){
//Function:addElevator
//Normal Case: adding a new elevator
//Expected output: new elevator is added
bool allPassed=true;
CentCom cent(3,5);
allPassed = cent.addElevator(1, 5, 10); 
//Error case:Adding an elevcator with an invalid ID
//Expected output: Invalid elevator should not be added
CentCom cent2(3,4);
if(cent2.addElevator(5,2,1)){
    allPassed=true;
}
return allPassed;
}
bool Tester::testsetUP(){
bool allPassed=true;
//Function: setUP
//Error case: setting up an elevator where the first floor is greater than the last floor
//Expected output: The elevator would be failed to set up
Elevator anElv(3);
anElv.setUp(5,3);
if (anElv.m_bottom!=nullptr && anElv.m_top!=nullptr)
{
 allPassed=false;
}
return allPassed;
}
bool Tester::testinsertFloor(){
bool allPassed=true;
//Function:insertFloor
//Error case: Attempting to insert an invalid floor
//Expected output: The invalid floor should not be added
Elevator anElv(5);
anElv.setUp(2,5);
if(anElv.insertFloor(-2)){
    allPassed=false;
}
return allPassed;
}
bool Tester:: testpushButton(){
bool allPassed=true;
// Function: pushButton
//Normal case: request a valid floor number
//Expected result: the requested floor number should be added to the appropriacte list
Elevator anElv(4);
anElv.setUp(2,6);
anElv.pushButton(3);
Floor *current=anElv.m_upRequests;
while (current!=nullptr)
{
    if (current->m_floorNum==3)
    {
        allPassed=true;
        break;
    }
    current=current->m_next;
}
//Error case: Pushing a button on a secured floor
//Expected output: The function should return false
CentCom cont(1, 10); // Create a CentCom instance with 1 elevator
cont.addElevator(0, 1, 10); // Elevator 0: Floors 1-10   
cont.setSecure(0, 5, true); // Secure floor 5
Elevator *elv=cont.getElevator(0);
if (allPassed==elv->pushButton(5))
{
    allPassed=false;
}

return allPassed;
   
}
bool Tester:: testprocessNextRequest(){
bool allPassed=true;
//Function:processNextRequest
//Normal case:Processing a valid next request 
//Expected output: the current floor should be moved to the next request
CentCom cent(1,10);
cent.addElevator(0,1,10);

Elevator *elv= cent.getElevator(0);

elv->pushButton(3);
elv->pushButton(7);
elv->processNextRequest();
if (elv->m_currentFloor->m_floorNum!=3)
{
    allPassed=false;
}
//Error case: Attempting to process a process  a request that was not made
//Expected ouput: The function should return false
CentCom cent2(1,10);
cent2.addElevator(0,1,10);
Elevator *elv2= cent2.getElevator(0);
if (elv2->processNextRequest())
{
    allPassed=false;
}

return allPassed;

}



int main(){
Tester test;
 if (test.testConstructor())
{
    cout<<PASS_STATEMENT<<"Success. Correctly constructes object"<<endl;
}
 else{
    cout<<FAIL_STATEMENT<<"Failed to create object"<<endl;
} 
  if (test.testaddElevator())
{
    cout<<PASS_STATEMENT<<"Success. Correctly added elevator"<<endl;
}
else{
    cout<<FAIL_STATEMENT<<"Failed to add elevator"<<endl;
}  
if (test.testsetUP())
{

    cout<<PASS_STATEMENT<<"Success. Correctly set up elevator"<<endl;
}
else{
    cout<<FAIL_STATEMENT<<" Failed to set up elevator"<<endl;
}
 if (test.testinsertFloor())
{
    cout<<PASS_STATEMENT<<"Success. Correctly inserted floor"<<endl;
}
else{
    cout<<FAIL_STATEMENT<<"Failed to insert floor"<<endl;
}
 if (test.testpushButton())
{
    cout<<PASS_STATEMENT<<"Success. Correctly pushed button"<<endl;
}
else{
    cout<<FAIL_STATEMENT<<"Failed. incorrectly pushed button"<<endl;
}
 if (test.testprocessNextRequest())
{
    cout<<PASS_STATEMENT<<"Success. Correctly processed the next request"<<endl;
}
else{
    cout<<FAIL_STATEMENT<<"Failed. incorrectly processed next request"<<endl;
} 
    return 0;
  }    