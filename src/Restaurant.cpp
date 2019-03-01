//
// Created by ariel on 11/9/18.
//

#include "Restaurant.h"
#include <iostream>
#include <fstream>
#include <string>
Restaurant::Restaurant() : open(false), numOfTables(0), id_for_new_customer(0), myConfigFilePath(""), tables(), menu(), actionsLog(){}

Restaurant::Restaurant(const std::string &configFilePath):open(false), numOfTables(0), id_for_new_customer(0), myConfigFilePath(configFilePath), tables(), menu(), actionsLog() {
    std:: string line;
    std:: ifstream myfile;
    myfile.open(myConfigFilePath);
    while (getline(myfile,line))
    {
        if (line.size()!=0) {
            if (line.at(0) == '#') {
                //--- Adding Tables number:
                if (line.substr(1)=="number of tables"){
                    getline(myfile,line);
                    while (line.size()==0){
                        getline(myfile,line);
                    }
                    numOfTables=std::stoi(line);
                }
                //--- Adding Tables description:
                if (line.substr(1)=="tables description") {
                    getline(myfile, line);
                    while (line.size()==0){
                        getline(myfile,line);
                    }
                    while (line.size()!=0) {
                        if (line.size() != 0) {
                            tables.push_back(new Table(std::stoi(line.substr(0, line.find(",")))));
                            if (line.find(",") != std::string::npos) {
                                line = line.substr(line.find(",") + 1);

                            } else {
                                line = "";
                            }
                        }
                    }
                }
                //--- Adding Dishes to the Menu:
                if (line.size()>1 && line.substr(1)=="Menu") {
                    int id(0);
                    while (getline(myfile,line)) {
                        if (line.size() != 0) {
                            // generate name:
                            std::string name = line.substr(0, line.find(","));
                            // generate type:
                            line = line.substr(line.find(",") + 1);
                            std::string type = line.substr(0, line.find(","));
                            DishType dt;
                            if (type == "VEG") { dt = VEG; }
                            if (type == "SPC") { dt = SPC; }
                            if (type == "BVG") { dt = BVG; }
                            if (type == "ALC") { dt = ALC; }
                            // generate price:
                            line = line.substr(line.find(",") + 1);
                            int price = std::stoi(line.substr(0, line.find(",")));
                            menu.push_back(std::move(Dish(id, name, price, dt)));
                            id++;
                        }
                    }
                }
            }
        }
    }

    myfile.close();
}

void Restaurant::copy(const Restaurant &other) {
    open=other.open;
    numOfTables=other.numOfTables;
    id_for_new_customer=other.id_for_new_customer;
    myConfigFilePath=other.myConfigFilePath;
    for (size_t i = 0; i < other.tables.size(); ++i) {
        tables.push_back(new Table(*other.tables[i]));
    }
    for (size_t j = 0; j < other.menu.size(); ++j) {
        menu.push_back(Dish(other.menu[j]));
    }
    for (size_t k = 0; k < other.actionsLog.size(); ++k) {
        actionsLog.push_back(other.actionsLog[k]->myType());
    }
}

Restaurant::Restaurant(const Restaurant &other) :open(false), numOfTables(0), id_for_new_customer(0), myConfigFilePath(""), tables(), menu(), actionsLog() {
    copy(other);
}

Restaurant &Restaurant::operator=(const Restaurant &other) {
    if (&other !=this){
        clean();
        copy(other);
    }
    return *this;
}

void Restaurant::steal(Restaurant& other) {
    open=other.open;
    numOfTables=other.getNumOfTables();
    id_for_new_customer=other.id_for_new_customer;
    myConfigFilePath = other.myConfigFilePath;
    tables = other.tables;
    for (size_t j = 0; j < other.menu.size(); ++j) {
        menu.push_back(Dish(other.menu[j]));
    }
    actionsLog=other.actionsLog;
    for (size_t i = 0; i < other.tables.size(); ++i) {
        other.tables[i]= nullptr;
    }
    for (size_t j = 0; j < other.actionsLog.size(); ++j) {
        other.actionsLog[j]= nullptr;
    }

}
Restaurant::Restaurant(Restaurant&& other): open(false), numOfTables(0), id_for_new_customer(0), myConfigFilePath(""), tables(), menu(), actionsLog() {
    steal(other);
}

Restaurant &Restaurant::operator=(Restaurant&& other) {
    clean();
    steal(other);
    return *this;
}

void Restaurant::clean() { // this functions returns (-1) if clean wasn't successful, or the bill.
    for (size_t i=0; i<tables.size();i++){
        delete tables[i];
    }
    tables.clear();
    menu.clear();
    for (size_t i=0; i<actionsLog.size();i++){
        delete actionsLog[i];
    }
    actionsLog.clear();
}

void Restaurant::start() {
    open=true;
    std::cout<<"Restaurant is now open!\n";
    CommandsReader();
}

int Restaurant::getNumOfTables() const {
    return numOfTables;
}

Table *Restaurant::getTable(int ind) {

    if ((ind>=0) & (ind<=(int)tables.size())) {
        return tables[ind];
    }
    else{
        return nullptr;
    }
}

const std::vector<BaseAction *> &Restaurant::getActionsLog() const {
    return actionsLog;
}

std::vector<Dish> &Restaurant::getMenu() {
    return menu;
}

void Restaurant::printMenu() {
    if (menu.size()==0){
    }
    else {
        for (size_t i=0;i<menu.size();i++){
            menu[i].toString();
        }
    }
}

int Restaurant::cleanTable(int id) {
    if ((id>=0) & (id<=numOfTables)){
        return tables[id]->clean();
    }
    return (-1);
}

int Restaurant::get_id_for_new_customer() {
    id_for_new_customer++;
    return (id_for_new_customer-1);
}

Restaurant::~Restaurant(){
    for (size_t i=0; i<tables.size();i++){
        delete tables[i];
    }
    tables.clear();
    menu.clear();
    for (size_t i=0; i<actionsLog.size();i++){
        delete actionsLog[i];
    }
    actionsLog.clear();
}

void Restaurant::CommandsReader() {
    std::string command;
    getline(std::cin, command);

        while (command != "closeall") {
            std::string command_description = command;
            // check what is the commend
            if (command.size() != 0) {
                if (command.find(' ') != std::string::npos) {
                    std::string first_word = command.substr(0, command.find(' '));
                    command = command.substr(command.find(' ') + 1);
                    if (first_word == "open") {
                        // generate table id:
                        int table_id = std::stoi(command.substr(0, command.find(' ')));
                        command = command.substr(command.find(' ') + 1);
                        // generate customers list on the heap:
                        std::vector<Customer *> *ref = new std::vector<Customer *>();
                        while (command.size() != 0) {
                            std::string customer_name, customer_type;
                            customer_name = command.substr(0, command.find(','));
                            customer_type = command.substr(command.find(',') + 1,
                                                           command.find(' ') - (command.find(',') + 1));
                            if (customer_type == "veg") {
                                ref->push_back(new VegetarianCustomer(customer_name, get_id_for_new_customer()));
                            }
                            if (customer_type == "spc") {
                                ref->push_back(new SpicyCustomer(customer_name, get_id_for_new_customer()));
                            }
                            if (customer_type == "alc") {
                                ref->push_back(new AlchoholicCustomer(customer_name, get_id_for_new_customer()));
                            }
                            if (customer_type == "chp") {
                                ref->push_back(new CheapCustomer(customer_name, get_id_for_new_customer()));
                            }
                            if (command.find(' ') != std::string::npos) {
                                command = command.substr(command.find(' ') + 1);
                            } else {
                                command = "";
                            }
                        }
                        BaseAction *open_action = new OpenTable(table_id, (*ref));
                        open_action->set_contant(command_description);
                        open_action->set_errorMsg("Error: Table does not exist or is already open");
                        open_action->act(*this);
                        actionsLog.push_back(open_action);
                        // check memory leaks here
                        for (size_t i = 0; i < ref->size(); ++i) {
                            delete ((*ref)[i]);
                            (*ref)[i] = nullptr;
                        }
                        ref->clear();
                        delete ref;

                    }
                    if (first_word == "order") {
                        int table_id = std::stoi(command);
                        BaseAction *order_action = new Order(table_id);
                        order_action->set_contant(command_description);
                        order_action->set_errorMsg("Error: Table does not exist or is not open");
                        order_action->act(*this);
                        actionsLog.push_back(order_action);
                    }
                    if (first_word == "move") {
                        int source_table = stoi(command.substr(0, 1));
                        int destination_table = stoi(command.substr(2, 1));
                        int customer_id = stoi(command.substr(4, 1));
                        BaseAction *move_action = new MoveCustomer(source_table, destination_table, customer_id);
                        move_action->set_contant(command_description);
                        move_action->set_errorMsg("Error: Cannot move customer");
                        move_action->act(*this);
                        actionsLog.push_back(move_action);
                    }
                    if (first_word == "close") {
                        int table_id = stoi(command.substr(0, 1));
                        BaseAction *close_action = new Close(table_id);
                        close_action->set_contant(command_description);
                        close_action->set_errorMsg("Error: Table does not exist or is not open");
                        close_action->act(*this);
                        actionsLog.push_back(close_action);
                    }
                    if (first_word == "status") {
                        int table_id = stoi(command.substr(0, 1));
                        BaseAction *status_action = new PrintTableStatus(table_id);
                        status_action->set_contant(command_description);
                        status_action->act(*this);
                        actionsLog.push_back(status_action);
                    }
                }
                if (command == "menu") {
                    BaseAction *menu_action = new PrintMenu;
                    menu_action->set_contant(command_description);
                    menu_action->act(*this);
                    actionsLog.push_back(menu_action);
                }
                if (command == "log") {
                    BaseAction *log_action = new PrintActionsLog;
                    log_action->set_contant(command_description);
                    log_action->act(*this);
                    actionsLog.push_back(log_action);
                }
                if (command == "backup") {
                    BaseAction *backup_action = new BackupRestaurant;
                    backup_action->set_contant(command_description);
                    backup_action->act(*this);
                    actionsLog.push_back(backup_action);
                }
                if (command == "restore") {
                    BaseAction *restore_action = new RestoreResturant;
                    restore_action->set_contant(command_description);
                    restore_action->set_errorMsg("Error: No backup available");
                    restore_action->act(*this);
                    actionsLog.push_back(restore_action);
                }


            }
            getline(std::cin, command);
        }
        if (command == "closeall") {
            std::string command_description = command;
            BaseAction *closeall_action = new CloseAll;
            closeall_action->set_contant(command_description);
            closeall_action->act(*this);
            actionsLog.push_back(closeall_action);
        }
    
}
