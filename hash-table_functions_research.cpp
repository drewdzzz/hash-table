#include <iostream>
#include <string.h>
#include "text_file_worker.hpp"

const unsigned int HASH_TABLE_SIZE = 100;

const int OUTPUT_FILE_NAME_BUFFER_SIZE = 20;

const char* INPUT_FILE_NAME = "words.txt";


const int HASH_NUMBER = 7;

typedef const char* HashData;
struct HashNode {
    HashNode* next = nullptr;
    HashData data;

    HashNode(HashData new_data):data(new_data)
    {}

    int length () {
        HashNode* curr = this;
        int length = 0;
        while (curr){
            curr = curr->next;
            ++length;
        }
        return length;
    }
};


class HashTable {
public:

    HashTable(unsigned int (*hash_parameter) (HashData data)) {
        hash_ptr = hash_parameter;
        for (unsigned int i = 0; i < HASH_TABLE_SIZE; ++i)
            table[i] = nullptr;
    }

    void add(HashData data) {
        unsigned int hash_res = hash_calc(data) % HASH_TABLE_SIZE;
        HashNode* next_node = table[hash_res];
        table[hash_res] = new HashNode(data);
        table[hash_res]->next = next_node;
    }

    void write_analysis(FILE* output_file_stream) {
        for (int i = 0; i < HASH_TABLE_SIZE; ++i)
            fprintf(output_file_stream, "%d;", (table[i] ? table[i]->length() : 0));
    }


private:

    unsigned int (*hash_ptr) (HashData data);

    unsigned int hash_calc(HashData data) {
        return hash_ptr(data);
    }

    HashNode* table[HASH_TABLE_SIZE];

};

unsigned int hash_calc1(HashData data);

unsigned int hash_calc2(HashData data);

unsigned int hash_calc3(HashData data);

unsigned int hash_calc4(HashData data);

unsigned int hash_calc5(HashData data);

unsigned int hash_calc6(HashData data);

unsigned int hash_calc7(HashData data);





int main() {

    file_info input_file = file_worker(INPUT_FILE_NAME);

    unsigned int (*hash_functions[HASH_NUMBER]) (HashData);

    hash_functions[0] = hash_calc1;
    hash_functions[1] = hash_calc2;
    hash_functions[2] = hash_calc3;
    hash_functions[3] = hash_calc4;
    hash_functions[4] = hash_calc5;
    hash_functions[5] = hash_calc6;
    hash_functions[6] = hash_calc7;

    char output_file_name[OUTPUT_FILE_NAME_BUFFER_SIZE];
    sprintf (output_file_name, "%d_hash.csv", HASH_TABLE_SIZE);


    FILE* output_stream = fopen(output_file_name, "w");


    for (int i = 0; i < HASH_NUMBER; ++i) {
        HashTable hash_table(hash_functions[i]);
        int words_number = input_file.number_of_strings;
        for (int j = 0; j < words_number; ++j)
            hash_table.add(input_file.stringpointer[j].b_ptr);
        hash_table.write_analysis(output_stream);
        fprintf(output_stream, "\n");
    }

    return 0;
}



    unsigned int hash_calc1(HashData data) {
        return 1;
    }

    unsigned int hash_calc2(HashData data) {
        return strlen(data);
    }

    unsigned int hash_calc3(HashData data) {
        unsigned int sum = 0;
        int i = 0;
        while (data[i])
            sum += data[i++];
        return sum;
    }

    unsigned int hash_calc4(HashData data) {
        int length = strlen(data);
        if (!length)
            return 0;
        return hash_calc3(data) / length;
    }

    unsigned int hash_calc5(HashData data) {
        unsigned int result = 0;
        int i = 0;
        while(data[i]) {
            result ^= data[i++];
            result = (result<<1 | result>>31);
        }
        return result;
    }

    unsigned int hash_calc6(HashData data) {
        unsigned int len = strlen(data);
        unsigned int base = 31;
        unsigned int factor = 1;
        unsigned int result = 0;
        for (unsigned int i = 0; i < len; ++i) {
            result += data[i]*factor;
            factor *= base;
        }
        return result;
    }

    unsigned int hash_calc7(HashData data) {
        unsigned int len = strlen(data);
        unsigned int base = 53;
        unsigned int factor = 1;
        unsigned int result = 0;
        for (unsigned int i = 0; i < len; ++i) {
            result += data[i]*factor;
            factor *= base;
        }
        return result;
    }
