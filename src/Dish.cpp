//
// Created by ariel on 11/9/18.
//

#include "Dish.h"
#include <iostream>

Dish::Dish(int d_id, std::string d_name, int d_price, DishType d_type): id(d_id),name(d_name),price(d_price),type(d_type){
}



int Dish::getId() const {
    return id;
}

std::string Dish::getName() const {
    return name;
}

int Dish::getPrice() const {
    return price;
}

DishType Dish::getType() const {
    return type;
}

void Dish::toString() const {
    // this method used for action: printMnenu.

    std::string string_representstion_of_type;
    if(type==VEG){string_representstion_of_type="VEG";}
    if(type==SPC){string_representstion_of_type="SPC";}
    if(type==ALC){string_representstion_of_type="ALC";}
    if(type==BVG){string_representstion_of_type="BVG";}
    std::cout <<name<<" "<<string_representstion_of_type<<" "<<price<<"NIS"<<std::endl;
}
