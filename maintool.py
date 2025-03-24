import os
import sys
import subprocess

if __name__ == "__main__":
    # Call premake
    subprocess.run(("premake5.exe", "vs2022"))