import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12

import IzLibrary 1.0
import IzListView 1.0

IzTextField {
	id: root

	// completer's model
	property QtObject model: null

	// filtered role name
	property string roleName

	// role name used to display value
	// defaults to roleName if not set
	property string displayRoleName: roleName

	// true if entry was selected
	property bool selectionIsValid: false

	// emited when entry was selected
	signal entrySelected(var data)

	// emited when partial entry was selected
	signal partialEntrySelected(string data)

	// returns data form underlying model
	function getSelectedModelData(roleName) {
		if (!entrySelected) {
			return undefined;
		}
		return completerListView.proxy.dataFromRoleName(0, 0, roleName);
	}

	onTextChanged: {
		if (filterDelay.running) {
			filterDelay.restart();
		} else {
			filterDelay.start();
		}
		root.selectionIsValid = false;
	}

	Keys.onDownPressed: {
		completerListView.listView.incrementCurrentIndex();
	}

	Keys.onUpPressed: {
		completerListView.listView.decrementCurrentIndex();
	}

	Keys.onEnterPressed: {
		if (completerListView.listView.currentItem !== null) {
			text = completerListView.listView.currentItem.text;
		}
	}

	Keys.onReturnPressed: {
		if (completerListView.listView.currentItem !== null) {
			text = completerListView.listView.currentItem.text;
		}
		root.partialEntrySelected(text);
	}

	Timer {
		id: filterDelay

		interval: 500

		onTriggered: {
			completerListView.proxy.applyFilter(roleName, text);
		}
	}

	Popup {
		y: root.height
		width: root.width

		contentHeight: completerListView.listView.contentHeight < 200 ? completerListView.listView.contentHeight : 200
		focus: false
		closePolicy: Popup.NoAutoClose
		visible: root.activeFocus
		padding: 0

		background: Rectangle {
			color: Material.color(Material.Grey, Material.Shade800)
			border.width: 1
			border.color: Material.color(Material.Grey)
		}

		contentItem: IzListView {
			id: completerListView

			model: root.model

			listView.delegate: ItemDelegate {
				text: model[root.displayRoleName]
				height: 24
				width: ListView.view.width
				highlighted: ListView.isCurrentItem

				onReleased: {
					root.text = model[root.displayRoleName];
					root.selectionIsValid = true;
					root.entrySelected(completerListView.proxy.rowAsMap(index));
				}

				contentItem: IzText {
					text: parent.text
				}
			}
		}
	}
}
