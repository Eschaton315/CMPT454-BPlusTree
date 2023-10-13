#include <iostream>
#include <vector>
#include"BPlusTree.h"
#include <ctime>

using namespace std;

// Function Prototypes
void simpleTest();
std::string intToWords(int);

std::string intToWords(int num) {
   static const std::vector<std::string> ones = {"", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
    static const std::vector<std::string> teens = {"ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen", "seventeen", "eighteen", "nineteen"};
    static const std::vector<std::string> tens = {"", "", "twenty", "thirty", "forty", "fifty", "sixty", "seventy", "eighty", "ninety"};

    if (num == 0) {
        return "zero";
    }
    else if (num < 10) {
        return ones[num];
    }
    else if (num < 20) {
        return teens[num - 10];
    }
    else {
        int tensDigit = num / 10;
        int onesDigit = num % 10;
        if (onesDigit == 0) {
            return tens[tensDigit];
        }
        else {
            return tens[tensDigit] + " " + ones[onesDigit];
        }
    }
}



int main() {
    simpleTest();
    cout << endl;
}

void simpleTest()
{
    BPlusTree bp1(4);

    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // Number of random numbers to generate
    int count = 40;

    // Range for the random numbers
    int minRange = 0;
    int maxRange = 99;

    std::vector<int> randomNumbers;

    for (int i = 0; i < count; i++) {
        int randomNumber = std::rand() % (maxRange - minRange + 1) + minRange;
        randomNumbers.push_back(randomNumber);
    }

    std::vector<std::string> words;

    for (int i = 0; i < count; i++) {
    words.push_back(intToWords(randomNumbers[i]));
    }

    for(int i = 0; i < count; i++){
        bp1.insert(randomNumbers[i], words[i]);
    }

    // Insert, remove and find
    /*
    bp1.insert(7, "seven");
    bp1.insert(1, "one");
    bp1.insert(3, "three");
    bp1.insert(9, "nine");
    bp1.insert(5, "five");
    */



    cout << "find 3: " << bp1.find(3) << " (three)" << endl;
    //bp1.remove(7);
    cout << "find 7: " << bp1.find(7) << " (<empty>)" << endl << endl;

    // Printing
    bp1.printKeys();
    cout << endl << "CHECK" << endl;
    cout << "[5]" << endl;
    cout << "[1 3] [5 9]" << endl << endl;
    bp1.printValues();
    cout << endl << "CHECK" << endl;
    cout << "one" << endl << "three" << endl << "five" << endl << "nine" << endl;

    // Copy constructor and op=
    BPlusTree bp2(bp1);
    BPlusTree bp3(7);
    bp3.insert(13, "thirteen");
    bp3 = bp1;

    cout << endl << "simple test complete" << endl;
}


