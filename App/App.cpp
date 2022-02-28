/*
 * Copyright (C) 2011-2019 Intel Corporation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *   * Neither the name of Intel Corporation nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */


#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <unistd.h>
#include <pwd.h>

#include "sgx_urts.h"
#include "Enclave_u.h"

#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <algorithm>    // std::max


# define TOKEN_FILENAME   "enclave.token"
# define ENCLAVE_FILENAME "enclave.signed.so"
/* Global EID shared by multiple threads */

typedef struct _sgx_errlist_t {
    sgx_status_t err;
    const char *msg;
    const char *sug; /* Suggestion */
} sgx_errlist_t;

/* Error code returned by sgx_create_enclave */
static sgx_errlist_t sgx_errlist[] = {
    {
        SGX_ERROR_UNEXPECTED,
        "Unexpected error occurred.",
        NULL
    },
    {
        SGX_ERROR_INVALID_PARAMETER,
        "Invalid parameter.",
        NULL
    },
    {
        SGX_ERROR_OUT_OF_MEMORY,
        "Out of memory.",
        NULL
    },
    {
        SGX_ERROR_ENCLAVE_LOST,
        "Power transition occurred.",
        "Please refer to the sample \"PowerTransition\" for details."
    },
    {
        SGX_ERROR_INVALID_ENCLAVE,
        "Invalid enclave image.",
        NULL
    },
    {
        SGX_ERROR_INVALID_ENCLAVE_ID,
        "Invalid enclave identification.",
        NULL
    },
    {
        SGX_ERROR_INVALID_SIGNATURE,
        "Invalid enclave signature.",
        NULL
    },
    {
        SGX_ERROR_OUT_OF_EPC,
        "Out of EPC memory.",
        NULL
    },
    {
        SGX_ERROR_NO_DEVICE,
        "Invalid SGX device.",
        "Please make sure SGX module is enabled in the BIOS, and install SGX driver afterwards."
    },
    {
        SGX_ERROR_MEMORY_MAP_CONFLICT,
        "Memory map conflicted.",
        NULL
    },
    {
        SGX_ERROR_INVALID_METADATA,
        "Invalid enclave metadata.",
        NULL
    },
    {
        SGX_ERROR_DEVICE_BUSY,
        "SGX device was busy.",
        NULL
    },
    {
        SGX_ERROR_INVALID_VERSION,
        "Enclave version was invalid.",
        NULL
    },
    {
        SGX_ERROR_INVALID_ATTRIBUTE,
        "Enclave was not authorized.",
        NULL
    },
    {
        SGX_ERROR_ENCLAVE_FILE_ACCESS,
        "Can't open enclave file.",
        NULL
    },
};


/* Check error conditions for loading enclave */
void print_error_message(sgx_status_t ret)
{
    size_t idx = 0;
    size_t ttl = sizeof sgx_errlist/sizeof sgx_errlist[0];

    for (idx = 0; idx < ttl; idx++) {
        if(ret == sgx_errlist[idx].err) {
            if(NULL != sgx_errlist[idx].sug)
                printf("Info: %s\n", sgx_errlist[idx].sug);
            printf("Error: %s\n", sgx_errlist[idx].msg);
            break;
        }
    }
    
    if (idx == ttl)
    	printf("Error code is 0x%X. Please refer to the \"Intel SGX SDK Developer Reference\" for more details.\n", ret);
}


/* Initialize the enclave:
 *   Call sgx_create_enclave to initialize an enclave instance
 */

unsigned long int eid;

extern "C" {
    unsigned long int initialize_enclave(void)
    {
        sgx_status_t ret = SGX_ERROR_UNEXPECTED;
        /* Call sgx_create_enclave to initialize an enclave instance */
        /* Debug Support: set 2nd parameter to 1 */
        ret = sgx_create_enclave(ENCLAVE_FILENAME, SGX_DEBUG_FLAG, NULL, NULL, &eid, NULL);
        if (ret != SGX_SUCCESS) {
            print_error_message(ret);
            return -1;
        }

        return eid;
    }

    void destroy_enclave() {
        sgx_destroy_enclave(eid);
    }


    // c++ to pythorh interface
    // TODO: Uncomment removeNoisePython after done with implementing ecalls
    void precomputePython(float* weight, int* dim) {
        sgx_status_t ret = SGX_ERROR_UNEXPECTED;
        // ret = ecall_precompute(eid, inp, dim);
        if (ret != SGX_SUCCESS) {
            print_error_message(ret);
            return;
        }
    }

    // c++ to pythorh interface
    // TODO: Uncomment addNoisePython after done with implementing ecalls
    void addNoisePython(float* inp, int* dim, float* out) {
        sgx_status_t ret = SGX_ERROR_UNEXPECTED;
        // ret = ecall_addNoise(eid, inp, dim, out);

        if (ret != SGX_SUCCESS) {
            print_error_message(ret);
            return;
        }
    }

    // c++ to pythorh interface
    // TODO: Uncomment removeNoisePython after done with implementing ecalls
    void removeNoisePython(float* inp, int* dim, float* out) {
        sgx_status_t ret = SGX_ERROR_UNEXPECTED;
        // ret = ecall_removeNoise(eid, inp, dim, out);

        if (ret != SGX_SUCCESS) {
            print_error_message(ret);
            return;
        }
    }

    // c++ to pythorh interface
    // TODO: Uncomment removeNoisePython after done with implementing ecalls
    void nativeMatMul(float* w, int* dimW, float* inp, int* dimInp, float* out) {
        sgx_status_t ret = SGX_ERROR_UNEXPECTED;
        // ret = ecall_nativeMatMul(eid, w, dimW, inp, dimInp, out);
        
        if (ret != SGX_SUCCESS) {
            print_error_message(ret);
            return;
        }
    }
}

/* OCall functions */
void ocall_print_string(const char *str)
{
    /* Proxy/Bridge will check the length and null-terminate 
     * the input string to prevent buffer overflow. 
     */
    printf("%s", str);
}


int SGX_CDECL main(int argc, char *argv[])
{   

    if (argc != 2) {
        std::cerr << argv[0] << " [num of elements]" << std::endl;
        return -1;
    }

    int size =  std::stoi(argv[1]);

    // enclave creations
    eid = initialize_enclave(); // ECREATE EADD EEXTEND EINIT

    int* inp = new int[size];
    
    for (int i  = 0; i < size; i++)
        inp[i] = rand() % 30;

    // encrypt the inp buffer
    // use memcpy and seal_function
    // ....

    int retVal;

    // argument order
    // eid, [return value pointer], arguments ...
    ecall_compute_secrete_operation(eid, &retVal, inp, size);  // EENTER
    
    std::cout << "Result is " << retVal << std::endl;

    delete [] inp;
    /* Destroy the enclave */
    sgx_destroy_enclave(eid);

    return 0;
}