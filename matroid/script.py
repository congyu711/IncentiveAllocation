import os
import random

n = 200000
B = 155000000
for i in range(5):
    seed = random.randint(0,123123)
    print(seed)
    os.system("./lpcallback "+str(seed)+" "+str(n)+" "+str(B)+">> lp.out")
    os.system("./search_greedy "+str(seed)+" "+str(n)+" "+str(B)+">> greedy.out")
    # input("done\n")