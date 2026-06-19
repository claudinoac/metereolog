Import("env")
env.Execute("$PYTHONEXE -m pip install dotenv")

import os
from dotenv import dotenv_values

current_path = os.getcwd() 
env_file_path = os.path.join(current_path, ".env")

env_values = dotenv_values(env_file_path)

env_vars = []
print("---------------- DEFINED ENV VARS ----------------------")
for name, value in env_values.items():
    print(f"{name} = \"{value}\"")
    env_vars.append((f"ENV_{name}", env.StringifyMacro(value)))

print(env_vars)
env.Append(CPPDEFINES=env_vars)
