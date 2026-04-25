// UMBC - CMSC 341 - Spring 2025 - Proj1
#include "centcom.h"
CentCom::CentCom(int numElevators,int buildingID){
    buildingID>=0 ? m_id=buildingID: m_id=0; // Initialize building id and make sure its not negative
    numElevators>0 ? m_numElevators= numElevators : m_numElevators=1; //Initialize number of elevators and make sure its not negative
    m_elevatorsList = new Elevator *[m_numElevators]; //allocate memory for th 2d array

    for (int i = 0; i < m_numElevators; i++)
    {
     m_elevatorsList[i]=nullptr; // Initialize all to nullptr
    }
    
}
CentCom::~CentCom(){
    if (m_elevatorsList!=nullptr)
    {
        for (int i = 0; i < m_numElevators; i++) // Iterate through the elevators pointers
        {
            delete m_elevatorsList[i]; //Delete every elevator pointer
        }
        delete [] m_elevatorsList; //Delete the array
    }
    
}
bool CentCom::addElevator(int ID, int bottomFloor, int topFloor){
    if (ID==INVALIDID||ID>=m_numElevators)
    {
        return false; //return false if the id passed in is invalid
    }
    if (m_elevatorsList[ID]!=nullptr) //Ensure the pointer isnt already pointing to something
    {
        delete m_elevatorsList[ID];
    }
    
    m_elevatorsList[ID]=new Elevator(ID); // Create the new elevator
    m_elevatorsList[ID]->setUp(bottomFloor, topFloor); // set up the new elevator
    return true;   
}
Elevator* CentCom::getElevator(int ID){
    for (int i = 0; i < m_numElevators; i++) //Iterate through the elevators
    {
        if (m_elevatorsList[i]!=nullptr && m_elevatorsList[i]->m_id==ID)
        {
           return m_elevatorsList[i]; //Return the elevator
        }
       
    }   
    return nullptr;
}
bool CentCom::setSecure(int ID, int floorNum, bool yes_no){
    if (ID==INVALIDID|| ID>=m_numElevators|| m_elevatorsList[ID]==nullptr) //If the id is invalid return false
    {
        return false;
    }
    Elevator *ptr= m_elevatorsList[ID];
    Floor *current= ptr->m_bottom;
    while (current!=nullptr) //iterate through the linked list
    {
        if (current->m_floorNum==floorNum) //Find desired floor number
        {
            current->m_secured=yes_no; //Set its secured state to true or false
            return true;
        }
        current=current->m_next;
    }
    return false;
}
bool CentCom::clearEmergency(int ID){
    if (ID==INVALIDID || ID>=m_numElevators|| m_elevatorsList[ID]==nullptr) //If the id is invalid return flase
    {
        return false;
    }
    for (int i = 0; i < m_numElevators; i++) //Iterate through the elevator list
    {
        if (m_elevatorsList[i]->m_id==ID) //Find the desired ID
        {
            m_elevatorsList[i]->m_emergency=false; //Clear the emergency
        }
        
    }
   return true; 
}
Elevator::Elevator(int ID){ 
//Construct elevator objects
    m_id=ID;
    m_load=0;
    m_emergency=false;
    m_bottom=nullptr;
    m_top=nullptr;
    m_currentFloor=nullptr;
    m_downRequests=nullptr;
    m_upRequests=nullptr;
    m_moveState=IDLE;
    m_doorState=CLOSED;
    
    
    
}
Elevator::~Elevator(){
   clear(); // Call clear
   
}
void Elevator::clear() {
    Floor* current; //Iterate through the elevator and delet all floor nodes
    current = m_bottom;
    while (current!=nullptr) {
        Floor* temp = current;
        current = current->m_next;
        delete temp;
    }
    m_bottom = m_top = nullptr; //set to nullptr
    
    current = m_upRequests; //Iterate through the uprequest linked list and delete all nodes
    while (current!=nullptr) {
        Floor* temp = current;
        current = current->m_next;
        delete temp;
    }
    m_upRequests = nullptr; //set to nullptr

    current = m_downRequests;//Iterate through the downrequest linked list and delete all nodes
    while (current!=nullptr) {
        Floor* temp = current;
        current = current->m_next;
        delete temp;
    }
    m_downRequests = nullptr;//set to nullptr
}
void Elevator::setUp(int firstFloor, int lastFloor){
    if (firstFloor>=lastFloor)// ensure the set up is valid
    {
        return;
    }
    if(m_bottom==nullptr){
        //Create first and list floor
    Floor *first = new Floor(firstFloor); 
    Floor *last = new Floor(lastFloor);

    m_bottom=first;
    m_top=last;
    Floor *prev=first;
    
    for (int i = firstFloor+1; i <= lastFloor; i++)
    {
        //Create floors in between first and last
        Floor *ptr = new Floor(i);
        prev->m_next=ptr;
        ptr->m_previous=prev;
        prev=ptr;
    }
        prev->m_next=last;
        last->m_previous=prev;
        m_currentFloor=first;
    }
}
bool Elevator::insertFloor(int floor) {
    if (floor != m_bottom->m_floorNum - 1) { //Check for invalid floor number
        return false;
    }
    Floor* newFloor = new Floor(floor); //Create new floor

    newFloor->m_next = m_bottom; //make new floor the head of the list
    if (m_bottom != nullptr) {
        m_bottom->m_previous = newFloor; // if the list is empty make the newfloor the only node
    }
    m_bottom = newFloor;

    return true;
}

bool Elevator::checkSecure(int floor){
   Floor *current=m_bottom;
   while (current!=nullptr) //Iterate through the list
   {
    current=current->m_next;
    if (current->m_floorNum==floor)
     {
        if (current->m_secured) //If the floor is secured
        {
            return true; 
        }
        
     }
    
   }
   return false;
    
}
void Elevator::pushEmergency(bool pushed){
    //If true set to true, if false set to false
    if (pushed) 
    {
        m_emergency=true;
    }
    else{
        m_emergency=false;
    }
}
bool Elevator::pushButton(int floor) {
    Floor* current = m_bottom;
    // Traverse the floors to find if the floor exists
    while (current != nullptr) {
        if (current->m_floorNum == floor) { 
            if (current->m_secured) {
                return false; // Floor is secured
            }
        }
        current=current->m_next;
    }
            //Iterate through up request to ensure its not already there
            Floor* temp = m_upRequests;
            while (temp!=nullptr) {
                if (temp->m_floorNum == floor) 
                {
                    return false;
                } 
                temp = temp->m_next;
            }
            //Iterate through down request to ensure its not already there
            Floor *temp2= m_downRequests;
            while (temp2!=nullptr) {
                if (temp2->m_floorNum == floor)
                {
                    return false;
                } 
                temp2 = temp2->m_next;
            }

            // Allocate memory for the new floor request
            Floor* insertFloor = new Floor(floor);

            if (m_currentFloor) {  // UP request
                    m_moveState = UP;
                if (m_upRequests==nullptr || m_upRequests->m_floorNum > floor) {
                    insertFloor->m_next = m_upRequests;
                    if (m_upRequests) m_upRequests->m_previous = insertFloor;
                    m_upRequests = insertFloor;
                } else {
                    Floor* temp = m_upRequests;
                    while (temp->m_next && temp->m_next->m_floorNum < floor) {
                        temp = temp->m_next;
                    }
                    insertFloor->m_next = temp->m_next;
                    if (temp->m_next) temp->m_next->m_previous = insertFloor;
                    temp->m_next = insertFloor;
                    insertFloor->m_previous=temp;
                }
            } else {  // DOWN request
                if (m_moveState == IDLE) m_moveState = DOWN;

                if (!m_downRequests || m_downRequests->m_floorNum < floor) {
                    insertFloor->m_next = m_downRequests;
                    if (m_downRequests) m_downRequests->m_previous = insertFloor;
                    m_downRequests = insertFloor;
                } else {
                    Floor* temp = m_downRequests;
                    while (temp->m_next && temp->m_next->m_floorNum > floor) {
                        temp = temp->m_next;
                    }
                    insertFloor->m_next = temp->m_next;
                    insertFloor->m_previous = temp;
                    if (temp->m_next) temp->m_next->m_previous = insertFloor;
                    temp->m_next = insertFloor;
                }
            }
            return true; // Successfully added request
    if (!insertFloor)
    {
        delete insertFloor;
        return false;
    }
    
    
    return false; // Floor not found
}


bool Elevator::processNextRequest() {
    //ensure load is ot at limit and not emergency
    if (m_emergency || m_load == LOADLIMIT) {
        return false; 
    }

    Floor* request = nullptr;

    //  Determine the movestate of next request
    if (m_moveState == UP && m_upRequests != nullptr) {
        request = m_upRequests;  
        m_upRequests = m_upRequests->m_next; // move to the next
        if (m_upRequests) {
            m_upRequests->m_previous = nullptr;  // Update pointer
        }
    } 
    else if (m_moveState == DOWN && m_downRequests != nullptr) {
        request = m_downRequests;  
        m_downRequests = m_downRequests->m_next; // move to the next
        if (m_downRequests) {
            m_downRequests->m_previous = nullptr;  // Update pointer
        }
    } 
    else {
        return false; // No requests to process
    }

    // set current floor to the request
    m_currentFloor = request;

    
    if (m_upRequests == nullptr && m_downRequests == nullptr) {
        m_moveState = IDLE;  // if no more request make the movestate idle
    }

    return true; 
}

void Elevator::enter(int load){
    m_load+=load; //Incriment the load
    if (m_load>LOADLIMIT)
    {
        m_load=LOADLIMIT;
    }
    
}
int Elevator::exit(int load){
    m_load-=load; //Decriment the load
    if (m_load<0)
    {
        return 0;
    }
    else{
        return m_load;
    }
}
void Elevator::dump(){
    if (m_moveState == IDLE) cout << "Elevator " << m_id << " is idle.";
    else if (m_moveState == UP) cout << "Elevator " << m_id << " is moving up.";
    else if (m_moveState == DOWN) cout << "Elevator " << m_id << " is moving down.";
    cout << endl;
    if (m_emergency == true) cout << "someone pushed the emergency button!" << endl;
    if (m_top != nullptr){
        Floor *temp = m_top;
        cout << "Top" << endl;
        while(temp->m_previous != nullptr){
            cout << temp->m_floorNum;
            if (temp->m_floorNum == m_currentFloor->m_floorNum) cout << " current ";
            if (temp->m_secured == true) cout << " secured ";
            cout << endl;
            temp = temp->m_previous;
        }
        cout << temp->m_floorNum;
        if (temp->m_floorNum == m_currentFloor->m_floorNum) cout << " current ";
        if (temp->m_secured == true) cout << " secured ";
        cout << endl;
        cout << "Bottom" << endl; 
    }
}