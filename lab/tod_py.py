import lab_helper
lab_helper.import_tod_pyd()
from lab_helper import tod_py as tod

tod.get_version()

context = tod.create_tod_context()
print(context.get_device_info())


