#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
#include "bill.h"
#include "operator.h"

class Customer {
private:
    int id, age, totalSpentTalkingTime, totalMessageSent;
    double totalInternetUsage;
    std::string name;
    Operator *op;
    Bill *bill;

public:
    Customer(int id_, const std::string &name_, int age_, Operator *op_, double creditLimit);
    Customer(const Customer&);
    ~Customer();

    int getId() const;
    int getAge() const;
    int getTotalSpentTalkingTime() const;
    int getTotalMessageSent() const;
    double getTotalInternetUsage() const;
    std::string getName() const;
    Operator* getOperator() const;
    Bill* getBill() const;

    void setOperator(Operator*);

    std::string toString() const;

    void talk(int, Customer&);
    void message(int, const Customer&);
    void connection(double);
    void pay(double);
    void changeTheLimit(double amount);


};

Customer::Customer(int id_, const std::string &name_, int age_, Operator *op_, double creditLimit)
    : id(id_), name(name_), age(age_), op(op_), bill(new Bill(creditLimit)),
      totalSpentTalkingTime(0), totalMessageSent(0), totalInternetUsage(0) {}

Customer::Customer(const Customer& x)
    : id(x.id), age(x.age), totalSpentTalkingTime(x.totalSpentTalkingTime),
      totalMessageSent(x.totalMessageSent), totalInternetUsage(x.totalInternetUsage),
      name(x.name), op(x.op), bill(new Bill(*x.bill)) {}

Customer::~Customer() {
    delete bill;
    bill = nullptr;
    op = nullptr;
}

int Customer::getId() const {
    return id;
}

int Customer::getAge() const {
    return age;
}

int Customer::getTotalSpentTalkingTime() const {
    return totalSpentTalkingTime;
}

int Customer::getTotalMessageSent() const {
    return totalMessageSent;
}

double Customer::getTotalInternetUsage() const {
    return totalInternetUsage;
}

std::string Customer::getName() const {
    return name;
}

Operator* Customer::getOperator() const {
    return op;
}

Bill* Customer::getBill() const {
    return bill;
}

void Customer::setOperator(Operator* newOp) {
    op = newOp;
}

void Customer::talk(int minutes, Customer &other) {
    if (minutes > 0 && &other != this) {
        double callCost = op->calculateTalkingCost(minutes, age);
        if (bill->check(callCost)) {
            bill->add(callCost);
            totalSpentTalkingTime += minutes;
            op->addTalkingTime(minutes);
            if (op->getId() != other.getOperator()->getId())
                other.getOperator()->addTalkingTime(minutes);
        }
    }
}

void Customer::message(int quantity, const Customer &other) {
    if (quantity > 0 && &other != this) {
        double messageCost = op->calculateMessageCost(quantity, op->getId(), other.getOperator()->getId());
        if (bill->check(messageCost)) {
            bill->add(messageCost);
            totalMessageSent += quantity;
            op->addTotalMessageSent(quantity);
        }
    }
}

void Customer::connection(double amount) {
    if (amount > 0) {
        double internetCost = op->calculateNetworkCost(amount);
        if (bill->check(internetCost)) {
            bill->add(internetCost);
            totalInternetUsage += amount;
            op->addTotalInternetUsage(amount);
        }
    }
}

void Customer::pay(double amount) {
    if (amount > 0) {
        bill->pay(amount);
    }
}

std::string Customer::toString() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    oss << "Customer " << id << ": " << name << ", " << age << " years old, "
        << "talking time: " << totalSpentTalkingTime << " minutes, "
        << "messages: " << totalMessageSent << ", "
        << "internet: " << totalInternetUsage << " GB, "
        << "debt: $" << bill->getCurrentDebt();
    return oss.str();
}

void Customer::changeTheLimit(double amount) {
      bill->changeTheLimit(amount);
    }

#endif
