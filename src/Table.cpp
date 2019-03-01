//
// Created by ariel on 11/9/18.
//

#include "Table.h"
#include <vector>
#include <iostream>

Table::Table(int t_capacity) : capacity(t_capacity), bill(0), open(false), customersList(), orderList() {
}

void Table::copy(const Table &other) {
    capacity=other.getCapacity();
    bill=other.bill;
    open=other.open;
    for (size_t i=0; i<other.customersList.size(); i++){
        customersList.push_back(other.customersList[i]->myType());
    }

    for (size_t i=0; i<other.orderList.size(); i++){
        orderList.push_back(OrderPair(other.orderList[i].first,Dish(other.orderList[i].second)));
    }


}
// Copy constructor
Table::Table(const Table &other) : capacity(), bill(), open(), customersList(), orderList(){
    copy(other);
}
// Copy assignment operator
Table &Table::operator=(const Table &other) {
    //std::cout<<"Copy assignment operator called in Table\n";
    if (&other !=this){
        clean();
        copy(other);
    }
    return *this;
    }

void Table::steal(Table& other) {
    capacity=other.getCapacity();
    bill=other.bill;
    open=other.open;
    customersList = other.customersList;
    for (size_t i=0; i<other.orderList.size(); i++){
        orderList.push_back(OrderPair(other.orderList[i].first,Dish(other.orderList[i].second)));
    }
    other.orderList.clear();
    for (size_t j = 0; j < other.customersList.size(); ++j) {
        other.customersList[j]= nullptr;
    }
    other.customersList.clear();
}
// Move constructor
Table::Table(Table&& other) : capacity(), bill(), open(), customersList(), orderList(){
    steal(other);
}
// Move assignment operator
Table &Table::operator=(Table&& other) {
    clean();
    steal(other);
    return *this;
}


int Table::clean() { // this functions returns (-1) if clean wasn't successful, or the bill.
    if (!isOpen()){
        return -1;
    }
    int bill_to_return= bill;
    for (size_t i=0;i<customersList.size();i++) {
        delete customersList[i];
        customersList[i] = nullptr;
    }
    customersList.clear();
    orderList.clear();
    bill=0;
    closeTable();
    return bill_to_return;
}

Table::~Table() {
    for (size_t i=0;i<customersList.size();i++){
        delete customersList[i];
        customersList[i]= nullptr;
    }
    customersList.clear();
    orderList.clear();

}

int Table::getCapacity() const {
    return capacity;
}

void Table::addCustomer(Customer *customer) {
    customersList.push_back(customer->myType());
    capacity--;
}

void Table::removeCustomer(int id) {
    int i=0;
    while ((size_t )i<=customersList.size()){
        if (customersList[i]->getId()==id){
            bill = bill - customersList[i]->getBill();
            delete customersList[i];
            customersList[i]=nullptr;
            customersList.erase(customersList.begin()+i);
            i=customersList.size();
        }
        i++;
    }
    capacity++;
    // if he was the last customer, automaticlly close this table.
    if (customersList.size()==0){
        closeTable();
    }

}

Customer *Table::getCustomer(int id) {
    for (size_t i=0; i<customersList.size();i++){
        if (id==customersList[i]->getId()){
            return customersList[i];
        }
    }
    return nullptr;
}

std::vector<Customer *> &Table::getCustomers() {
    return customersList;
}

std::vector<OrderPair> &Table::getOrders() {
    return orderList;
}

void Table::order(const std::vector<Dish> &menu) {

    for (size_t i=0; i<customersList.size();i++){
        std::vector<int> ordered_dish_id_list =customersList[i]->order(menu);
        if (ordered_dish_id_list.size()!=0) {
            for (size_t j = 0; j <ordered_dish_id_list.size();j++){
                orderList.push_back(OrderPair(customersList[i]->getId(),menu[ordered_dish_id_list[j]]));
                customersList[i]->addToBill(menu[ordered_dish_id_list[j]].getPrice());
                bill=bill+menu[ordered_dish_id_list[j]].getPrice();
                std::cout<<customersList[i]->getName()<<" ordered "<<menu[ordered_dish_id_list[j]].getName()<<std::endl;
            }
        }
    }
}

void Table::openTable() {
    open=true;
}

void Table::closeTable() {
    open=false;
}

int Table::getBill() {
    return bill;
}

bool Table::isOpen() {
    return open;
}

void Table::printOrders() {
    if (orderList.size()!=0){
        for (size_t i=0; i<orderList.size();i++){
            std::cout<<customersList[orderList[i].first]->getName()
            <<" ordered "
            <<orderList[i].second.getName()
            <<std::endl;
        }
    }

}

void Table::pushOrder(int id_of_customer,Dish* dish_ordered) {
    orderList.push_back(OrderPair(id_of_customer,*dish_ordered));
    bill = bill+dish_ordered->getPrice();
}
void Table::clearBill() {
    bill=0;
}
void Table::printTableStatus(int id) {
    // Prints a status report of a given table.
    // The report should include the table status,
    // a list of customers that are seating in the table,
    // and a list of orders done by each customer.
    // If the table is closed, only the table status should be printed.
    // This action never results in an error.
    // Syntax:status <table_id>
    // Syntax of the output:
    // Table <table_id> status: <open/closed>
    // Customers:
    // <customer_1_id> <customer_1_name>
    // ...
    // <customer_n_id> <customer_n_name>
    // Orders:
    // <dish_name> <dish_price> <customer_id>
    // ...
    // <dish_name> <dish_price> <customer_id>
    // Current bill: <total_bill>
    if (open == false){
        std::cout<<"Table "<<id<<" status: closed\n";
    }
    else{
        std::cout<<"Table "<<id<<" status: open\n";
        std::cout<<"Customers:\n";
        for (size_t i=0; i<customersList.size();i++){         // Print customer list
            std::cout<<customersList[i]->toString()<<std::endl;
        }
        std::cout<<"Orders:\n";
        for (size_t i=0; i<orderList.size();i++){             // Print order list
            std::cout<<orderList[i].second.getName()
            <<" "<<orderList[i].second.getPrice()<<"NIS"
            <<" "<<orderList[i].first<<std::endl;
        }
        std::cout<<"Current Bill: "<<bill<<"NIS"<<std::endl;       // Print bill

    }

}


