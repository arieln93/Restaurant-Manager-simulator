#ifndef CUSTOMER_H_
#define CUSTOMER_H_

#include <vector>
#include <string>
#include "Dish.h"

class Customer{
public:
    // constructor:
    Customer(std::string c_name, int c_id);
    virtual Customer* myType()=0;
    virtual ~Customer();
    // This functions recieves a vector of dishes, and returns a vector of
    // dishes that the customer ordered:
    virtual std::vector<int> order(const std::vector<Dish> &menu)=0;
    virtual std::string toString() const = 0;
    std::string getName() const;
    int getId() const;
    int getBill();
    void addToBill(int price);
private:
    const std::string name;
    const int id;
    int bill;
};


class VegetarianCustomer : public Customer {
public:
    VegetarianCustomer(std::string name, int id);
    virtual ~VegetarianCustomer();
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    Customer* myType();
private:
};


class CheapCustomer : public Customer {
public:
    CheapCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    Customer* myType();
private:
    bool ordered;
};


class SpicyCustomer : public Customer {
public:
    SpicyCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    Customer* myType();
private:
    bool ordered;
};


class AlchoholicCustomer : public Customer {
public:
    AlchoholicCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    Customer* myType();
private:
    int lowerBound;
    int lastID;
};



#endif
