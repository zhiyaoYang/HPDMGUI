# Heat Pump Design Model Graphical User Interface

The HPDMGUI is the desktop GUI application for the HPDM tool developed by ORNL.

todo list:

* ver 0.0, only for existing .hpdm case, not configuration change (no adding new component), load and write .hpdm file
	* linkDialog: add/remove var/successive
	* replace component icon with pic
	* component list dock (maybe used for more func based on item selected?)
	
	* system level settings (referigerant)
	* write into .hpdm file
	* run calculation
	* read and display results
	
* later version
	* dialogs: apply valid check
	* designated icon/schematic for each component
	* system level overview table (master panel)
	* fomula: special component
	* sync with external component list & allow adding new component
	* parametric


	
last updated: 4/19/2017


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