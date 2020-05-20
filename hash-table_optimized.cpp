#include <iostream>
#include <string.h>
#include <time.h>
#include "text_file_worker.hpp"

const unsigned int HASH_TABLE_SIZE = 10000;

const unsigned int SEARCH_NUMBER = 100000000;

const int OUTPUT_FILE_NAME_BUFFER_SIZE = 20;

const char* INPUT_FILE_NAME = "words.txt";

const unsigned int HASH_CALC_BASE = 53;

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

    HashTable() {
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

    bool contains (HashData desired) {
        unsigned int hash_result = hash_calc(desired);
        HashNode* currNode = table[hash_result%HASH_TABLE_SIZE];

        while (currNode) {
            if (!strcmp(desired, currNode->data))
                return true;
            currNode = currNode->next;
        }

        return false;
    }

private:

    unsigned int hash_calc(HashData data) {
        unsigned int len = strlen(data);
        unsigned int result = 0;

        /*unsigned int factor = 1;

        for (unsigned int i = 0; i < len; ++i) {
            result += data[i]*factor;
            factor *= base;
        }*/

        asm (".intel_syntax noprefix\n"
             "xor edx, edx\n"
             "xor edi, edi\n inc edi\n"
             "loop_note:\n"
             "xor eax, eax\n"
             "lodsb\n"
             "mov r8, rdx\n"
             "mul edi\n"
             "mov rdx, r8\n"
             "add edx, eax\n"
             "mov eax, edi\n"
             "mul bl\n"
             "mov edi, eax\n"
             "cmp ecx, 0\n"
             "ja loop_note\n"
             ".att_syntax prefix\n"
             :"=d"(result), "=c"(len)
             :"b" (HASH_CALC_BASE), "c"(len), "S"(data)
             :"%eax", "%edi");
        return result;
    }



    HashNode* table[HASH_TABLE_SIZE];

};

int main() {

    clock_t begin = clock();

    file_info input_file = file_worker(INPUT_FILE_NAME);

    HashTable hash_table;

    unsigned int words_number = input_file.number_of_strings;
    for (int j = 0; j < words_number; ++j)
        hash_table.add(input_file.stringpointer[j].b_ptr);

    for (unsigned int i = 0; i < SEARCH_NUMBER; ++i) {
        hash_table.contains( input_file.stringpointer[ rand() % words_number ].b_ptr);
    }

    clock_t end = clock();

    printf("Execution time: %.lf\n", (double)(end - begin));

    return 0;
}
