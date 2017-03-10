# Heat Pump Design Model Graphical User Interface

The HPDMGUI is the desktop GUI application for the HPDM tool developed by ORNL.

todo list:

* ver 0.0, only for existing .hpdm case, not configuration change (no adding new component), load and write .hpdm file
	* load & create component from .hpdm file
	* load & create link from .hpdm file
	* auto populate component parameters into the compDialog
	* write current case info back into the .hpdm file

* later version
	* sync with external component list & allow adding new component


	
last updated: 3/2/2017


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