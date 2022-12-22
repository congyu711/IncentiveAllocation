import os
import random

n = 20000
B = 15500000
for i in range(500):
    seed = random.randint(0,123123)
    print(seed)
    os.system("./lpcallback "+str(seed)+" "+str(n)+" "+str(B)+">> lp.out")
    os.system("./search_greedy "+str(seed)+" "+str(n)+" "+str(B)+">> greedy.out")