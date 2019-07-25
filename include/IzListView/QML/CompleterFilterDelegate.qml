import QtQuick 2.12

import IzLibrary 1.0

Completer {
	// used to clear filter data
	function clearData() {
		proxyModel.applyFilter(role, "");
		text = "";
	}

	placeholderText: placeholder + "\u2026"
	roleName: role
	displayRoleName: displayRole === "" ? role : displayRole
	model: filterModel

	onPartialEntrySelected: {
		if(proxyModel.applyFilter(role, data)) {
			filterValue = data;
		} else {
			text = "";
		}
	}

	onEntrySelected: {
		if(proxyModel.applyFilter(role, data[role])) {
			filterValue = data[role];
		} else {
			text = "";
		}
	}

	onEditingFinished: {
		if(proxyModel.applyFilter(role, text)) {
			filterValue = text;
		} else {
			text = "";
		}
	}
}
