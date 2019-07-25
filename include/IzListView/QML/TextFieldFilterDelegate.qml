import QtQuick 2.12

import IzLibrary 1.0

IzTextField {
	// used to clear filter data
	function clearData() {
		proxyModel.applyFilter(role, "");
		text = "";
	}

	placeholderText: placeholder + "\u2026"

	onEditingFinished: {
		if(proxyModel.applyFilter(role, text)) {
			filterValue = text;
		} else {
			text = "";
		}
	}
}
