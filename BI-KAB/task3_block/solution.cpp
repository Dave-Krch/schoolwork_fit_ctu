#ifndef __PROGTEST__
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <climits>
#include <cstdint>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <unistd.h>
#include <string>
#include <memory>
#include <vector>
#include <fstream>
#include <cassert>
#include <cstring>

#include <openssl/evp.h>
#include <openssl/rand.h>

using namespace std;

struct crypto_config
{
	const char * m_crypto_function;
	std::unique_ptr<uint8_t[]> m_key;
	std::unique_ptr<uint8_t[]> m_IV;
	size_t m_key_len;
	size_t m_IV_len;
};

#endif /* _PROGTEST_ */

bool encrypt_data ( const std::string & in_filename, const std::string & out_filename, crypto_config & config ) {

    //typ sifry
    const EVP_CIPHER * cipher = EVP_get_cipherbyname(config.m_crypto_function);
    if(!cipher) {
        return false;
    }

    //delka klice a iv
    size_t key_len = EVP_CIPHER_key_length(cipher);
    size_t IV_len = EVP_CIPHER_iv_length(cipher);

    //vygenerovat klic a iv pokud jsou kratke nebo neexistuji
    if(key_len > config.m_key_len || !config.m_key) {
        config.m_key_len = key_len;
        config.m_key = std::make_unique<uint8_t[]>(key_len);

        if(!RAND_bytes(config.m_key.get(), key_len)) {
            return false;
        }
    }
    if(IV_len != 0 && (IV_len > config.m_IV_len || !config.m_IV) ) {
        config.m_IV_len = IV_len;
        config.m_IV = std::make_unique<uint8_t[]>(IV_len);

        if(!RAND_bytes(config.m_IV.get(), IV_len)) {
            return false;
        }
    }

    //kontext
    EVP_CIPHER_CTX * ctx;
    if(!(ctx = EVP_CIPHER_CTX_new())) {
        return false;
    }
    if(!EVP_EncryptInit_ex(ctx, cipher, NULL, config.m_key.get(), config.m_IV.get())) {
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }

    //--- !! odted vsude pri chybe se musi free ctx a zavrit soubory

    //otevreni souboru
    FILE * input_file = fopen(in_filename.c_str(), "rb");
    FILE * output_file = fopen(out_filename.c_str(), "w");

    if (!input_file || !output_file) {
        if (input_file) fclose(input_file);
        if (output_file) fclose(output_file);
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }

    //buffery pro sifrovani
    size_t block_size = EVP_CIPHER_get_block_size(cipher);

    int encrypted_data_len;

    std::vector<uint8_t> plain_data(block_size + 18);
    std::vector<uint8_t> encrypted_data(block_size + 18);

    size_t bytes_read;
    size_t bytes_writen;

    //cteni hlavicky
    bytes_read = fread(plain_data.data(), 1, 18, input_file);

    if(bytes_read < 18) {
        fclose(input_file);
        fclose(output_file);
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }

    //prepsani hlavicky do output souboru
    bytes_writen = fwrite(plain_data.data(), 1, 18, output_file);

    if(bytes_writen != 18) {
        fclose(input_file);
        fclose(output_file);
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }

    //sifrovani
    while (true) {
        bytes_read = fread(plain_data.data(), 1, block_size, input_file);

        //fail pri cteni
        if(bytes_read != block_size && !feof(input_file)) {
            fclose(input_file);
            fclose(output_file);
            EVP_CIPHER_CTX_free(ctx);
            return false;
        }

        //sifrovani bloku
        if( !EVP_EncryptUpdate(ctx, encrypted_data.data(), &encrypted_data_len, plain_data.data(), int(bytes_read) ) ) {
            fclose(input_file);
            fclose(output_file);
            EVP_CIPHER_CTX_free(ctx);
            return false;
        }

        //zapsani do output souboru
        bytes_writen = fwrite(encrypted_data.data(), 1, encrypted_data_len, output_file);

        if(int(bytes_writen) != encrypted_data_len) {
            fclose(input_file);
            fclose(output_file);
            EVP_CIPHER_CTX_free(ctx);
            return false;
        }

        //eof
        if(feof(input_file)) {
            break;
        }
    }

    //final encrypt a zapsani do souboru
    if( !EVP_EncryptFinal_ex(ctx, encrypted_data.data(), &encrypted_data_len) ) {
        fclose(input_file);
        fclose(output_file);
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }

    bytes_writen = fwrite(encrypted_data.data(), 1, encrypted_data_len, output_file);

    if(int(bytes_writen) != encrypted_data_len) {
        fclose(input_file);
        fclose(output_file);
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }

    fclose(input_file);
    fclose(output_file);
    EVP_CIPHER_CTX_free(ctx);
    return true;
}

bool decrypt_data ( const std::string & in_filename, const std::string & out_filename, crypto_config & config ) {

    //typ sifry
    const EVP_CIPHER * cipher = EVP_get_cipherbyname(config.m_crypto_function);
    if(!cipher) {
        //std::cout << "1" << std::endl;
        return false;
    }

    //delka klice a iv
    size_t key_len = EVP_CIPHER_key_length(cipher);
    size_t IV_len = EVP_CIPHER_iv_length(cipher);

    //fail pokud nejsou klice/jsou kratke
    if(key_len > config.m_key_len || !config.m_key) {
        //std::cout << "2" << std::endl;
        return false;
    }
    if(IV_len != 0 && (IV_len > config.m_IV_len || !config.m_IV) ) {
        //std::cout << "3" << std::endl;
        return false;
    }

    //kontext
    EVP_CIPHER_CTX * ctx;
    if(!(ctx = EVP_CIPHER_CTX_new())) {
        //std::cout << "4" << std::endl;
        return false;
    }
    if(!EVP_DecryptInit_ex(ctx, cipher, NULL, config.m_key.get(), config.m_IV.get())) {
        EVP_CIPHER_CTX_free(ctx);
        //std::cout << "5" << std::endl;
        return false;
    }

    //--- !! odted vsude pri chybe se musi free ctx a zavrit soubory

    //otevreni souboru
    FILE * input_file = fopen(in_filename.c_str(), "rb");
    FILE * output_file = fopen(out_filename.c_str(), "w");

    if (!input_file || !output_file) {
        if (input_file) fclose(input_file);
        if (output_file) fclose(output_file);
        EVP_CIPHER_CTX_free(ctx);
        //std::cout << "6" << std::endl;
        return false;
    }

    //buffery pro de-sifrovani
    size_t block_size = EVP_CIPHER_get_block_size(cipher);

    int plain_data_len;

    std::vector<uint8_t> encrypted_data(block_size + 18);
    std::vector<uint8_t> plain_data(block_size + 18);

    size_t bytes_read;
    size_t bytes_writen;

    //cteni hlavicky
    bytes_read = fread(plain_data.data(), 1, 18, input_file);

    if(bytes_read < 18) {
        fclose(input_file);
        fclose(output_file);
        EVP_CIPHER_CTX_free(ctx);
        //std::cout << "7" << std::endl;
        return false;
    }

    //prepsani hlavicky do output souboru
    bytes_writen = fwrite(plain_data.data(), 1, 18, output_file);

    if(bytes_writen != 18) {
        fclose(input_file);
        fclose(output_file);
        EVP_CIPHER_CTX_free(ctx);
        //std::cout << "9" << std::endl;
        return false;
    }

    //de-sifrovani
    while (true) {
        bytes_read = fread(encrypted_data.data(), 1, block_size, input_file);

        //fail pri cteni
        if(bytes_read != block_size && !feof(input_file)) {
            fclose(input_file);
            fclose(output_file);
            EVP_CIPHER_CTX_free(ctx);
            //std::cout << "10" << std::endl;
            return false;
        }

        //sifrovani bloku
        if( !EVP_DecryptUpdate(ctx, plain_data.data(), &plain_data_len, encrypted_data.data(), int(bytes_read) ) ) {
            fclose(input_file);
            fclose(output_file);
            EVP_CIPHER_CTX_free(ctx);
            //std::cout << "11" << std::endl;
            return false;
        }

        //zapsani do output souboru
        bytes_writen = fwrite(plain_data.data(), 1, plain_data_len, output_file);

        if(int(bytes_writen) != plain_data_len) {
            fclose(input_file);
            fclose(output_file);
            EVP_CIPHER_CTX_free(ctx);
            //std::cout << "12" << std::endl;
            return false;
        }

        //eof
        if(feof(input_file)) {
            break;
        }
    }

    //final encrypt a zapsani do souboru
    if( !EVP_DecryptFinal_ex(ctx, plain_data.data(), &plain_data_len) ) {
        fclose(input_file);
        fclose(output_file);
        EVP_CIPHER_CTX_free(ctx);
        //std::cout << "13" << std::endl;
        return false;
    }

    bytes_writen = fwrite(plain_data.data(), 1, plain_data_len, output_file);

    if(int(bytes_writen) != plain_data_len) {
        fclose(input_file);
        fclose(output_file);
        EVP_CIPHER_CTX_free(ctx);
        //std::cout << "14" << std::endl;
        return false;
    }

    fclose(input_file);
    fclose(output_file);
    EVP_CIPHER_CTX_free(ctx);
    return true;
}


#ifndef __PROGTEST__

bool compare_files ( const char * name1, const char * name2 ) {
    FILE *file1 = fopen(name1, "rb");
    FILE *file2 = fopen(name2, "rb");

    if (!file1 || !file2) {
        if (file1) fclose(file1);
        if (file2) fclose(file2);
        return false;
    }

    fseek(file1, 0, SEEK_END);
    fseek(file2, 0, SEEK_END);
    long size1 = ftell(file1);
    long size2 = ftell(file2);
    if (size1 != size2) {
        fclose(file1);
        fclose(file2);
        return false;
    }

    rewind(file1);
    rewind(file2);

    int byte1, byte2;
    while ((byte1 = fgetc(file1)) != EOF && (byte2 = fgetc(file2)) != EOF) {
        if (byte1 != byte2) {
            fclose(file1);
            fclose(file2);
            return false;
        }
    }

    fclose(file1);
    fclose(file2);
    return true;
}

int main ( void ) {

    std::cout << "compare working" << std::endl;

	crypto_config config {nullptr, nullptr, nullptr, 0, 0};

	// ECB mode
	config.m_crypto_function = "AES-128-ECB";
	config.m_key = std::make_unique<uint8_t[]>(16);
 	memset(config.m_key.get(), 0, 16);
	config.m_key_len = 16;

	assert( encrypt_data  ("homer-simpson.TGA", "out_file.TGA", config) &&
			compare_files ("out_file.TGA", "homer-simpson_enc_ecb.TGA") );

	assert( decrypt_data  ("homer-simpson_enc_ecb.TGA", "out_file.TGA", config) &&
			compare_files ("out_file.TGA", "homer-simpson.TGA") );
/*
	assert( encrypt_data  ("UCM8.TGA", "out_file.TGA", config) &&
			compare_files ("out_file.TGA", "UCM8_enc_ecb.TGA") );

	assert( decrypt_data  ("UCM8_enc_ecb.TGA", "out_file.TGA", config) &&
			compare_files ("out_file.TGA", "UCM8.TGA") );

	assert( encrypt_data  ("image_1.TGA", "out_file.TGA", config) &&
			compare_files ("out_file.TGA", "ref_1_enc_ecb.TGA") );

	assert( encrypt_data  ("image_2.TGA", "out_file.TGA", config) &&
			compare_files ("out_file.TGA", "ref_2_enc_ecb.TGA") );

	assert( decrypt_data ("image_3_enc_ecb.TGA", "out_file.TGA", config)  &&
		    compare_files("out_file.TGA", "ref_3_dec_ecb.TGA") );

	assert( decrypt_data ("image_4_enc_ecb.TGA", "out_file.TGA", config)  &&
		    compare_files("out_file.TGA", "ref_4_dec_ecb.TGA") );

	// CBC mode
	config.m_crypto_function = "AES-128-CBC";
	config.m_IV = std::make_unique<uint8_t[]>(16);
	config.m_IV_len = 16;
	memset(config.m_IV.get(), 0, 16);

	assert( encrypt_data  ("UCM8.TGA", "out_file.TGA", config) &&
			compare_files ("out_file.TGA", "UCM8_enc_cbc.TGA") );

	assert( decrypt_data  ("UCM8_enc_cbc.TGA", "out_file.TGA", config) &&
			compare_files ("out_file.TGA", "UCM8.TGA") );

	assert( encrypt_data  ("homer-simpson.TGA", "out_file.TGA", config) &&
			compare_files ("out_file.TGA", "homer-simpson_enc_cbc.TGA") );

	assert( decrypt_data  ("homer-simpson_enc_cbc.TGA", "out_file.TGA", config) &&
			compare_files ("out_file.TGA", "homer-simpson.TGA") );

	assert( encrypt_data  ("image_1.TGA", "out_file.TGA", config) &&
			compare_files ("out_file.TGA", "ref_5_enc_cbc.TGA") );

	assert( encrypt_data  ("image_2.TGA", "out_file.TGA", config) &&
			compare_files ("out_file.TGA", "ref_6_enc_cbc.TGA") );

	assert( decrypt_data ("image_7_enc_cbc.TGA", "out_file.TGA", config)  &&
		    compare_files("out_file.TGA", "ref_7_dec_cbc.TGA") );

	assert( decrypt_data ("image_8_enc_cbc.TGA", "out_file.TGA", config)  &&
		    compare_files("out_file.TGA", "ref_8_dec_cbc.TGA") );
	return 0;

 */
}

#endif /* _PROGTEST_ */
