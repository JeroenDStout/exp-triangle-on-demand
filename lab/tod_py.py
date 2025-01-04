# %matplotlib widget

import lab_helper
lab_helper.import_tod_pyd()
from lab_helper import tod_py as tod

import numpy as np
from IPython.display import clear_output
import matplotlib.pyplot as plt
import ipywidgets as widgets
from ipywidgets import interact, interact_manual

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
fig = plt.figure()
im = plt.imshow(image, animated=False)

def render_triangle(size=1.0, spin=0.0, background_colour='#afafaf',
                    colour_1='#ff0000', colour_2='#00ff00', colour_3='#0000ff'):
  triangle_instr = tod.render_triange_instr()
  tod.render_triangle(context, triangle_instr)
  image = tod.get_image(context)
  im.set_array(image)
  pass

interact(
  render_triangle,
  size              = widgets.FloatSlider(min = 0, max = 10, value = 1),
  spin              = widgets.FloatSlider(min = -3.1416, max = 3.1416, value = 0),
  background_colour = widgets.ColorPicker(value='#afafaf'),
  colour_1          = widgets.ColorPicker(value='#ff0000'),
  colour_2          = widgets.ColorPicker(value='#00ff00'),
  colour_3          = widgets.ColorPicker(value='#0000ff')
)

set_triangle_props()
