//
// Created by ariel on 11/9/18.
//

#include "Customer.h"
#include <vector>
#include <iostream>
using namespace std;



Customer::Customer(std::string c_name, int c_id):name(c_name),id(c_id) ,bill(0) {
}

Customer::~Customer() {
}

std::string Customer::getName() const {
    return name;
}
int Customer::getId() const {
    return id;
}

int Customer::getBill() {
    return bill;
}

void Customer::addToBill(int price) {
    bill = bill + price;
}

//----------------------------VegetarianCustomer---------------------------
VegetarianCustomer::VegetarianCustomer(std::string name, int id):Customer(name,id) {
}

Customer *VegetarianCustomer::myType() {
    return new VegetarianCustomer(*this);
}

VegetarianCustomer::~VegetarianCustomer() {
}


std::vector<int> VegetarianCustomer::order(const std::vector<Dish> &menu) {
    vector<int> VegOrder;
    int mostExpensiveBeverage=0,menuIndexVEG=INT8_MAX,menuIndexBVG=INT8_MAX;
    for (size_t i = 0; i < menu.size(); ++i) {
        // This is a customer that always orders the vegetarian dish with the smallest id in the menu.
        if (menu[i].getType()==VEG){
            if (menu[i].getId()<menuIndexVEG){
                menuIndexVEG=menu[i].getId();
            }
        }
        // and the most expensive beverage(Non-alcoholic),
        // if there are more then one, then take the one with the smallest id.
            if (menu[i].getType()==BVG){
                if(menu[i].getPrice()==mostExpensiveBeverage){
                    if (menu[i].getId()<menuIndexBVG){
                        menuIndexBVG=menu[i].getId();
                    }
                }
                if (menu[i].getPrice()>mostExpensiveBeverage){
                    mostExpensiveBeverage=menu[i].getPrice();
                    menuIndexBVG=menu[i].getId();
                }
            }
    }
    if (menuIndexVEG!=INT8_MAX) {
        if (menuIndexBVG!=INT8_MAX) {
            VegOrder.push_back(menuIndexVEG);
            VegOrder.push_back(menuIndexBVG);
        }
    }
    return VegOrder;
}

std::string VegetarianCustomer::toString() const {
    return std::to_string(getId())+" "+getName();
}


//----------------------------------CheapCustomer----------------------------
CheapCustomer::CheapCustomer(std::string name, int id):Customer(name,id), ordered(false) {
}
Customer *CheapCustomer::myType() {
    return new CheapCustomer(*this);
}
std::vector<int> CheapCustomer::order(const std::vector<Dish> &menu) {
    vector<int> cheapOrder;
    int lowestPrice = INT8_MAX;
    int menuIndexCHP=INT8_MAX;
    if (!ordered) {
        for (size_t i = 0; i < menu.size(); ++i) {
            // if there is more then one cheapest meal, take the one with the smallest id:
            if (menu[i].getPrice() == lowestPrice) {
                if (menu[i].getId() < menuIndexCHP) {
                    menuIndexCHP = menu[i].getId();
                }
            }
            if (menu[i].getPrice() < lowestPrice) {
                lowestPrice = menu[i].getPrice();
                menuIndexCHP = menu[i].getId();
            }
        }
        if(menuIndexCHP!=INT8_MAX) {
            cheapOrder.push_back(menuIndexCHP);
            ordered= true;
        }
    }
    return cheapOrder;
}
std::string CheapCustomer::toString() const {
    return std::to_string(getId())+" "+getName();
}


//----------------------------------SpicyCustomer----------------------------
SpicyCustomer::SpicyCustomer(std::string name, int id):Customer(name,id), ordered(false){
}
Customer *SpicyCustomer::myType() {
    return new SpicyCustomer(*this);
}
std::vector<int> SpicyCustomer::order(const std::vector<Dish> &menu) {
    vector<int> spicyOrder;
    int expensiveSPC=0,cheapBVG=INT8_MAX,menuIndexSPC=INT8_MAX,menuIndexBVG=INT8_MAX;
    // First, he order the most expensive SPC dish in the menu.
    // If there is no SPC dish the customer won't order at all.
    if (!ordered) {
        for (size_t i = 0; i < menu.size(); ++i) {
            if (menu[i].getType() == SPC) {
                // if there is more then one expensive SPC meal, take the one with the smallest id:
                if (menu[i].getPrice() == expensiveSPC) {
                    if (menu[i].getId() < menuIndexSPC) {
                        menuIndexSPC = menu[i].getId();
                    }
                }
                if (menu[i].getPrice() > expensiveSPC) {
                    expensiveSPC = menu[i].getPrice();
                    menuIndexSPC = menu[i].getId();
                }
            }
        }
        if (menuIndexSPC != INT8_MAX) {
        spicyOrder.push_back(menuIndexSPC);
        ordered= true;
        }
        return spicyOrder;
    }
    // For further orders, he picks the cheapest non - alcoholic beverage in the menu.
    // The order might be equal to previous orders.
    else{
        for (size_t i = 0; i < menu.size() ; ++i) {
            if (menu[i].getType()==BVG){
                // if there is more then one cheapest BVG , take the one with the smallest id:
                if (menu[i].getPrice() == cheapBVG) {
                    if (menu[i].getId() < menuIndexBVG) {
                        menuIndexBVG = menu[i].getId();
                    }
                }
                if (menu[i].getPrice()<cheapBVG){
                    cheapBVG=menu[i].getPrice();
                    menuIndexBVG=menu[i].getId();
                }
            }
        }
        if (menuIndexBVG!=INT8_MAX) {
            spicyOrder.push_back(menuIndexBVG);
        }
        return spicyOrder;
    }
}
std::string SpicyCustomer::toString() const {
    return std::to_string(getId())+" "+getName();
}


//----------------------------------AlchoholicCustomer----------------------------
AlchoholicCustomer::AlchoholicCustomer(std::string name, int id) :Customer(name,id), lowerBound(-1), lastID(-1){
}
Customer *AlchoholicCustomer::myType() {
    return new AlchoholicCustomer(*this);
}
std::vector<int> AlchoholicCustomer::order(const std::vector<Dish> &menu) {
    vector<int> alcoholicOrder;
    // This is a customer who only orders alcoholic beverages.
    // He starts with ordering the cheapest one, and in each further order,
    // he picks the next expensive alcoholic beverage after lowerBound.
    // After reaching the most expensive alcoholic beverage, he won't order again.
    int upperBound=INT8_MAX,menuIndexALC=INT8_MAX;
    bool change = true;
    if (lowerBound!=INT8_MAX) {
        for (size_t i = 0; i < menu.size(); ++i) {
            if (menu[i].getType() == ALC){
                // if there is more then one cheapest ALC , take the one with the smallest id:
                if (menu[i].getPrice() == lowerBound) {
                    if (menu[i].getId() > lastID) {
                        menuIndexALC = menu[i].getId();
                        change = false;
                        break;
                    }
                }
                if (menu[i].getPrice() == upperBound) {
                    if (menu[i].getId() < menuIndexALC){
                        menuIndexALC = menu[i].getId();
                    }
                }
                if ((menu[i].getPrice() > lowerBound) & (menu[i].getPrice() < upperBound)) {
                    upperBound = menu[i].getPrice();
                    menuIndexALC = menu[i].getId();
                }
            }
        }
        // if there is no more expensive alcoholic BVG lest, then don't order.
        if(menuIndexALC!=INT8_MAX) {
            alcoholicOrder.push_back(menuIndexALC);
            lastID= menuIndexALC;
        }
        if (change == true){

            lowerBound=upperBound;
        }
    }
    return alcoholicOrder;

}
std::string AlchoholicCustomer::toString() const {
    return std::to_string(getId())+" "+getName();
}
