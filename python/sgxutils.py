from ctypes import *
from ctypes import POINTER
from ctypes import create_string_buffer as toCChar
from torch.utils.cpp_extension import load

import torch
import numpy as np

class SGXUtils(object):
    def __init__(self):
        self.lib = load(
            name="sgx", 
            sources=["pytorch-extension/sgx_python.cc"]
        )

        self.lib.init_sgx()

    # given a weight & precompute w*r
    def precompute(self, w):
        self.lib.precompute(w.T.cpu())

    # given a input vector, compute inp + r
    def addNoise(self, inp):
        return self.lib.addNoise(inp.cpu()).cuda()

    # recover the results using w*r
    def removeNoise(self, inp):
        return self.lib.removeNoise(inp.cpu()).cuda()

    # run matrix multiplicaiton inside sgx Encalve
    def nativeMatMul(self, w, inp):
        return self.lib.nativeMatMul(w.T.cpu(), inp.cpu())