
import widgets.depthfinder
import widgets.navigator
import widgets.diagnostics

class Submarine:
	def __init__(self):
		self.depthfinder = widgets.depthfinder.Depthfinder()
		self.navigator = widgets.navigator.Navigator()
		self.diagnostics = widgets.diagnostics.Diagnostics()