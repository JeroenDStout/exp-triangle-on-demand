import os
import subprocess
import sys
from pathlib import Path

root_path = os.path.realpath(os.path.dirname(__file__) + "/../")
junc_path = root_path + "/_junc_tree/"
if len(sys.argv) >= 2:
  root_path = sys.argv[1]  

print("Setting up junctions for " + root_path)

folder_types = [ "cmake", "src", "include", "script", "assets" ]
folder_map   = {}

for folder_t in folder_types:
  # Get canonical file path
  dir_path = root_path + "/" + folder_t
  if not os.path.isdir(dir_path):
    continue
    
  # Gather all generated files
  for r, ds, f in os.walk(dir_path):
    for d in ds:
      if not d in folder_map:
        folder_map[d] = {}
      folder_map[d]["can_" + folder_t] = dir_path + "/" + d
    break
    
  # Get generated file path
  dir_path = root_path + "/build/gen/" + folder_t
  if not os.path.isdir(dir_path):
    continue
    
  # Gather all generated files
  for r, ds, f in os.walk(dir_path):
    for d in ds:
      if not d in folder_map:
        folder_map[d] = {}
      folder_map[d]["gen_" + folder_t] = dir_path + "/" + d
    break
   
# Create tree
for pr_k, pr_v in folder_map.items():
  base_path = junc_path + "/" + pr_k
  Path(base_path).mkdir(parents=True, exist_ok=True) 

  # Make sure the dir is empty
  for _, dirs, _ in os.walk(base_path):
    for d in dirs:
      Path(base_path + "/" + d).rmdir()
  
  # Add links
  for d_k, d_v in pr_v.items():
    subprocess.check_call('mklink /J "%s" "%s"' % (base_path + "/" + d_k + "/", d_v + "/"), shell=True)
