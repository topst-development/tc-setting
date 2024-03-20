/****************************************************************************************
 *   FileName    : BluetoothSearch.qml
 *   Description : BluetoothSearch.qml
 ****************************************************************************************
 *
 *   TCC Version 1.0
 *   Copyright (c) Telechips Inc.
 *   All rights reserved

This source code contains confidential information of Telechips.
Any unauthorized use without a written permission of Telechips including not limited
to re-distribution in source or binary form is strictly prohibited.
This source code is provided “AS IS” and nothing contained in this source code
shall constitute any express or implied warranty of any kind, including without limitation,
any warranty of merchantability, fitness for a particular purpose or non-infringement of any patent,
copyright or other third party intellectual property right.
No warranty is made, express or implied, regarding the information’s accuracy,
completeness, or performance.
In no event shall Telechips be liable for any claim, damages or other liability arising from,
out of or in connection with this source code or the use in the source code.
This source code is provided subject to the terms of a Mutual Non-Disclosure Agreement
between Telechips and Company.
*
****************************************************************************************/

import QtQuick 2.6
import "types"

Item {
	id: bluetoothSearch

	property bool discoverable: true
	property bool searching: false

	signal backPressed

	Text {
		x: 20
		y: 40
		width: 100
		height: 21

		text: "CoD"
		horizontalAlignment: Text.AlignLeft
		color: "lightgray"
		font.pixelSize: 21
	}

	Text {
		id: codInfo

		x: 130
		y: 40
		width: 170
		height: 21

		text: ""
		elide: Text.ElideRight
		horizontalAlignment: Text.AlignLeft
		verticalAlignment: Text.AlignVCenter
		color: "white"
		font.pixelSize: 21
	}

	Text {
		x: 20
		y: 70
		width: 100
		height: 21

		text: "Name"
		horizontalAlignment: Text.AlignLeft
		color: "lightgray"
		font.pixelSize: 21
	}

	Text {
		id: nameInfo

		x: 130
		y: 70
		width: 170
		height: 21

		text: ""
		elide: Text.ElideRight
		horizontalAlignment: Text.AlignLeft
		verticalAlignment: Text.AlignVCenter
		color: "white"
		font.pixelSize: 21
	}

	Text {
		x: 20
		y: 100
		width: 100
		height: 21

		text: "Address"
		horizontalAlignment: Text.AlignLeft
		color: "lightgray"
		font.pixelSize: 21
	}

	Text {
		id: addressInfo

		x: 130
		y: 100
		width: 170
		height: 21

		text: ""
		elide: Text.ElideRight
		horizontalAlignment: Text.AlignLeft
		verticalAlignment: Text.AlignVCenter
		color: "white"
		font.pixelSize: 21
	}

	Text {
		x: 20
		y: 130
		width: 100
		height: 21

		text: "RSSI"
		horizontalAlignment: Text.AlignLeft
		color: "lightgray"
		font.pixelSize: 21
	}

	Text {
		id: rssiInfo

		x: 130
		y: 130
		width: 170
		height: 21

		text: ""
		elide: Text.ElideRight
		horizontalAlignment: Text.AlignLeft
		verticalAlignment: Text.AlignVCenter
		color: "white"
		font.pixelSize: 21
	}

	ImageButton {
		id: discoverButton

		x: 0
		y: 180
		width: 163
		height: 75

		source: {
			if (pressed) {
				return "images/setting_bt_discoverable_prsd.png"
			} else {
				if (bluetoothSearch.discoverable) {
					return "images/setting_bt_discoverable_nor.png"
				} else {
					return "images/setting_bt_discoverable_foc.png"
				}
			}
		}

		onClicked: {
			bluetoothSearch.discoverable = !bluetoothSearch.discoverable
			BluetoothManager.setDiscoverable(bluetoothSearch.discoverable)
		}
	}

	ImageButton {
		id: connectButton

		x: 164
		y: 180
		width: 163
		height: 75

		source: {
			if (nameInfo.text != "") {
				if (pressed) {
					return "images/setting_bt_connect_prsd.png"
				} else {
					return "images/setting_bt_connect_nor.png"
				}
			} else {
				return "images/setting_bt_connect_dis.png"
			}
		}

		onClicked: {
			if (addressInfo.text != "") {
				BluetoothManager.connectDevice(addressInfo.text)
			}
		}
	}

	ImageButton {
		id: searchButton

		x: 0
		y: 280
		width: 163
		height: 75

		source: {
			if (bluetoothSearch.searching) {
				return "images/setting_bt_search_dis.png"
			} else {
				if (pressed) {
					return "images/setting_bt_search_prsd.png"
				} else {
					return "images/setting_bt_search_nor.png"
				}
			}
		}

		onClicked: {
			if (!bluetoothSearch.searching) {
				bluetoothSearch.searching = true
				BluetoothManager.searchDevice(true)

				codInfo.text = ""
				nameInfo.text = ""
				addressInfo.text = ""
				rssiInfo.text = ""
			}
		}
	}

	ImageButton {
		id: stopButton

		x: 164
		y: 280
		width: 163
		height: 75

		source: {
			if (!bluetoothSearch.searching) {
				return "images/setting_bt_stop_dis.png"
			} else {
				if (pressed) {
					return "images/setting_bt_stop_prsd.png"
				} else {
					return "images/setting_bt_stop_nor.png"
				}
			}
		}

		onClicked: {
			if (bluetoothSearch.searching) {
				bluetoothSearch.searching = false
				BluetoothManager.searchDevice(false)
			}
		}
	}

	ImageButton {
		id: backButton

		x: 0
		y: 380
		width: 163
		height: 75

		source: {
			if (pressed) {
				return "images/setting_bt_back_prsd.png"
			} else {
				return "images/setting_bt_back_nor.png"
			}
		}

		onClicked: {
			bluetoothSearch.backPressed()
		}
	}

	Item {
		id: table

		x: 337
		y: 40
		width: 322
		height: 456

		ListView {
			id: list

			anchors.fill: parent

			spacing: 1

			model: BluetoothManager.searchList

			delegate: Rectangle {
				id: listItem

				width: 322
				height: 57

				color: touch.pressed ? "lightgraay" : "gray"

				Text {
					anchors.fill: parent

					text: name
					color: "white"
					font.pixelSize: 21
					elide: Text.ElideRight
					verticalAlignment: Text.AlignVCenter
				}

				MouseArea {
					id: touch
					anchors.fill: parent

					onClicked: {
						codInfo.text = cod
						nameInfo.text = name
						addressInfo.text = addr
						rssiInfo.text = rssi
					}
				}
			}
		}
	}

	Connections {
		target: BluetoothManager

		onSearchCompleted: {
			bluetoothSearch.searching = false
		}
	}
}
