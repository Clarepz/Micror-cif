//
// Created by Clarence Provenaz on 15.03.2024.
//
#include <iostream>

#include "message.h"
#include "lifeform.h"
#include "constantes.h"

bool domainCheck(S2d center);
bool ageCheck(int age);

Alg::Alg(S2d position, int age) {
    domainCheck(position);
    position_=position;
    ageCheck(age);

    age_=age;
    std::cout<<position_.x<<position_.y<<std::endl;
}


bool domainCheck(S2d center){
    if (center.x<max-1 and center.y<max-1 and center.x>1 and center.y>1){
        return true;
    }else{
        std::cout << message::lifeform_center_outside(center.x,center.y);
        exit(EXIT_FAILURE);
    }
}

bool ageCheck(int age){
    if (age>0){
        return true;
    }else{
        std::cout << message::lifeform_age(age);
        exit(EXIT_FAILURE);
    }
}