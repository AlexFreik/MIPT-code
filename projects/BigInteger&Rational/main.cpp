//
//  BI.cpp
//  Speedwaagonnnnnnnm
//
//  Created by Александр Фреик on 18/11/2019.
//  Copyright © 2019 Freik. All rights reserved.
//
#include<iostream>
#include<vector>
#include<string>


#include "biginteger.h"

int main() {
    std::string s0 = "256";
    std::string s1 = "-7777777787";
    std::string s10 = "1";
    int t = 3, j;
    //std::cout << t++ << " ";
    //std::cout << t <<  std::endl;
    BigInteger num0(s0), num1(s1), num10(s10);
    Rational r0(num0), r1(num1), r10(num10);
    Rational r("111111" * num1, -num0);
    Rational rr("2569897658000000000000058", -num1 * num0);
    //std::cout << r.gcd() << std::endl;
    std::cout << r.toString() << std::endl << (+rr).toString() << std::endl;
    r += rr;
    std::cout << (r).toString() << std::endl;
    //std::cout << (num30 >= num3) << " " << (num3 >= num2) << std::endl;
    
}
