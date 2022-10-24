import subprocess
import filecmp

while True:
    subprocess.Popen("./gen 500 > data.in && ./klevel-bf && ./klevel",shell=True).wait()
    try:
        status = filecmp.cmp("klevel.out", "klevelbf.out")
        if status:
            print("files are the same")
        else:
            print("files are different")
            break
    except IOError:
        print("Error:"+ "File not found or failed to read")