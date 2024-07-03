#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include<algorithm>
#include "bill.h"
#include "operator.h"
#include "vox.h"
#include "internet.h"
#include "customer.h"

using namespace std;

int main(int argc, char* argv[]) {
    ifstream inputFile;
    ofstream outputFile;

    if (argc != 3) {
        cout << "usage: " << argv[0] << " input_file output_file\n";
        return -1;
    }

    inputFile.open(argv[1]);
    if (!inputFile.is_open()) {
        cout << argv[0] << ": File \"" << argv[1] << "\" not found\n";
        return -1;
    }

    outputFile.open(argv[2]);
    if (!outputFile.is_open()) {
        cout << argv[0] << ": Unable to create output file \"" << argv[2] << "\"\n";
        return -1;
    }

    vector<Customer*> customers;
    vector<Operator*> operators;

    int C, O, N;
    inputFile >> C >> O >> N;

    customers.resize(C);
    operators.resize(O);

    int idClient = 0;
    int idOperator = 0;
    int age, operatorId;
    double limit;
    string name;
    int opType;
    double talkingCharge, messageCost, networkCharge, discountRate;
    Operator* op;
    Customer* customer;
    int id1, id2;
    double quantity, time, amount;
    int id;

    for (int i = 0; i < N; ++i) {
        int operationType;
        inputFile >> operationType;
        switch (operationType) {
            case 1:
                inputFile >> name >> age >> operatorId >> limit;
                customer = new Customer(idClient, name, age, operators[operatorId], limit);
                customers[idClient] = customer;
                idClient++;
                break;
            case 2:
                inputFile >> opType >> talkingCharge >> messageCost >> networkCharge >> discountRate;
                if (opType == 1) {
                    op = new VoxOperator(idOperator, talkingCharge, messageCost, networkCharge, discountRate, VOX);
                } else if (opType == 2) {
                    op = new InternetOperator(idOperator, talkingCharge, messageCost, networkCharge, discountRate, INTERNET);
                }
                operators[idOperator] = op;
                idOperator++;
                break;
            case 3:
                inputFile >> id1 >> id2 >> time;
                customers[id1]->talk(time, *customers[id2]);
                break;
            case 4:
                inputFile >> id1 >> id2 >> quantity;
                customers[id1]->message(quantity, *customers[id2]);
                break;
            case 5:
                inputFile >> id >> amount;
                customers[id]->connection(amount);
                break;
            case 6:
                inputFile >> id >> amount;
                customers[id]->pay(amount);
                break;
            case 7:
                inputFile >> id1 >> id2;
                customers[id1]->setOperator(operators[id2]);
                break;
            case 8:
                inputFile >> id >> amount;
                customers[id]->getBill()->changeTheLimit(amount);
                break;
            default:
                break;
        }
    }

outputFile << fixed << setprecision(2);

    for (auto& op : operators) {
        outputFile << "Operator " << op->getId() << ": " << op->getTotalSpentTalkingTime() << " "
                   << op->getTotalMessageSent() << " " << op->getTotalInternetUsage() << "\n";
    }

    for (auto& customer : customers) {
        outputFile << "Customer " << customer->getId() << ": " << customer->getBill()->getTotalMoneySpent() << " "
                   << customer->getBill()->getCurrentDebt() << "\n";
    }

    Customer* topTalker = nullptr;
    for (auto& customer : customers) {
        if (!topTalker || customer->getTotalSpentTalkingTime() > topTalker->getTotalSpentTalkingTime() ||
            (customer->getTotalSpentTalkingTime() == topTalker->getTotalSpentTalkingTime() && customer->getId() < topTalker->getId())) {
            topTalker = customer;
        }
    }
    if (topTalker) {
        outputFile << topTalker->getName() << ": " << topTalker->getTotalSpentTalkingTime() << "\n";
    }

    Customer* topMessenger = nullptr;
    for (auto& customer : customers) {
        if (!topMessenger || customer->getTotalMessageSent() > topMessenger->getTotalMessageSent() ||
            (customer->getTotalMessageSent() == topMessenger->getTotalMessageSent() && customer->getId() < topMessenger->getId())) {
            topMessenger = customer;
        }
    }
    if (topMessenger) {
        outputFile << topMessenger->getName() << ": " << topMessenger->getTotalMessageSent() << "\n";
    }

    Customer* topInternetUser = nullptr;
    for (auto& customer : customers) {
        if (!topInternetUser || customer->getTotalInternetUsage() > topInternetUser->getTotalInternetUsage() ||
            (customer->getTotalInternetUsage() == topInternetUser->getTotalInternetUsage() && customer->getId() < topInternetUser->getId())) {
            topInternetUser = customer;
        }
    }
    if (topInternetUser) {
        outputFile << fixed << setprecision(2) << topInternetUser->getName() << ": " << (double)((int)(topInternetUser->getTotalInternetUsage())) << "\n";
    }
    
    for (auto& customer : customers) {
        delete customer;
    }
    for (auto& op : operators) {
        delete op;
    }

    inputFile.close();
    outputFile.close();
    return 0;
}