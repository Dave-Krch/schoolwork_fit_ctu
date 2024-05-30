#ifndef __PROGTEST__
#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

#include <openssl/evp.h>
#include <openssl/rand.h>

using namespace std;

#endif /* __PROGTEST__ */

bool checkZeroBits(const unsigned char hash[EVP_MAX_MD_SIZE], unsigned int length, int zero_bits) {

    int leading_zero_bits = 0;
    for (unsigned int i = 0; i < length; ++i) {
        unsigned char byte = hash[i];

        int count = 0;
        while ((byte & 0x80) == 0 && count < 8) {
            byte <<= 1;
            ++count;
        }
        leading_zero_bits += count;
        if (count < 8) {
            break;
        }
    }

    return leading_zero_bits >= zero_bits;
}

std::string hash_to_hex_string(const unsigned char *hash, unsigned int length) {
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (unsigned int i = 0; i < length; ++i) {
        ss << std::setw(2) << static_cast<unsigned int>(hash[i]);
    }
    return ss.str();
}

int findHashEx (int numberZeroBits, string & outputMessage, string & outputHash, string_view hashType) {

    if(numberZeroBits < 0 || numberZeroBits > EVP_MAX_MD_SIZE)
        return 0;

    //typ hash funkce
    const EVP_MD * type;
    //kontext
    EVP_MD_CTX * ctx;
    //message
    unsigned char message[EVP_MAX_MD_SIZE];
    //hash
    unsigned char hash[EVP_MAX_MD_SIZE];
    //delka hashe
    unsigned int length;

    OpenSSL_add_all_digests();

    //generovani zpravy
    if(!RAND_bytes(message, EVP_MAX_MD_SIZE))
        return 0;

    //nacteni typu hash funkce
    type = EVP_get_digestbyname(hashType.data());
    if(!type) {
        //std::cout << "Nespravny typ" << std::endl;
        return 0;
    }

    ctx = EVP_MD_CTX_new();
    if(ctx == NULL) {
        //std::cout << "Chyba inicializace kontextu" << std::endl;
        return 0;
    }

    while (true) {
        if (!EVP_DigestInit_ex(ctx, type, NULL))
            return 0;

        if (!EVP_DigestUpdate(ctx, message, EVP_MAX_MD_SIZE))
            return 0;

        if (!EVP_DigestFinal_ex(ctx, hash, &length))
            return 0;


        if(checkZeroBits(hash, length, numberZeroBits))
            break;

        for(unsigned int i = 0; i < EVP_MAX_MD_SIZE; i++) {
            message[i] = hash[i];
        }
    }

    outputMessage = hash_to_hex_string(message, EVP_MAX_MD_SIZE);

    outputHash = hash_to_hex_string(hash, length);

    //std::cout << outputMessage << "\n\n" << outputHash << std::endl;

    EVP_MD_CTX_free(ctx);

    return 1;
}

int findHash (int numberZeroBits, string & outputMessage, string & outputHash) {
    return findHashEx(numberZeroBits, outputMessage, outputHash, "sha512");
}

#ifndef __PROGTEST__

int checkHash(int bits, const string & hash) {
    // DIY
}

int main (void) {
    string hash, message;
    assert(findHash(7, message, hash) == 1);
    assert(!message.empty() && !hash.empty() && checkHash(0, hash));
    message.clear();
    hash.clear();
    assert(findHash(1, message, hash) == 1);
    assert(!message.empty() && !hash.empty() && checkHash(1, hash));
    message.clear();
    hash.clear();
    assert(findHash(2, message, hash) == 1);
    assert(!message.empty() && !hash.empty() && checkHash(2, hash));
    message.clear();
    hash.clear();
    assert(findHash(3, message, hash) == 1);
    assert(!message.empty() && !hash.empty() && checkHash(3, hash));
    message.clear();
    hash.clear();
    assert(findHash(-1, message, hash) == 0);
    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */

