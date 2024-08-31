import subprocess
import sys

packages_to_install = [
  # [ 'antlr4', [
  #   [ sys.executable, "-m", "pip", "install", "antlr4-tools" ],
  #   [ "echo", "yes", "|", "antlr4" ],
  #   [ sys.executable, "-m", "pip", "install", "antlr4-python3-runtime" ]
  # ]],
  # [ 'overrides', [
  #   [ sys.executable, "-m", "pip", "install", "overrides" ],
  # ]]
]
install_all         = False

if len(sys.argv) >= 2:
  if 'install_all' in sys.argv:
    install_all = True    
    
print("Installing " + str(len(packages_to_install)) + " packages")

for idx, val in enumerate(packages_to_install):
  print()
  print()

  if install_all:
    print(str(idx+1) + "/" + str(len(packages_to_install)) + " " + val[0])
  
  do_install = install_all

  while not do_install:
    print(str(idx+1) + "/" + str(len(packages_to_install)) + " " + val[0])
    for idx_sub, sub in enumerate(val[1]):
      print("  " + str(idx_sub+1) + "/" + str(len(val[1])) + ' ' + ' '.join(sub))
    print("[Y/n] > ", end='')
  
    choice = input().lower()
    if choice in [ 'n' ]:
      break
    if choice in [ '', 'y' ]:
      do_install = True
      break
  
  if do_install:
    for idx_sub, sub in enumerate(val[1]):
      print("  " + str(idx_sub+1) + "/" + str(len(val[1])) + ' ' + ' '.join(sub))
  
      subprocess.check_call(sub, shell=True)
  
print()
print()
print("Done")

if not install_all:
  input()
