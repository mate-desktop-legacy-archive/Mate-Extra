import warnings
warnings.warn("Module mate.applet is deprecated; "
              "please import mateapplet instead",
              DeprecationWarning)
del warnings

from mateapplet import *
