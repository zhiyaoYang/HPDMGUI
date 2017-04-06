# Heat Pump Design Model Graphical User Interface

The HPDMGUI is the desktop GUI application for the HPDM tool developed by ORNL.

todo list:

* ver 0.0, only for existing .hpdm case, not configuration change (no adding new component), load and write .hpdm file
	* linkDialog: change system data
	* componentDialog: change system data
	* replace icon with pic
	* toggle connection display
	* system level settings (referigerant)
	* write into .hpdm file
	* run calculation
	* read and display results
	
* later version
	* smaller drawing for stream line component (or jump to designated icons for components in later versions)
	* system level overview table (master panel)
	* fomula: special component
	* sync with external component list & allow adding new component
	* parametric


	
last updated: 4/4/2017


note:
* what is the dock for?
	* system pic & system level paramters?
	* or complete component list to choose from?
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