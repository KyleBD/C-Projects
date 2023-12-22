#include "QueueList.h"

#include "Customer.h"

QueueList::QueueList() { head = nullptr; }

QueueList::QueueList(Customer* customer) { head = customer; }

QueueList::~QueueList() {
  delete head;
}

Customer* QueueList::get_head() {  
  return head;
}

void QueueList::enqueue(Customer* customer) {
  // a customer is placed at the end of the queue
  // if the queue is empty, the customer becomes the head
  Customer* temp_node = head;
  if(head == NULL){
    head = customer;
  }
  else{
    while(temp_node -> get_next() != NULL){
      temp_node = temp_node -> get_next(); //Traverse the que until stepping out of while loop
    }
    temp_node -> set_next(customer); // stepping out of while loops means end of list so set new node
  }
  return;
}

Customer* QueueList::dequeue() {
  // remove a customer from the head of the queue 
  // and return a pointer to it
  Customer* return_node = head;
  Customer* temp_node = NULL;
  if(head == NULL){ //Nothing to deque since the list is empty
    return NULL;
  }
  else{
    temp_node = return_node -> get_next();
    return_node -> set_next(nullptr);
    head = temp_node;
    return return_node;
  }
}

int QueueList::get_items() {
  // count total number of items each customer in the queue has
  Customer* cur_customer = head;
  int total_items = cur_customer -> get_numOfItems();
  int index = 0;
  if(cur_customer == nullptr){
    return total_items;
  }
  else{
    while(cur_customer -> get_next() != nullptr){
      if(index != 0){
        cur_customer = cur_customer -> get_next();
        total_items = total_items + cur_customer -> get_numOfItems();
      }
      index = index + 1;
    }
  }
  return total_items;
}

bool QueueList::empty_queue() {
  // if the queue is empty, return false
  // if the queue is not empty, delete all customers
  // and return true
  if(head = nullptr){
    return false;
  }
  else{
    delete head;
    return true;
  }
  
}

void QueueList::print() {
  // print customers in a queue
  Customer* temp = head;
  while (temp != nullptr) {
    temp->print();
    temp = temp->get_next();
  }
}
