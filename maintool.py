import os
import sys
import platform
import subprocess

if __name__ == "__main__":
    print(platform.system())

    if platform.system() == "Windows":
        # Call premake
        subprocess.run(("premake5.exe", "vs2022"))
    elif platform.system() == "Linux":
        # Call premake
        subprocess.run(("./premake5", "gmake"))