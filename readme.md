# Heat Pump Design Model Graphical User Interface

The HPDMGUI is the desktop GUI application for the HPDM tool developed by ORNL.

todo list:

* ver 0.0, only for existing .hpdm case, not configuration change (no adding new component), load and write .hpdm file
	* componentDialog template
	* load & create component from .hpdm file
	* load & create link from .hpdm file
	* linkDialog load and change system data
	* componentDialog load and change system data
	* system level settings (referigerant)
	* write into .hpdm file
	* run calculation
	* read and display results
	
* later version
	* system level overview table (master panel)
	* fomula: special component
	* sync with external component list & allow adding new component
	* parametric


	
last updated: 3/14/2017


note:
* what is the dock for?
	* system pic & system level paramters?
	* dock show/hide

* separate "layout" stage from "parameter setting" stage?
	* layout: move around
		* multiple selection?
			* drag to select
			* toggle to select
			* selection state indicator (color/boundary)
	* parameter setting: no selection/drag/moving
			* single click --> dock
				* component/link
				* blank --> system
			* double click --> dialog

* use enum for scene status