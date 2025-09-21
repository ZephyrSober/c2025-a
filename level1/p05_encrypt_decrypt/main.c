#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define DEFAULTKEY 10

char* read_string() {
    const size_t INITIAL_BUFFER_SIZE = 128;
    const int GROWTH_FACTOR = 2;
    size_t buff_size = INITIAL_BUFFER_SIZE;

    char* buff = (char*)malloc(buff_size*sizeof(char));
    buff[0]='\0';
    size_t total_lenth = 0;
    char ch;

    printf("input string:");
    while ((ch = getchar()) != '\n') {
        if (total_lenth+1 >= buff_size) {
            buff_size *= GROWTH_FACTOR;
            char* new_buff = realloc(buff,buff_size*sizeof(char));
            buff = new_buff;
        }
        buff[total_lenth++]=ch;
    }
    buff[total_lenth]='\0';
    return buff;
}

char* encrypt(char* str) {
    for (int i = 0; i != strlen(str); i++) {
        str[i] += (i==0) ? DEFAULTKEY : str[i-1]%10;
    }
    return str;
}

char* decrypt(char* str) {
    char previous_key = DEFAULTKEY;
    char current_key = 0;
    for (int i = 0; i != strlen(str); i++) {
        current_key = str[i]%10;
        str[i] -= previous_key;
        previous_key = current_key;
    }
    return str;
}

int main() {
    char* plaintext = read_string();
    printf("%s\n",plaintext);
    char* ciphertext = encrypt(plaintext);
    printf("%s\n",ciphertext);
    char* decrypttext = decrypt(ciphertext);
    printf("%s\n",decrypttext);
    return 0;
}