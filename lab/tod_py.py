# %matplotlib widget

import lab_helper
lab_helper.import_tod_pyd()
from lab_helper import tod_py as tod

import numpy as np
from IPython.display import clear_output
import matplotlib.pyplot as plt

# +
context = tod.create_tod_context()
print(tod.get_version() + ": " + context.get_device_info())

reps = 0
# -

if False:
  tod.render_triangle(context, np.array([ 1, 0.5, 0.5 ]))
  image = tod.get_image(context)
  fig_static = plt.figure()
  im_static  = plt.imshow(image)
  plt.show()

# +
import matplotlib.animation as animation
import functools

tod.clear(context, np.array([ 0, 0, 1 ]))
image = tod.get_image(context)

reps += 1
def animate_func(i, my_im, if_rep):
    global reps
    
    if if_rep != reps:
        return None
    
    if i % 10 == 0:
        clear_output()
        display('#' + str(i))
    
    tod.render_triangle(context, np.array([ 0.5, i / 50, 1 ]))
    image = tod.get_image(context)
    my_im.set_array(image)
    return [ my_im ]

fig = plt.figure()
im = plt.imshow(image, animated=True)

anim = animation.FuncAnimation(fig, functools.partial(animate_func, my_im=im, if_rep=reps), frames=100, interval=10, blit=True, repeat=True)
# -

