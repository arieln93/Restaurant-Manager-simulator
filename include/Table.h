#ifndef TABLE_H_
#define TABLE_H_

#include <vector>
#include "Customer.h"
#include "Dish.h"

typedef std::pair<int, Dish> OrderPair;

class Table{
public:
    //-----------CONSTRUCTORS---------
    Table(int t_capacity);
    int clean();
    void copy(const Table& other);
    Table(const Table& other);
    Table& operator=(const Table& other);
    void steal (Table& other);
    Table(Table&& other);
    Table& operator=(Table&& other);
    virtual ~Table();
    //----------GETTERS---------
    int getCapacity() const;
    int getBill();
    std::vector<Customer*>& getCustomers();
    std::vector<OrderPair>& getOrders();
    Customer* getCustomer(int id);
    //-----------METHODS---------
    void addCustomer(Customer* customer);
    void removeCustomer(int id);
    void order(const std::vector<Dish> &menu);
    void openTable();
    void closeTable();
    bool isOpen();
    void pushOrder(int id_of_customer,Dish* dish_ordered);
    void printOrders();
    void printTableStatus(int id);
    void clearBill();

private:
    int capacity;
    int bill;
    bool open;
    std::vector<Customer*> customersList;
    std::vector<OrderPair> orderList; //A list of pairs for each order in a table - (customer_id, Dish)
};

#endif
