#include <iostream>
#include <cstring>
#include <omp.h>
#include <openssl/evp.h>
#include <pthread.h>

// Function that calculates sha512 hash of some string
void compute_sha512(const unsigned char* input, size_t length, unsigned char* output) {
    EVP_MD_CTX* mdctx;
    const EVP_MD* md;
    unsigned int len;

    // Initialize the OpenSSL library
    if (OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CONFIG, NULL) == 0) {
        std::cerr << "Failed to initialize OpenSSL library" << std::endl;
        return;
    }

    // Create a new digest context
    mdctx = EVP_MD_CTX_new();
    if (mdctx == nullptr) {
        std::cerr << "Failed to create EVP_MD_CTX" << std::endl;
        return;
    }

    // Get the SHA-512 digest
    md = EVP_sha512();
    if (md == nullptr) {
        std::cerr << "Failed to get SHA-512 digest" << std::endl;
        EVP_MD_CTX_free(mdctx);
        return;
    }

    // Initialize the digest context
    if (EVP_DigestInit_ex(mdctx, md, nullptr) != 1) {
        std::cerr << "Failed to initialize digest context" << std::endl;
        EVP_MD_CTX_free(mdctx);
        return;
    }

    // Update the digest with the input data
    if (EVP_DigestUpdate(mdctx, input, length) != 1) {
        std::cerr << "Failed to update digest" << std::endl;
        EVP_MD_CTX_free(mdctx);
        return;
    }

    // Finalize the digest
    if (EVP_DigestFinal_ex(mdctx, output, &len) != 1) {
        std::cerr << "Failed to finalize digest" << std::endl;
        EVP_MD_CTX_free(mdctx);
        return;
    }

    // Clean up
    EVP_MD_CTX_free(mdctx);
}

// Function to pin the current thread to a specific core
void set_thread_affinity(int core_id) {
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(core_id, &cpuset);

    pthread_t thread = pthread_self();
    if (pthread_setaffinity_np(thread, sizeof(cpu_set_t), &cpuset) != 0) {
        std::cerr << "Failed to set thread affinity for core " << core_id << std::endl;
    }
}

// In this sample program, we execute several dummy hash operations
// in single thread pinned to a specific core.
// The goal is to understand thread affinity and differences
// between P-cores and E-cores.
// Also, you may want to check core isolation benefits using
// this program.
int main() {
    // Example string to hash
    const char* text = "Hello, world!";
    size_t length = std::strlen(text);

    // Output buffer for SHA-512 hash
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len;

    // Total amount of threads. My setup is a Intel Core i3 1215u
    // with 8 threads (2+2 P-cores/Hyperthreading and 4 E-cores)
    int thread_num = 8;

    // Each thread executes the dummy hash operation
    for (int thread = 0; thread < thread_num; thread++) {
    	// Timing variables
    	double start_time, end_time;
    	start_time = omp_get_wtime();

    	// Pin this single thread to a specific core
    	set_thread_affinity(thread);

    	// Compute the SHA-512 hash dummy operation
        compute_sha512(reinterpret_cast<const unsigned char*>(text), length, hash);
    	int it = 10000000;
    	while (it > 0) {
    		compute_sha512(reinterpret_cast<const unsigned char*>(hash), (size_t)EVP_MAX_MD_SIZE, hash);
    		it--;
    	}

        // End timing
        end_time = omp_get_wtime();

        // Print elapsed time
        std::cout << "Elapsed time: " << (end_time - start_time) << " seconds for thread " << thread << std::endl;
    }

    // Print the hash in hexadecimal format
    std::cout << "SHA-512 hash: ";
    for (int i = 0; i < 64; ++i) {
        printf("%02x", hash[i]);
    }
    std::cout << std::endl;

    return 0;
}
