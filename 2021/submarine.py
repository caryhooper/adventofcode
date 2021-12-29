import widgets.depthfinder
import widgets.navigator
import widgets.diagnostics
import widgets.entertainment
import widgets.weapons
import documentation.manual
import widgets.tools


class Submarine:
	def __init__(self):
		self.depthfinder = widgets.depthfinder.Depthfinder()
		self.navigator = widgets.navigator.Navigator()
		self.diagnostics = widgets.diagnostics.Diagnostics()
		self.entertainment = {"bingo": widgets.entertainment.Bingo()}
		self.manual = documentation.manual.Manual()
		self.weapons = {"probe_cannon": widgets.weapons.ProbeCannon()}
		self.calculator = widgets.tools.SnailfishCalculator()
