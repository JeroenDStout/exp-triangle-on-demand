def import_tod_pyd():
  global tod_py
  import os
  import sys
  import glob
  import shutil
  import datetime
  
  rel_dir      = os.path.realpath(os.path.dirname(__file__) + "/../..")
  pyd_path_in  = glob.glob(rel_dir + '/*/bin/RelWithDebInfo/tod_py*.pyd')[0]
  
  cache_dir    = rel_dir + '/lab/pyd_cache/'
  if not os.path.exists(cache_dir):
    os.makedirs(cache_dir)
  if cache_dir not in sys.path:
    sys.path.insert(0, cache_dir)
    
  old_cache    = glob.glob(cache_dir + '*.pyd')
  for old in old_cache:
    try:
      os.remove(old)
    except OSError:
      pass
  
  pyd_path_out = cache_dir + 'tod_py.pyd'
  if os.path.exists(pyd_path_out):
    shutil.move(pyd_path_out, pyd_path_out + ".old~" + str(datetime.datetime.now().timestamp()))
  shutil.copyfile(pyd_path_in, pyd_path_out)
        
  import tod_py
  