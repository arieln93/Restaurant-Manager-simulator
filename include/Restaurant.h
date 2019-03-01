//
// Created by ariel on 11/9/18.
//

#ifndef AS1_RESTAURANT_H
#define AS1_RESTAURANT_H


#include <vector>
#include <string>
#include "Dish.h"
#include "Table.h"
#include "Action.h"


class Restaurant{
public:
    Restaurant();
    Restaurant(const std::string &configFilePath);
    void clean();
    void copy(const Restaurant& other);
    Restaurant(const Restaurant& other);
    Restaurant& operator=(const Restaurant& other);
    void steal (Restaurant& other);
    Restaurant(Restaurant&& other);
    Restaurant& operator=(Restaurant&& other);
    virtual ~Restaurant();
    //------------------------
    void start();
    int getNumOfTables() const;
    Table* getTable(int ind);
    const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    std::vector<Dish>& getMenu();
    void printMenu();
    int cleanTable (int id);
    void CommandsReader();
    int get_id_for_new_customer();
private:
    bool open;
    int numOfTables;
    int id_for_new_customer;
    std::string myConfigFilePath;
    std::vector<Table*> tables;
    std::vector<Dish> menu;
    std::vector<BaseAction*> actionsLog;
};


#endif //AS1_RESTAURANT_H
