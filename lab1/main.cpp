#include <iostream>
#include <vector>

const short int AMOUNT_OF_NUMBERS = 10;
const short int AMOUNT_OF_LETTERS = 26;
const short int AMOUNT_OF_DIGITS = 7;

struct TObject{
    std::string key;
    std::string value;
};

void CountSort(int* count, const std::string& flag, std::vector<TObject*>& keyValues, std::vector<TObject*>& sorted, short int rank){
    char c;
    if(flag == "number") {
        c = '0';
    }
    else {
        c = 'A';
    }
    for(int k = 0; k < keyValues.size(); ++k){
        ++count[keyValues[k]->key[rank] - c];
    }
    if(flag == "number") {
        for (short int j = 1; j < AMOUNT_OF_NUMBERS; ++j) {
            count[j] += count[j - 1];
        }
    }
    else {
        for(short int j = 1; j < AMOUNT_OF_LETTERS; ++j){
            count[j] += count[j - 1];
        }
    }
    for(int m = keyValues.size(); m != 0; --m){
        --count[keyValues[m - 1]->key[rank] - c];
        sorted[count[keyValues[m - 1]->key[rank] - c]] = keyValues[m - 1];
    }
}

void RadixSort(std::vector<TObject*>& keyValues){
    int count[AMOUNT_OF_LETTERS] = {};
    std::vector<TObject*> sorted(keyValues.size());
    for(short int i = AMOUNT_OF_DIGITS; i >= 0; --i){
        if (keyValues[0]->key[i] == ' '){
            continue;
        }
        else if(keyValues[0]->key[i] >= '0' && keyValues[0]->key[i] <= '9'){
            CountSort(count, "number", keyValues, sorted, i);
        }
        else{
            CountSort(count, "letter", keyValues, sorted, i);
        }
        for (short int j = 0; j < AMOUNT_OF_LETTERS; ++j) {
            count[j] = 0;
        }
        std::swap(keyValues, sorted);
    }
}

int main() {
    std::ios:: sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    std::string firstPart, secondPart, thirdPart, value;
    std::vector<TObject*> keyValues;
    while(std::cin >> firstPart >> secondPart >> thirdPart >> value){
        TObject* data = new TObject();
        data->key = firstPart + " " + secondPart + " " + thirdPart;
        data->value = value;
        keyValues.push_back(data);
    }
    if (!keyValues.empty()) {
        RadixSort(keyValues);
    }
    for (int i = 0; i < keyValues.size(); ++i) {
        std:: cout << keyValues[i]->key << "\t" << keyValues[i]->value << "\n";
        delete keyValues[i];
    }
    return 0;
}
