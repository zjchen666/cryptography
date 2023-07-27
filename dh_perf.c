#include <stdio.h>
#include <openssl/dh.h>
#include <openssl/ec.h>
#include <openssl/evp.h>
#include <openssl/ecdh.h>
#include <openssl/dh.h>
#include <openssl/obj_mac.h>
#include <time.h>

void handleErrors() {
    fprintf(stderr, "Error\n");
	return;
}

unsigned char *ecdh_low(size_t *secret_len, EC_KEY *peerkey)
{
	EC_KEY *key;
	int field_size;
	unsigned char *secret;

	/* Create an Elliptic Curve Key object and set it up to use the ANSI X9.62 Prime 256v1 curve */
	if(NULL == (key = EC_KEY_new_by_curve_name(NID_X9_62_prime256v1))) handleErrors();

	/* Generate the private and public key */
	if(1 != EC_KEY_generate_key(key)) handleErrors();

	/* Calculate the size of the buffer for the shared secret */
	field_size = EC_GROUP_get_degree(EC_KEY_get0_group(key));
	*secret_len = (field_size+7)/8;

	/* Allocate the memory for the shared secret */
	if(NULL == (secret = OPENSSL_malloc(*secret_len))) handleErrors();

	/* Derive the shared secret */
	*secret_len = ECDH_compute_key(secret, *secret_len, EC_KEY_get0_public_key(peerkey),
						key, NULL);

	/* Clean up */
	EC_KEY_free(key);

	if(*secret_len <= 0)
	{
		OPENSSL_free(secret);
		return NULL;
	}

	return secret;
}

int ecdh_test() {

    // Choose an elliptic curve (e.g., secp256r1)
    EC_KEY *peer_key = EC_KEY_new_by_curve_name(NID_X9_62_prime256v1);
    if (!peer_key) {
        fprintf(stderr, "Error creating ECDH key\n");
        return 1;
    }

    if (EC_KEY_generate_key(peer_key) != 1) {
        fprintf(stderr, "Error generating ECDH keys\n");
        return 1;
    }

    // Perform ECDH key exchange
    clock_t start_time, end_time;
    int num_iterations = 1000; // Number of iterations for measurement

    start_time = clock();
    for (int i = 0; i < num_iterations; i++) {
        // Perform ECDH key exchange
        size_t secret_len = 0;
        unsigned char *shared_secret = ecdh_low(&secret_len, peer_key);
        // Use the shared secret or do something with it here
        if (secret_len <= 0) {
            fprintf(stderr, "Error generating ECDH keys\n");
            return 1;
        }
        free(shared_secret);
    }
    end_time = clock();

    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Average ECDH execution time: %f seconds\n", elapsed_time / num_iterations);
    printf("Average ECDH : %f per seconds\n", num_iterations / elapsed_time);

    // Cleanup
    EC_KEY_free(peer_key);
    EVP_cleanup();

    return 0;
}

unsigned char* dh(DH *privkey, BIGNUM *pubkey) {

    int secret_size;

    /* Generate the public and private key pair */
    if(1 != DH_generate_key(privkey)) handleErrors();

    /* Compute the shared secret */
    unsigned char *secret;
    if(NULL == (secret = OPENSSL_malloc(sizeof(unsigned char) * (DH_size(privkey))))) handleErrors();

    if(0 > (secret_size = DH_compute_key(secret, pubkey, privkey))) handleErrors();

    return secret;
}

int dh_test() {
    DH *privkey;
    int codes;
    
    // Perform ECDH key exchange
    clock_t start_time, end_time;
    int num_iterations = 1; // Number of iterations for measurement
    
    // Generate DH parameters
    /* Generate the parameters to be used */
    if(NULL == (privkey = DH_new())) handleErrors();
    if(1 != DH_generate_parameters_ex(privkey, 1024, DH_GENERATOR_2, NULL)) handleErrors();

    if(1 != DH_check(privkey, &codes)) handleErrors();
    if(codes != 0)
    {
        /* Problems have been found with the generated parameters */
        /* Handle these here - we'll just abort for this example */
        printf("DH_check failed\n");
        abort();
    }

    /* Receive the public key from the peer. In this example we're just hard coding a value */
    BIGNUM *peer_key = NULL;
    if(0 == (BN_dec2bn(&peer_key, "01234567890123456789012345678901234567890123456789"))) handleErrors();

    start_time = clock();
    for (int i = 0; i < num_iterations; i++) {
        // Perform ECDH key exchange
        unsigned char *shared_secret = dh(privkey, peer_key);
        // Use the shared secret or do something with it here
        OPENSSL_free(shared_secret);
    }
    end_time = clock();

    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Average DH execution time: %f seconds\n", elapsed_time / num_iterations);
    printf("Average DH : %f per seconds\n", num_iterations / elapsed_time);

    // Cleanup
    BN_free(peer_key);
    DH_free(privkey);
    EVP_cleanup();

    return 0;
}
// build command:
// gcc -g -Wall -Wextra -o ecdh ecdh.c -lcrypto
int main() {
    // Initialize OpenSSL
    OpenSSL_add_all_algorithms();
    
    ecdh_test();
    dh_test();
    return 0;
}
