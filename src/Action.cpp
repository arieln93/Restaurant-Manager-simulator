//
// Created by ariel on 11/9/18.
//
#include "Action.h"
#include "Restaurant.h"
extern Restaurant* backup;
//------------------------------------CLASS: BASE ACTION------------------------------

BaseAction::BaseAction(): action_contant(), errorMsg(), status(PENDING){}

ActionStatus BaseAction::getStatus() const {
    return status;
}

void BaseAction::complete() {
    status = COMPLETED;
}

void BaseAction::error(std::string errorMsg) {
    status = ERROR;
    std::cout<<errorMsg<<std::endl;
}

std::string BaseAction::getErrorMsg() const {
    return errorMsg;
}

void BaseAction::set_errorMsg(std::string msg) {
    errorMsg = msg;
}

void BaseAction::set_contant(std::string contant) {
    action_contant=contant;
}
std::string BaseAction::get_contant() const {
    return action_contant;
}

BaseAction::~BaseAction() {

}


//------------------------------------CLASS: OPEN TABLE--------------------------------
// Opens a given table and assigns a list of customers to it.
// If the table doesn't exist or is already open, this action should result in an error:
// “Table does not exist or is already open”.
// Syntax: open <table_id> <customer_1>,<customer_1_strategy> <customer_2>,<customer_2_strategy>....
OpenTable::OpenTable(int id, std::vector<Customer *> &customersList) : tableId(id), customers(){
    for (size_t i = 0; i <customersList.size() ; ++i) {
        customers.push_back(customersList[i]);
        }
    }


void OpenTable::act(Restaurant &restaurant) {
    Table *t=restaurant.getTable(tableId);

    if (t!= nullptr){
        if (t->isOpen()==false){
            if (!((int)customers.size() > t->getCapacity())){
                t->openTable();
                for (size_t i=0; i<customers.size();i++){
                    t->addCustomer(customers[i]);}
                complete();
            }
            else {error(getErrorMsg());}
        }
        else{error(getErrorMsg());}
    }
}

OpenTable::~OpenTable() {
    for (size_t i = 0; i <customers.size() ; ++i) {
        customers[i] = nullptr;
    }
    customers.clear();
}

BaseAction *OpenTable::myType() {
    return new OpenTable(*this);
}
std::string OpenTable::toString() const {
    if (getStatus()==COMPLETED){
        return get_contant()+" Completed";
    }
    return get_contant()+" "+getErrorMsg();
}




//------------------------------------CLASS: Order------------------------------------
// Takes an order from a given table:
// This function will perform an order from each customer in the table,
// and each customer will order according to his strategy.
// After finishing with the orders, a list of all orders should be printed.
// If the table doesn't exist,or isn't open, this action should result in an error:
// “Table does not exist or is not open”.
// Syntax: order <table_id>
Order::Order(int id) : tableId(id) {}

void Order::act(Restaurant &restaurant) {
    Table *table=restaurant.getTable(tableId);
    if (table!= nullptr){
        if (table->isOpen()){
            table->order(restaurant.getMenu());
            complete();
        }
        else {error(getErrorMsg());}
    }
    else {error(getErrorMsg());}
}
BaseAction *Order::myType() {
    return new Order(*this);
}
std::string Order::toString() const {
    if (getStatus()==COMPLETED){
        return get_contant()+" Completed";
    }
    return get_contant()+" "+getErrorMsg();
}




//------------------------------------CLASS: MoveCustomer----------------------------
// Moves a customer from one table to another.
// Also moves all orders made by this customer from the bill of the origin table,
// to the bill of the destination table.
// If the origin table has no customers left after this move, the program will close the origin table.
// If either the origin or destination table are closed or doesn't exist,
// or if no customer with the received id is in the origin table,
// or if the destination table has no available seats for additional customers,
// this action should result in an error: “Cannot move customer”.
// Syntax: move <origin_table_id> <dest_table_id> <customer_id>
// Example:"move 2 3 5" will move customer 5 from table 2 to table 3.
MoveCustomer::MoveCustomer(int src, int dst, int customerId) : srcTable(src), dstTable(dst), id(customerId){}

void MoveCustomer::act(Restaurant &restaurant) {

    if ((!(srcTable > restaurant.getNumOfTables()) | (dstTable > restaurant.getNumOfTables()))) {

        Table *sourceTable = restaurant.getTable(srcTable);
        Table *destinationTable= restaurant.getTable(dstTable);
            if (((sourceTable != nullptr) & (destinationTable != nullptr))){
                if ((sourceTable->isOpen() & destinationTable->isOpen())) {
                    Customer* customer_to_move=sourceTable->getCustomer(id);
                    if ((customer_to_move) != (nullptr)) {
                        if (destinationTable->getCapacity() > 0) {
                            //move the customer.
                            destinationTable->addCustomer(customer_to_move);             //moving the customer
                            sourceTable->removeCustomer(id);                        //removing him from source table
                            std::vector<OrderPair> tmpOrders=(sourceTable->getOrders());
                            sourceTable->getOrders().clear();
                            sourceTable->clearBill();
                            for(size_t i=0; i<tmpOrders.size();i++){                           //moving all of his orders
                                if (tmpOrders[i].first==id){
                                    destinationTable->pushOrder(id,&tmpOrders[i].second);
                                }
                                else{
                                    sourceTable->pushOrder(tmpOrders[i].first,&tmpOrders[i].second);
                                    }
                            }
                            complete();
                        } else { error(getErrorMsg()); }
                    } else { error(getErrorMsg()); }
                } else { error(getErrorMsg()); }
        }else { error(getErrorMsg()); }
    }else { error(getErrorMsg()); }

}
BaseAction *MoveCustomer::myType() {
    return new MoveCustomer(*this);
}
std::string MoveCustomer::toString() const {
    if (getStatus()==COMPLETED){
        return get_contant()+" Completed";
    }
    return get_contant()+" "+getErrorMsg();}




//------------------------------------CLASS: Close-------------------------------------
// Closes a given table. Should print the bill of the table to the screen.
// After this action the table should be open for new customers.
// If the table doesn't exist, or isn't open, this action should result in an error:
// “Table does not exist or is not open”.
// Syntax: close <table_id>
// Example:"close 2" closes table 2,and then prints: "Table 2 was closed. Bill 500NIS"
Close::Close(int id) : tableId(id){}

void Close::act(Restaurant &restaurant) {
    int bill = restaurant.cleanTable(tableId);
    if (bill!=(-1)){
        std::cout<<"Table "<<tableId<<" was closed. Bill "<<bill<<"NIS\n";
        complete();
    }
    else { error(getErrorMsg());}
}
BaseAction *Close::myType() {
    return new Close(*this);
}
std::string Close::toString() const {
    if (getStatus()==COMPLETED){
        return get_contant()+" Completed";
    }
    return get_contant()+" "+getErrorMsg();}




//------------------------------------CLASS: CloseAll-------------------------------------
// Closes all tables in the restaurant, and then closes the restaurant and exits.
// The bills of all the tables that were closed by that action should be printed,
// sorted by the table id in an increasing order.
// Note that if all tables are closed in the restaurant,the action will just close the restaurant and exit.
// This action never results in an error.
// Syntax: closeall
// Example: "closeall" will print:
// Table 2 was closed. Bill 500NIS
// Table 4 was closed. Bill 200NIS
// Table 5 was closed. Bill 600NIS
CloseAll::CloseAll() {
}

void CloseAll::act(Restaurant &restaurant) {
    for (int i=0; i<restaurant.getNumOfTables(); i++){          //clean all tables
        int bill = restaurant.cleanTable(i);
        if (bill!=(-1)){
            std::cout<<"Table "<<i<<" was closed. Bill "<<bill<<"NIS\n";
        }

    }
    complete();
}
BaseAction *CloseAll::myType() {
    return new CloseAll(*this);
}
std::string CloseAll::toString() const {
    if (getStatus()==COMPLETED){
        return get_contant()+" Completed";
    }
    return get_contant()+" "+getErrorMsg();
}




//------------------------------------CLASS: PrintMenu-------------------------------------
// Prints the menu of the restaurant.
// This action never results in an error.
// Each dish in the menu should be printed in the following manner:
// <dish_name> <dish_type> <dish_price>
// Syntax: menu
// Example:
// "menu" will print:
// Salad VEG 40NIS
// Water BVG 20NIS
// Chili con carne SPC 400NIS
PrintMenu::PrintMenu() {

}

void PrintMenu::act(Restaurant &restaurant) {
    restaurant.printMenu();
    complete();
}
BaseAction *PrintMenu::myType() {
    return new PrintMenu(*this);
}
std::string PrintMenu::toString() const {
    if (getStatus()==COMPLETED){
        return get_contant()+" Completed";
    }
    return get_contant()+" "+getErrorMsg();}




//------------------------------------CLASS: PrintTableStatus-------------------------------------
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
PrintTableStatus::PrintTableStatus(int id): tableId(id){}

void PrintTableStatus::act(Restaurant &restaurant) {
    restaurant.getTable(tableId)->printTableStatus(tableId);
    complete();
}
BaseAction *PrintTableStatus::myType() {
    return new PrintTableStatus(*this);
}
std::string PrintTableStatus::toString() const {
    if (getStatus()==COMPLETED){
        return get_contant()+" Completed";
    }
    return get_contant()+" "+getErrorMsg();
}




//------------------------------------CLASS: PrintActionsLog-------------------------------------
// Prints all the actions that were performed by the user (excluding current log action),
// from the first action to the last action.
// This action never results in an error.
// Syntax: log
// Syntax of the output:
// <action_n_name> <action_n_args> <action_n_status>
//...
// <action_1_name> <action_1_args> <action_1_status>
//
// where action's name is the action's syntax, action's args are the action's arguments.
// The status of each action should be"Completed" if the order was completed successfully,
// or "Error: <error_message>" otherwise.
// Example:
// open 2 Jon,spc Pete,veg Completed
// open 2 Roger,spc Keith,alc Error:Table does not exist or is already open

PrintActionsLog::PrintActionsLog() {

}

void PrintActionsLog::act(Restaurant &restaurant) {
    const std::vector<BaseAction*> &ref = restaurant.getActionsLog();
    for(size_t i=0; i<ref.size();i++){
        std::cout<<ref[i]->toString()<<std::endl;
    }
complete();

}
BaseAction *PrintActionsLog::myType() {
    return new PrintActionsLog(*this);
}
std::string PrintActionsLog::toString() const {
    if (getStatus()==COMPLETED){
        return get_contant()+" Completed";
    }
    return get_contant()+" "+getErrorMsg();
}




//------------------------------------CLASS: BackupRestaurant-------------------------------------

BackupRestaurant::BackupRestaurant() {

}

void BackupRestaurant::act(Restaurant &restaurant) {
    //Restaurant temp = restaurant;
    if (backup == nullptr) {
        backup = new Restaurant(restaurant);
    }
    else {
        *backup = restaurant;
    }
    complete();
}
BaseAction *BackupRestaurant::myType() {
    return new BackupRestaurant(*this);
}
std::string BackupRestaurant::toString() const {
    if (getStatus()==COMPLETED){
        return get_contant()+" Completed";
    }
    return get_contant()+" "+getErrorMsg();
}




//------------------------------------CLASS: RestoreResturant-------------------------------------

RestoreResturant::RestoreResturant() {

}

void RestoreResturant::act(Restaurant &restaurant) {
    if (backup!= nullptr){
        restaurant=*backup;
        complete();
    }
    else{
        error(getErrorMsg());
    }
}
BaseAction *RestoreResturant::myType() {
    return new RestoreResturant(*this);
}
std::string RestoreResturant::toString() const {
    if (getStatus()==COMPLETED){
        return get_contant()+" Completed";
    }
    return get_contant()+" "+getErrorMsg();}
