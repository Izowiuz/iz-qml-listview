import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.12

import IzLibrary 1.0
import IzListView 1.0

IzListViewImpl {
	id: root

	// view's alias
	readonly property alias listView: listView

	// proxy alias
	readonly property alias proxy: proxyModel

	// max filter columns count
	property int maxColumnFilters: 4

	// if set to true delegates of the view will be enabled
	property bool delegatesEnabled: true

	// true if internal list view has valid selection
	readonly property bool hasSelection: listView.currentIndex !== -1

	// current index of the list view
	readonly property alias currentIndex: listView.currentIndex

	// if true ListView will enable keyboard navigation
	property alias keyNavigationEnabled: listView.keyNavigationEnabled

	// emited when delegate was double clicked
	signal doubleClicked(int index)

	focus: true

	Connections {
		target: proxyModel.sourceModel
		ignoreUnknownSignals: true

		onDataRefreshStarted: {
			listView.prerefreshContentY = listView.contentY;
			listView.currentIndex = -1;
		}

		onDataRefreshEnded: {
			// WARNING: hack :D
			listView.model = proxyModel;
			if (listView.contentHeight > listView.prerefreshContentY) {
				listView.contentY = listView.prerefreshContentY;
			}
		}
	}

	ProgressBar {
		x: -height
		anchors {
			top: mainLayout.top
			topMargin: filtersGrid.height
			left: parent.left
			right: parent.right
		}

		indeterminate: true
		visible: proxyModel.isFiltering
	}

	ColumnLayout {
		id: mainLayout

		anchors.fill: parent

		GridLayout {
			id: filtersGrid

			implicitWidth: parent.width
			visible: !root.filtersModel.empty

			columns: root.maxColumnFilters

			Repeater {
				model: root.filtersModel

				delegate: RowLayout {
					Layout.fillWidth: true
					Layout.preferredHeight: 30
					Layout.maximumHeight: 30
					Layout.minimumHeight: 30
					Layout.columnSpan: index === root.filtersModel.count - 1 && (root.filtersModel.count % root.maxColumnFilters) !== 0 ?
										   (root.maxColumnFilters - (root.filtersModel.count % root.maxColumnFilters) + 1)
										 : 1

					Item {
						Layout.fillWidth: true
						Layout.fillHeight: true

						Loader {
							id: delegateLoader

							anchors.fill: parent

							source: {
								switch(type) {
								case IzListViewFilter.TextField:
									return "qrc:/include/IzListView/QML/TextFieldFilterDelegate.qml";
								case IzListViewFilter.Completer:
									return "qrc:/include/IzListView/QML/CompleterFilterDelegate.qml";
								case IzListViewFilter.ComboBox:
									return "qrc:/include/IzListView/QML/ComboBoxFilterDelegate.qml";
								default:
									return "";
								}
							}
						}
					}

					IzButton {
						Layout.preferredHeight: 18
						Layout.preferredWidth: 18

						enabled: isFiltered
						fontIcon: "\uf235"
						tooltip: qsTr("Usuń filtr")

						onReleased: {
							if (proxyModel.applyFilter(role, "")) {
								filterValue = "";
								delegateLoader.item.clearData();
							}
						}
					}
				}
			}
		}

		ListView {
			id: listView

			property real prerefreshContentY: 0

			Layout.fillHeight: true
			Layout.fillWidth: true

			boundsBehavior: Flickable.StopAtBounds
			headerPositioning: ListView.OverlayHeader
			footerPositioning: ListView.OverlayFooter
			flickableDirection: Flickable.HorizontalAndVerticalFlick
			clip: true
			spacing: 1
			focus: true
			keyNavigationWraps: true

			model: IzProxyModel {
				id: proxyModel

				sourceModel: root.model

				onIsFilteringChanged: {
					if (isFiltering) {
						listView.currentIndex = -1;
					}
				}
			}

			ScrollBar.vertical: ScrollBar {
				minimumSize: 0.1
				width: 5
			}

			ScrollBar.horizontal: ScrollBar {
				minimumSize: 0.1
				height: 5
			}

			delegate: ItemDelegate {

				readonly property QtObject view: root

				focus: true
				height: root.rowHeightProvider(index, model)
				width: ListView.view.width
				highlighted: ListView.isCurrentItem
				topInset: 0
				bottomInset: 0
				padding: 0
				topPadding: 0
				bottomPadding: 0

				onReleased: {
					if (ListView.view.currentIndex !== index) {
						ListView.view.currentIndex = index;
						listView.forceActiveFocus();
					}
				}

				onDoubleClicked: {
					root.doubleClicked(index);
				}

				contentItem: IzRectangle {
					color: "transparent"

					Rectangle {
						anchors.fill: parent

						color: "transparent"
						border.width: 1
						border.color: root.rowColorProvider(index, model)
						visible: root.rowColorProvider(index, model) !== "transparent"
						radius: 2
					}

					Loader {
						anchors {
							fill: parent
							leftMargin: 4
							rightMargin: 4
							topMargin: 2
							bottomMargin: 2
						}

						enabled: root.delegatesEnabled
						source: root.rowDelegateProvider(index, model)
					}
				}
			}

			onCurrentIndexChanged: {
				root.setCurrentRow(proxyModel.sourceRow(currentIndex));
			}

			Rectangle {
				id: emptyModelIndicator

				width: 200
				height: 40
				anchors.centerIn: parent
				visible: proxyModel.sourceModel.count === 0 && !proxyModel.sourceModel.isRefreshingData

				color: root.Material.background
				border.width: 1
				border.color: Material.color(Material.Blue)

				IzText {
					anchors.centerIn: parent
					text: qsTr("Brak pozycji do wyświetlenia")
				}
			}

			ProgressBar {
				anchors {
					left: parent.left
					right: parent.right
					bottom: parent.bottom
				}

				indeterminate: true
				visible: root.model.isRefreshingData
			}
		}
	}
}
