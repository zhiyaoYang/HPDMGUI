# Heat Pump Design Model Graphical User Interface

The HPDMGUI is the desktop GUI application for the HPDM tool developed by ORNL.

See the latest web-version of HPDM at http://hpdmflex.ornl.gov.

todo list:

* ver 0.0, only for existing .hpdm case, not configuration change (no adding new component), load and write .hpdm file
	* debug: new link setup-->load error
	* load existing batch run
	
* later version
	* dialogs: apply valid check
	* designated icon/schematic for each component
	* system level overview table (master panel)
	* fomula: special component
	* sync with external component list & allow adding new component

	
last updated: 8/2/2017


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