#ifndef ORDER_H
#define ORDER_H

#include <string>

// Estrutura simples de ordem (paper mode)
struct Order {
    std::string id;
    std::string side; // "BUY" ou "SELL"
    double qty;
    double price;
    std::string status; // "FILLED", "REJECTED", ...
};

#endif // ORDER_H
