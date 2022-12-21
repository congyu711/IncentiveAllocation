import os
import random

n = 20000
B = 15500000
while True:
    seed = random.randint(0,123123)
    print(seed)
    os.system("./lpcallback "+str(seed)+" "+str(n)+" "+str(B))
    os.system("./search_greedy "+str(seed)+" "+str(n)+" "+str(B))
    input()