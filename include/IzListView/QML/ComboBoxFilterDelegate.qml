import QtQuick 2.12

import IzLibrary 1.0

IzComboBox {
	// used to clear filter data
	function clearData() {
		proxyModel.applyFilter(role, "");
		currentIndex = -1;
	}

	textRole: displayRole === "" ? role : displayRole
	currentIndex: -1
	model: filterModel
	displayText: placeholder + " - " +currentText

	onActivated: {
		var fv = model.data(model.index(currentIndex, 0), model.roleNameToRole(role));
		if(proxyModel.applyFilter(role, fv)) {
			filterValue = fv;
		} else {
			currentIndex = -1;
		}
	}
}
