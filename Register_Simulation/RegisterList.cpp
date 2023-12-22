#include "RegisterList.h"

#include <iostream>

#include "Register.h"
using namespace std;

RegisterList::RegisterList() {
  head = nullptr;
  size = 0;
}

RegisterList::~RegisterList() {
  Register* cur_node = head;
  Register* temp_node = NULL;
  if(head != nullptr){
    while(cur_node->get_next() != nullptr){
      temp_node = cur_node;
      delete temp_node;
      cur_node = cur_node->get_next();
    }
  }
  return;
}

Register* RegisterList::get_head() {  
  return head;
}

int RegisterList::get_size() { 
  // return number of registers 
  return size;
}


Register* RegisterList::get_min_items_register() {
  // loop all registers to find the register with least number of items
  Register* cur_node = head;
  Register* min_item_reg = cur_node;
  int min_items = cur_node -> get_queue_list() -> get_items();
  int index = 0;

  if(head == nullptr){ //In this case there are no registers open
    return NULL;
  }
  while(cur_node->get_next() != nullptr){
    if(index != 0){
      cur_node = cur_node -> get_next();
    }
    index = index + 1;
    if((cur_node -> get_queue_list() -> get_items()) < min_items){
      min_item_reg = cur_node;
      min_items = cur_node -> get_queue_list() -> get_items();
    }
  }
  return min_item_reg;
}

Register* RegisterList::get_free_register() {
  // return the register with no customers
  // if all registers are occupied, return nullptr
  Register* temp_reg = head;
  if(head == nullptr){
    return nullptr;
  }
  else if(head -> get_queue_list() -> get_head() == nullptr){
    return head;
  }
  else{
    while(temp_reg -> get_next() != nullptr){
      temp_reg = temp_reg -> get_next();
      if((temp_reg -> get_queue_list() -> get_head()) == nullptr){
        return temp_reg;
      }
    }
    return nullptr;
  }

}

void RegisterList::enqueue(Register* newRegister) {
  // a register is placed at the end of the queue
  // if the register's list is empty, the register becomes the head
  // Assume the next of the newRegister is set to null
  // You will have to increment size 
  Register* temp_register = head;
  if(head == nullptr){
    head = newRegister;
    size = size +1;
  }
  else{
    while(temp_register -> get_next() != NULL){
      temp_register = temp_register -> get_next(); //traverse through the linked list
    }
    temp_register -> set_next(newRegister);
    size = size + 1;
  }
  return;
}

bool RegisterList::foundRegister(int ID) {
  // look for a register with the given ID
  // return true if found, false otherwise
  Register* temp_register = head;
  if(head == nullptr){
    return false;
  }
  if(head -> get_ID() == ID){
    return true;
  }
  else{
    while(temp_register -> get_next() != nullptr){
      temp_register = temp_register -> get_next();
      if(temp_register -> get_ID() == ID){
        return true;
      }
    }
    return false;
  }
}

Register* RegisterList::dequeue(int ID) {
  // dequeue the register with given ID

  // return the dequeued register
  // return nullptr if register was not found
  Register* temp_register = head;
  Register* prev_reg = NULL;
  Register* return_reg = NULL;
  if(head == nullptr){
    cout << "No registers to deque"<< endl;
    return nullptr;
  }
  else if(head -> get_ID() == ID){
    temp_register = head->get_next();
    return_reg = head;
    head = nullptr;
    head = temp_register;
    size = size - 1;
    return return_reg;
  }
  else{
    while(temp_register -> get_next() != NULL){
      if(temp_register -> get_ID() == ID){ //Want to delete temp and set the list around it 
        prev_reg -> set_next(temp_register -> get_next());
        temp_register -> set_next(nullptr);
        size = size - 1;
        return temp_register;
      }
      prev_reg = temp_register;
      temp_register = temp_register -> get_next();
    }
    return nullptr;
  }
}

Register* RegisterList::calculateMinDepartTimeRegister(double expTimeElapsed) {
  // return the register with minimum time of departure of its customer
  // if all registers are free, return nullptr
  Register* temp_reg = head;
  bool all_free = true;
  if(head == NULL){
    cout << "No registers open cant calculate time elapsed" << endl;
    return nullptr;
  }//First check if all the registers are free
  else{
    while(temp_reg -> get_next() != nullptr){
      if((temp_reg -> get_queue_list() -> get_head()) != nullptr){
        all_free = false;
      }
      temp_reg = temp_reg -> get_next();
    }
  }

  temp_reg = head;
  Register* min_time_reg = head;
  double new_time = 0;
  double min_time = ((temp_reg -> get_secPerItem()) * (temp_reg -> get_queue_list() -> get_items())) + (temp_reg -> get_overheadPerCustomer());
  if(!all_free){
    while(temp_reg -> get_next() != nullptr){
      new_time = ((temp_reg -> get_secPerItem()) * (temp_reg -> get_queue_list() -> get_items())) + (temp_reg -> get_overheadPerCustomer());
      if(new_time < min_time){
        min_time_reg = temp_reg;
      }
      temp_reg = temp_reg -> get_next();
    }
    return min_time_reg;
  }
  if(all_free){
    return nullptr;
  }
  return nullptr;
}

void RegisterList::print() {
  Register* temp = head;
  while (temp != nullptr) {
    temp->print();
    temp = temp->get_next();
  }
}
