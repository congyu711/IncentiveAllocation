import subprocess
import filecmp

while True:
    subprocess.Popen("./gen 100000 > data.in && ./bruteforce && ./kpq",shell=True).wait()
    try:
        status = filecmp.cmp("kpq.out", "bf.out")
        if status:
            print("files are the same")
        else:
            print("files are different")
            break
    except IOError:
        print("Error:"+ "File not found or failed to read")