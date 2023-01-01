#!/usr/bin/env python3
import os
import random

n = 20000
B = 15500000
for i in range(25):
    seed = random.randint(0,123123)
    print(seed)
    os.system("./lp "+str(n)+" "+str(B)+" "+str(seed)+" >> lp.out")
    # input("done\n")