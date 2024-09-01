# %matplotlib ipympl

import lab_helper
lab_helper.import_tod_pyd()
from lab_helper import tod_py as tod

import numpy as np
from IPython.display import clear_output
import matplotlib.pyplot as plt
plt.rcParams["animation.html"] = "jshtml"
plt.ioff();

context = tod.create_tod_context()
print(tod.get_version() + ": " + context.get_device_info())

# +
import matplotlib.animation as animation

tod.clear(context, np.array([ 0, 0, 1 ]))
image = tod.get_image(context)

fig = plt.figure()
im = plt.imshow(np.swapaxes(image, 0, 1), animated=True)

def animate_func(i):
    tod.clear(context, np.array([ i / 100, i / 50, 1 ]))
    image = tod.get_image(context)
    im.set_array(np.swapaxes(image, 0, 1))
    return [ im ]

anim = animation.FuncAnimation(fig, animate_func, frames=100, interval=30, blit=True, repeat=False)
anim
# -


