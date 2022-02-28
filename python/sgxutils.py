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


    def precompute(self, w):
        self.lib.precompute(w.T.cpu())

    def addNoise(self, inp):
        return self.lib.addNoise(inp.cpu()).cuda()

    def removeNoise(self, inp):
        return self.lib.removeNoise(inp.cpu()).cuda()

