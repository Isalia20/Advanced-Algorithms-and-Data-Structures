#ifndef QUAD_IOU
#define QUAD_IOU
#include <torch/torch.h>
#include <iostream>

using namespace std;

// // CUDA forward declaration
torch::Tensor calculateIoUCudaTorch(torch::Tensor quad_0, torch::Tensor quad_1);

// //C++ interface
// #define CHECK_CUDA(x) TORCH_CHECK(x.device().is_cuda(), #x "must be a CUDA tensor");
// #define CHECK_CONTIGUOUS(x) TORCH_CHECK(x.is_contiguous(), #x "must be contiguous");
// #define CHECK_INPUT(x) CHECK_CUDA(x); CHECK_CONTIGUOUS(x)

torch::Tensor calculateIoU(torch::Tensor quad_0, torch::Tensor quad_1){
    return calculateIoUCudaTorch(quad_0, quad_1);
}

PYBIND11_MODULE(TORCH_EXTENSION_NAME, m) {
    m.def("calculateIoU", &calculateIoU, "IoU Calculation for Quads (CUDA)");
};

#endif