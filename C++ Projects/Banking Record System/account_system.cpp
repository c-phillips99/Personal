#include <iostream>
#include <fstream>
#include <string>

class Account {
private:
    std::string accountNumber;
    std::string firstName;
    std::string lastName;
    float totalBalance;

public:
    void getData();
    void showData();
    void writeRecord();
    void readRecord();
    void searchRecord();
    void editRecord();
    void deleteRecord();
};

void Account::getData() {
    std::cout << "\nEnter Account Number: ";
    std::cin >> accountNumber;
    std::cout << "Enter First Name: ";
    std::cin >> firstName;
    std::cout << "Enter Last Name: ";
    std::cin >> lastName;
    std::cout << "Enter Balance: ";
    std::cin >> totalBalance;
    std::cout << std::endl;
}

void Account::showData() {
    std::cout << "Account Number: " << accountNumber << std::endl;
    std::cout << "First Name: " << firstName << std::endl;
    std::cout << "Last Name: " << lastName << std::endl;
    std::cout << "Current Balance: $" << totalBalance << std::endl;
    std::cout << "-------------------------------" << std::endl;
}

void Account::writeRecord() {
    std::ofstream outfile;
    outfile.open("record.bank", std::ios::binary | std::ios::app);
    getData();
    outfile.write(reinterpret_cast<char *>(this), sizeof(*this));
    outfile.close();
}

void Account::readRecord() {
    std::ifstream infile;
    infile.open("record.bank", std::ios::binary);
    if (!infile) {
        std::cout << "Error! File Not Found!" << std::endl;
        return;
    }
    std::cout << "\n**** Data From File ****" << std::endl;
    std::cout << "-------------------------------" << std::endl;
    while (infile) {
        infile.read(reinterpret_cast<char*>(this), sizeof(*this));
        if (infile) {
            showData();
        }
    }
    infile.close();
    std::cout << std::endl;
}

void Account::searchRecord() {
    int n;
    std::ifstream infile;
    infile.open("record.bank", std::ios::binary);
    if (!infile) {
        std::cout << "Error! File Not Found!" << std::endl;
        return;
    }
    infile.seekg(0, std::ios::end);
    int count = infile.tellg() / sizeof(*this);
    std::cout << "\nThere are " << count << " records in the file";
    std::cout << "\nEnter Record Number to Search: ";
    std::cin >> n;
    infile.seekg((n - 1) * sizeof(*this));
    infile.read(reinterpret_cast<char*>(this), sizeof(*this));
    std::cout << "-------------------------------" << std::endl;
    showData();
    std::cout << std::endl;
}

void Account::editRecord() {
    int n;
    std::fstream iofile;
    iofile.open("record.bank", std::ios::in | std::ios::binary);
    if (!iofile) {
        std::cout << "Error! File Not Found!" << std::endl;
        return;
    }
    iofile.seekg(0, std::ios::end);
    int count = iofile.tellg() / sizeof(*this);
    std::cout << "\nThere are " << count << " records in the file";
    std::cout << "\nEnter Record Number to edit: ";
    std::cin >> n;
    std::cout << std::endl;
    iofile.seekg((n-1) * sizeof(*this));
    iofile.read(reinterpret_cast<char*>(this), sizeof(*this));
    std::cout << "Record " << n << " has following data: " << std::endl;
    std::cout << "-------------------------------" << std::endl;
    showData();
    iofile.close();
    iofile.open("record.bank", std::ios::out | std::ios::in | std::ios::binary);
    iofile.seekp((n - 1) * sizeof(*this));
    std::cout << "\nEnter Data to Modify " << std::endl;
    getData();
    iofile.write(reinterpret_cast<char*>(this), sizeof(*this));
    iofile.close();
}

void Account::deleteRecord() {
    int n;
    std::ifstream infile;
    infile.open("record.bank", std::ios::binary);
    if (!infile) {
        std::cout << "Error! File Not Found!" << std::endl;
        return;
    }
    infile.seekg(0, std::ios::end);
    int count = infile.tellg() / sizeof(*this);
    std::cout << "\nThere are " << count << " records in the file";
    std::cout << "\nEnter Record Number to Delete: ";
    std::cin >> n;
    std::ofstream tmpfile;
    tmpfile.open("tmpfile.bank", std::ios::binary);
    infile.seekg(0);
    for (int i = 0; i < count; i++) {
        infile.read(reinterpret_cast<char*>(this), sizeof(*this));
        if (i == (n-1)) {
            continue;
        }
        tmpfile.write(reinterpret_cast<char*>(this), sizeof(*this));
    }
    infile.close();
    tmpfile.close();
    std::cout << "\n*** Deleting Record " << n << " *** \n" << std::endl;
    std::cout << "-------------------------------" << std::endl;
    showData();
    std::cout << std::endl;
    remove("record.bank");
    rename("tmpfile.bank", "record.bank");
    std::cout << "Record Successfully Deleted!" << std::endl;
    std::cout << std::endl;
}

int main() {
    Account myAccount;
    int choice;
    std::cout << "*** Acount Information System ***" << std::endl;
    while(true)
    {
        std::cout << "Select one option below ";
        std::cout << "\n\t1 --> Add record to file";
        std::cout << "\n\t2 --> Show all records from file";
        std::cout << "\n\t3 --> Search record from file";
        std::cout << "\n\t4 --> Update record";
        std::cout << "\n\t5 --> Delete record";
        std::cout << "\n\t6 --> Quit";
        std::cout << "\nEnter your choice: ";
        std::cin >> choice;
        switch (choice) {
        case 1:
            myAccount.writeRecord();
            break;
        case 2:
            myAccount.readRecord();
            break;
        case 3:
            myAccount.searchRecord();
            break;
        case 4:
            myAccount.editRecord();
            break;
        case 5:
            myAccount.deleteRecord();
            break;
        case 6:
            exit(0);
            break;
        default:
            std::cout << "\nEnter correct choice";
            exit(0);
        }
    }
    system("pause");
    return 0;
}
