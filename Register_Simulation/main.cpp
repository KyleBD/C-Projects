#include <cmath>
#include <cstdlib>
#include <iostream>
#include <sstream>

#include "Customer.h"
#include "QueueList.h"
#include "Register.h"
#include "RegisterList.h"

using namespace std;

string getMode();

// Register
void parseRegisterAction(stringstream &lineStream, string mode);
void openRegister(
    stringstream &lineStream,
    string mode);  // register opens (it is upto customers to join)
void closeRegister(stringstream &lineStream,
                   string mode);  // register closes 

// Customer
void addCustomer(stringstream &lineStream,
                 string mode);  // customer wants to join


// Helper functions
bool getInt(stringstream &lineStream, int &iValue);
bool getDouble(stringstream &lineStream, double &dValue);
bool foundMoreArgs(stringstream &lineStream);

// Global variables
RegisterList *registerList; // holding the list of registers
QueueList *doneList; // holding the list of customers served
QueueList *singleQueue; // holding customers in a single virtual queue
double expTimeElapsed; // time elapsed since the beginning of the simulation

void check_customer_done(string mode);
Register* least_item_register();
void check_multiple_departures();
Register* find_first_done();
// List of commands:
// To open a register
// register open <ID> <secPerItem> <setupTime> <timeElapsed>
// To close register
// register close <ID> <timeElapsed>
// To add a customer
// customer <items> <timeElapsed>

int main() {
  registerList = new RegisterList();
  doneList = new QueueList();
  singleQueue = new QueueList();
  expTimeElapsed = 0;

  // Set mode by the user
  string mode = getMode();

  string line;
  string command;

  cout << "> ";  // Prompt for input
  getline(cin, line);

  while (!cin.eof()) {
    stringstream lineStream(line);
    lineStream >> command;
    if (command == "register") {
      parseRegisterAction(lineStream, mode);
    } else if (command == "customer") {
      addCustomer(lineStream, mode);
    } 

    else {
      cout << "Invalid operation" << endl;
    }
    cout << "> ";  // Prompt for input
    getline(cin, line);
  }
  if(doneList -> get_head() != nullptr){
  Customer* temp_customer = doneList -> get_head();
  int index = 0;
  int index_2 = 0;
  double wait_time = 0;
  double max_wait_time = temp_customer ->get_departureTime() - temp_customer -> get_arrivalTime();
  double standard_deviation = 0;
  double average_wait_time = 0;
  int number_of_customers = 0;
  double diff_wait_time = 0;
  if(temp_customer -> get_next() == nullptr){
    wait_time = temp_customer ->get_departureTime() - temp_customer -> get_arrivalTime();
    cout << endl <<  "Finished at time " << expTimeElapsed << endl;
    cout<< "Statistics:" << endl;
    cout << "Maximum wait time: " << wait_time << endl;
    cout << "Average wait time: " << wait_time << endl;
    diff_wait_time = wait_time - wait_time;
    standard_deviation = sqrt((diff_wait_time * diff_wait_time));
    cout << "Standard Deviation of wait time: " << standard_deviation << endl;
  }
  else{
    while(temp_customer -> get_next() != nullptr){
      if(index != 0){
        temp_customer = temp_customer -> get_next();
      }
      index = index + 1;
      number_of_customers = number_of_customers + 1;
      wait_time = temp_customer ->get_departureTime() - temp_customer -> get_arrivalTime();
      average_wait_time = average_wait_time + wait_time;
      if(max_wait_time < wait_time){
        max_wait_time = wait_time;
      }
         
  }
  average_wait_time = average_wait_time / number_of_customers;
  wait_time = 0;  
  number_of_customers = 0;
  temp_customer = doneList -> get_head();
    while(temp_customer -> get_next() != nullptr){
      if(index_2 != 0){
        temp_customer = temp_customer -> get_next();
      }
      index_2 = index_2 + 1;
      wait_time = temp_customer ->get_departureTime() - temp_customer -> get_arrivalTime();
      number_of_customers = number_of_customers + 1;
      diff_wait_time =  wait_time - average_wait_time;
      standard_deviation = standard_deviation + (diff_wait_time * diff_wait_time);
    }
    standard_deviation = sqrt(standard_deviation/number_of_customers);
    cout << endl << "Finished at time " << expTimeElapsed << endl;
    cout<< "Statistics:" << endl;
    cout << "Maximum wait time: " << max_wait_time << endl;
    cout << "Average wait time: " << average_wait_time << endl;
    cout << "Standard Deviation of wait time: " << standard_deviation << endl;
  }
  }
  else{
    cout << endl << "Finished at time " << expTimeElapsed << endl;
    cout<< "Statistics:" << endl;
    cout << "Maximum wait time: " << "0" << endl;
    cout << "Average wait time: " << "-nan" << endl;
    cout << "Standard Deviation of wait time: " << "-nan" << endl;
  }
  delete singleQueue;

  // You have to make sure all dynamically allocated memory is freed 
  // before return 0
  return 0;
}

string getMode() {
  string mode;
  cout << "Welcome to ECE 244 Grocery Store Queue Simulation!" << endl;
  cout << "Enter \"single\" if you want to simulate a single queue or "
          "\"multiple\" to "
          "simulate multiple queues: \n> ";
  getline(cin, mode);

  if (mode == "single") {
    cout << "Simulating a single queue ..." << endl;
  } else if (mode == "multiple") {
    cout << "Simulating multiple queues ..." << endl;
  }

  return mode;
}

void addCustomer(stringstream &lineStream, string mode) {
  int items;
  double timeElapsed;
  if (!getInt(lineStream, items) || !getDouble(lineStream, timeElapsed)) {
    cout << "Error: too few arguments." << endl;
    return;
  }
  if (foundMoreArgs(lineStream)) {
    cout << "Error: too many arguments." << endl;
    return;
  }

  // Depending on the mode of the simulation (single or multiple),
  // add the customer to the single queue or to the register with
  // fewest items
  //for single mode we will want to first check if there are an free registers if not add to queue
  //if Free send to register use get_free_register method of register list class
  expTimeElapsed = expTimeElapsed + timeElapsed;
  check_customer_done(mode);
  cout << "A customer entered" << endl;
  Customer* customer = new Customer(expTimeElapsed, items);
  QueueList* temp_queue = nullptr;
  Register* temp_register = nullptr;
  if(mode == "single"){
    if(registerList -> get_free_register() == nullptr){
      singleQueue->enqueue(customer);
      cout << "No free registers"<< endl;
      return;
    }
    else{
      temp_register = registerList -> get_free_register();
      temp_queue = temp_register -> get_queue_list();
      temp_queue -> enqueue(customer);
      cout << "Queued a customer with free register " << temp_register -> get_ID() << endl;
      double test = temp_register -> calculateDepartTime();
      return;
    }

  }
  //for multiple registers we will want to send to fastest register using 
  if(mode == "multiple"){
    if(registerList -> get_free_register() == nullptr){
    temp_register = registerList -> get_min_items_register();
    }
    else{
      temp_register = registerList -> get_free_register();
    }
    temp_queue = temp_register -> get_queue_list();
    temp_queue -> enqueue(customer);
    cout << "Queued a customer with quickest register " << temp_register -> get_ID() << endl;
    if(temp_register ->get_queue_list() -> get_head() -> get_departureTime() == -1){
      double temp_depart = 0;
      temp_depart = temp_register -> calculateDepartTime();
    }
    return;
  }
  
}

void parseRegisterAction(stringstream &lineStream, string mode) {
  string operation;
  lineStream >> operation;
  if (operation == "open") {
    openRegister(lineStream, mode);
  } else if (operation == "close") {
    closeRegister(lineStream, mode);
  } else {
    cout << "Invalid operation" << endl;
  }
}

void openRegister(stringstream &lineStream, string mode) {
  int ID;
  double secPerItem, setupTime, timeElapsed;
  // convert strings to int and double
  if (!getInt(lineStream, ID) || !getDouble(lineStream, secPerItem) ||
      !getDouble(lineStream, setupTime) ||
      !getDouble(lineStream, timeElapsed)) {
    cout << "Error: too few arguments." << endl;
    return;
  }
  if (foundMoreArgs(lineStream)) {
    cout << "Error: too many arguments" << endl;
    return;
  }
  // Check if the register is already open
  // If it's open, print an error message
  // Otherwise, open the register
  // If we were simulating a single queue, 
  // and there were customers in line, then 
  // assign a customer to the new register
  Customer* temp_customer = nullptr;
  QueueList* temp_queue = nullptr;

  if(registerList -> foundRegister(ID) && registerList -> get_head() != nullptr){
    cout << "Error: register " << ID << " is already open" << endl;
    return;
  }
  else{
    expTimeElapsed = expTimeElapsed + timeElapsed;
    check_customer_done(mode);
    Register* register_ = new Register(ID, secPerItem, setupTime, expTimeElapsed);
    registerList -> enqueue(register_);
    cout << "Opened register " << ID << endl;
    if(mode == "single"){
      if(singleQueue -> get_head() == nullptr){ //the head of the single que is null so there is going to be noone in that queue
        return;
      }
      else{
          temp_customer = singleQueue -> dequeue();
          temp_queue = register_ -> get_queue_list();
          double temp_new_depart_time = register_ -> get_availableTime() + register_ -> get_secPerItem() * temp_customer ->get_numOfItems() + register_ -> get_overheadPerCustomer(); 
          temp_customer ->set_departureTime(temp_new_depart_time);
          temp_queue -> enqueue(temp_customer);
          cout << "Queued a customer with free register " << ID << endl;
          return; 
      }
    }
    else{ //in this case mode is multiple  
      return;
    }
  }
  return;
}

void closeRegister(stringstream &lineStream, string mode) {
  int ID;
  double timeElapsed;
  // convert string to int
  if (!getInt(lineStream, ID) || !getDouble(lineStream, timeElapsed)) {
    cout << "Error: too few arguments." << endl;
    return;
  }
  if (foundMoreArgs(lineStream)) {
    cout << "Error: too many arguments" << endl;
    return;
  }
  // Check if the register is open
  // If it is open dequeue it and free it's memory
  // Otherwise, print an error message 

  Register* temp_register = nullptr;
  if(registerList -> get_head() != nullptr){
    if(registerList -> foundRegister(ID)){
      expTimeElapsed = expTimeElapsed + timeElapsed;
      check_customer_done(mode);
      temp_register = registerList-> dequeue(ID);
      delete temp_register;
      cout << "Closed register " << ID << endl;
      return;
    }
    else{
      cout << "Error: register  " << ID << " is not open" << endl;
      return;
    }
  }
  else{
    cout << "Error: register  " << ID << " is not open" << endl;
  }
  
}

bool getInt(stringstream &lineStream, int &iValue) {
  // Reads an int from the command line
  string command;
  lineStream >> command;
  if (lineStream.fail()) {
    return false;
  }
  iValue = stoi(command);
  return true;
}

bool getDouble(stringstream &lineStream, double &dvalue) {
  // Reads a double from the command line
  string command;
  lineStream >> command;
  if (lineStream.fail()) {
    return false;
  }
  dvalue = stod(command);
  return true;
}

bool foundMoreArgs(stringstream &lineStream) {
  string command;
  lineStream >> command;
  if (lineStream.fail()) {
    return false;
  } else {
    return true;
  }
}

Register* least_item_register(){
  Register* temp_register = registerList -> get_head();
  Register* return_register = temp_register;
  int min_items = temp_register -> get_queue_list() -> get_items();
  if(registerList -> get_head() == nullptr){
    cout << "No min item register since there are no registers";
    return nullptr;
  }
  if((registerList -> get_head() -> get_next()) == nullptr){
    return registerList -> get_head();
  }
  else{
    while(temp_register -> get_next() != nullptr){
      if(temp_register -> get_queue_list() -> get_items() < min_items){
        return_register = temp_register;
      }
      temp_register = temp_register -> get_next();
    } 
    return return_register;
  }
}

void check_customer_done(string mode){
  Register* single_temp_reg = registerList -> get_head();
  if(registerList -> get_head() == nullptr){
    return;
  }
  else if(single_temp_reg -> get_next() == nullptr){
    if(mode == "single" && single_temp_reg ->get_queue_list() -> get_head() != nullptr){
      Customer* temp_single_customer = nullptr;
      if(single_temp_reg -> get_queue_list() -> get_head() -> get_departureTime() <= expTimeElapsed){
        temp_single_customer = single_temp_reg -> get_queue_list() -> dequeue();
        single_temp_reg -> set_availableTime(temp_single_customer -> get_departureTime());
        doneList ->enqueue(temp_single_customer); 
        cout << "Departed a customer at register ID " << single_temp_reg -> get_ID() << " at " << temp_single_customer ->get_departureTime() << endl;
        if(singleQueue -> get_head() != nullptr){
          temp_single_customer = singleQueue -> dequeue();
          double temp_new_depart_time = single_temp_reg -> get_availableTime() + single_temp_reg -> get_secPerItem() * temp_single_customer ->get_numOfItems() + single_temp_reg -> get_overheadPerCustomer();
          temp_single_customer -> set_departureTime(temp_new_depart_time);
          single_temp_reg -> get_queue_list() -> enqueue(temp_single_customer);
          single_temp_reg -> set_availableTime(temp_new_depart_time);
          cout << "Queued a customer with free register " << single_temp_reg -> get_ID() << endl;
          //check now if the new qued customer is greter than total time
          //this lends nicely to recursivley checking if the new customer is done
          check_customer_done(mode);
        }
      }
    }
    else{//iterate through to find customers that are done.
      if(single_temp_reg ->  get_queue_list() -> get_head() != nullptr){
        Customer* temp_multiple_customer = nullptr;
        Customer* new_customer = nullptr;
        if(single_temp_reg -> get_queue_list() -> get_head() -> get_departureTime() <= expTimeElapsed){
          temp_multiple_customer = single_temp_reg -> get_queue_list() -> dequeue();
          single_temp_reg -> set_availableTime(temp_multiple_customer -> get_departureTime());
          doneList -> enqueue(temp_multiple_customer);
          cout << "Departed a customer at register ID " << single_temp_reg -> get_ID() << " at " <<temp_multiple_customer -> get_departureTime() << endl;

          if(single_temp_reg -> get_queue_list() -> get_head() != nullptr){
            new_customer = single_temp_reg -> get_queue_list() -> get_head();
            double temp_new_depart_time = single_temp_reg -> get_availableTime() + single_temp_reg -> get_secPerItem() * new_customer ->get_numOfItems() + single_temp_reg -> get_overheadPerCustomer(); 
            new_customer -> set_departureTime(temp_new_depart_time);    
          }
          check_customer_done(mode);
        }
      }
    }
  }
  else{ //iterate through the registers and check if customer is done
    Register* temp_register = registerList -> get_head();
    Customer* temp_customer = nullptr;
    QueueList* temp_done_queue = nullptr;
    int index = 0;
    if(mode == "single"){
      while(find_first_done() != nullptr){
          temp_register = find_first_done();
          temp_customer = temp_register -> get_queue_list() -> dequeue();
          temp_register -> set_availableTime(temp_customer -> get_departureTime());
          doneList ->enqueue(temp_customer); 
          cout << "Departed a customer at register ID " << temp_register -> get_ID() << " at " << temp_customer ->get_departureTime() << endl;
          if(singleQueue -> get_head() != nullptr){ //now take customer from the single queue and put them in the now open register
            temp_customer = singleQueue -> dequeue();
            double temp_new_depart_time = temp_register -> get_availableTime() + temp_register -> get_secPerItem() * temp_customer ->get_numOfItems() + temp_register -> get_overheadPerCustomer();
            temp_customer -> set_departureTime(temp_new_depart_time);
            temp_register -> get_queue_list() -> enqueue(temp_customer);
            temp_register -> set_availableTime(temp_new_depart_time);
            cout << "Queued a customer with free register " << temp_register -> get_ID() << endl;
          }
      }
    }
    else{
    Register* temp_register = registerList -> get_head();
    Customer* temp_customer = nullptr;
    Customer* new_customer = nullptr;
      while(find_first_done() != nullptr){
          temp_register = find_first_done();
          temp_customer = temp_register -> get_queue_list() -> dequeue();
          temp_register -> set_availableTime(temp_customer -> get_departureTime());
          doneList ->enqueue(temp_customer); 
          cout << "Departed a customer at register ID " << temp_register -> get_ID() << " at " << temp_customer ->get_departureTime() << endl;
          if(temp_register -> get_queue_list() -> get_head() != nullptr){
            new_customer = temp_register -> get_queue_list() -> get_head();
            double temp_new_depart_time = temp_register -> get_availableTime() + temp_register -> get_secPerItem() * new_customer ->get_numOfItems() + temp_register -> get_overheadPerCustomer(); 
            new_customer -> set_departureTime(temp_new_depart_time);    
        }
      }
    }
  }
}
Register* find_first_done(){
  Register* temp_register = registerList -> get_head();
  Register* return_register = nullptr;
  double min_return_time = 0;
  bool checked = false;
  if(temp_register -> get_queue_list() ->get_head() != nullptr){
    if(temp_register -> get_queue_list() -> get_head() -> get_departureTime() <= expTimeElapsed){
      return_register = temp_register;
      min_return_time = temp_register -> get_queue_list() -> get_head() -> get_departureTime();
      checked = true;
    }
  }

  while(temp_register -> get_next() != nullptr){
    temp_register = temp_register -> get_next();
    if(temp_register-> get_queue_list() ->get_head() != nullptr){
      if(temp_register -> get_queue_list() -> get_head() -> get_departureTime() <= expTimeElapsed){
          if(min_return_time > temp_register -> get_queue_list() -> get_head() -> get_departureTime() || !checked){
            return_register = temp_register;
            min_return_time = temp_register -> get_queue_list() -> get_head() -> get_departureTime();
            checked = true;
          }

      }
    }
  }
  return return_register;
}
