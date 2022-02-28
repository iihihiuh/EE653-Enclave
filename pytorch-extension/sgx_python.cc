#include <torch/extension.h>

#include <iostream>
#include <vector>
#include <dlfcn.h>

#define SGXLIB "App/app.so"

static void* lib_;

void init_sgx() {
    // load the lib
    lib_ = dlopen(SGXLIB, RTLD_NOW);
    typedef void (* initialize_enclave_t) (void);
    initialize_enclave_t func = (initialize_enclave_t) dlsym(lib_, "initialize_enclave");
    // init sgx
    func();
}

torch::Tensor addNoise(torch::Tensor inp) {
    typedef void (* addNoise_t) (float* inp, int* dim, float* out);
    addNoise_t func = (addNoise_t) dlsym(lib_, "addNoisePython");

    auto res = torch::ones({inp.size(0), inp.size(1)});
    int* dim = new int[2];
    dim[0] = inp.size(0);
    dim[1] = inp.size(1);

    func((float*) inp.data_ptr(), dim, (float*) res.data_ptr());

    delete[] dim;
    return res;
}

torch::Tensor removeNoise(torch::Tensor inp) {
    typedef void (* removeNoise_t) (float* inp, int* dim, float* out);
    removeNoise_t func = (removeNoise_t) dlsym(lib_, "removeNoisePython");

    auto res = torch::ones({inp.size(0), inp.size(1)});
    int* dim = new int[2];
    dim[0] = inp.size(0);
    dim[1] = inp.size(1);

    func((float*) inp.data_ptr(), dim, (float*) res.data_ptr());

    delete[] dim;
    return res;
}

void precompute(torch::Tensor w, int batch) {
    typedef void (* precompute_t) (float* w, int* dim, int batch);
    precompute_t func = (precompute_t) dlsym(lib_, "precomputePython");

    int* dim = new int[2];
    dim[0] = w.size(0);
    dim[1] = w.size(1);
    func((float*) w.data_ptr(), dim, batch);
    delete[] dim;
}

torch::Tensor nativeMatMul(torch::Tensor w, torch::Tensor inp) {
    typedef void (* matMul_t) (float* w, int* dimW, float* inp, int* dimInp, float* out);
    matMul_t func = (matMul_t) dlsym(lib_, "nativeMatMul");

    auto res = torch::ones({inp.size(0), w.size(1)});

    int* dimW = new int[2];
    dimW[0] = w.size(0);
    dimW[1] = w.size(1);

    int* dimInp = new int[2];
    dimInp[0] = inp.size(0);
    dimInp[1] = inp.size(1);

    func((float*) w.data_ptr(), dimW, (float*) inp.data_ptr(), dimInp, (float*) res.data_ptr());

    delete [] dimInp;
    delete [] dimW;
    return res;
}

PYBIND11_MODULE(TORCH_EXTENSION_NAME, m) {
  m.def("init_sgx", &init_sgx, "init_sgx");
  m.def("addNoise", &addNoise, "addNoise");
  m.def("removeNoise", &removeNoise, "removeNoise");
  m.def("precompute", &precompute, "precompute");
  m.def("nativeMatMul", &nativeMatMul, "nativeMatMul");
}