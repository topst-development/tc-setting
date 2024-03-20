/****************************************************************************************
 *   FileName    : BluetoothPaired.qml
 *   Description : BluetoothPaired.qml
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
	id: bluetoothPaired

	property var currAddr: ""
	property var connectedAddr: ""
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
		verticalAlignment: Text.AlignVCenter
		horizontalAlignment: Text.AlignLeft
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
		verticalAlignment: Text.AlignVCenter
		horizontalAlignment: Text.AlignLeft
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

		text: currAddr
		elide: Text.ElideRight
		verticalAlignment: Text.AlignVCenter
		horizontalAlignment: Text.AlignLeft
		color: "white"
		font.pixelSize: 21
	}

	Text {
		id: hfp

		x: 20
		y: 160
		width: 80
		height: 21

		text: "HFP"
		horizontalAlignment: Text.AlignLeft
		color: "gray"
		font.pixelSize: 21
	}

	Text {
		id: a2dp

		x: 110
		y: 160
		width: 80
		height: 21

		text: "A2DP"
		horizontalAlignment: Text.AlignLeft
		color: "gray"
		font.pixelSize: 21
	}

	Text {
		id: avrcp

		x: 200
		y: 160
		width: 80
		height: 21

		text: "AVRCP"
		horizontalAlignment: Text.AlignLeft
		color: "gray"
		font.pixelSize: 21
	}

	Text {
		id: pbap

		x: 20
		y: 190
		width: 80
		height: 21

		text: "PBAP"
		horizontalAlignment: Text.AlignLeft
		color: "gray"
		font.pixelSize: 21
	}

	Text {
		id: map

		x: 110
		y: 190
		width: 80
		height: 21

		text: "MAP"
		horizontalAlignment: Text.AlignLeft
		color: "gray"
		font.pixelSize: 21
	}

	Text {
		id: hidd

		x: 200
		y: 190
		width: 80
		height: 21

		text: "HIDD"
		horizontalAlignment: Text.AlignLeft
		color: "gray"
		font.pixelSize: 21
	}

	Text {
		id: pan

		x: 20
		y: 220
		width: 80
		height: 21

		text: "PAN"
		horizontalAlignment: Text.AlignLeft
		color: "gray"
		font.pixelSize: 21
	}

	ImageButton {
		id: connectButton

		x: 0
		y: 280
		width: 163
		height: 75

		source: {
			if (currAddr != connectedAddr && currAddr != "") {
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
			if (currAddr != connectedAddr && currAddr != "") {
				BluetoothManager.connectDevice(currAddr);
			}
		}
	}

	ImageButton {
		id: disconnectButton

		x: 164
		y: 280
		width: 163
		height: 75

		source: {
			if (currAddr == connectedAddr && currAddr != "") {
				if (pressed) {
					return "images/setting_bt_disconnect_prsd.png"
				} else {
					return "images/setting_bt_disconnect_nor.png"
				}
			} else {
				return "images/setting_bt_disconnect_dis.png"
			}
		}

		onClicked: {
			if (currAddr == connectedAddr && currAddr != "") {
				BluetoothManager.disconnectPairedDevice(currAddr);
			}
		}
	}

	ImageButton {
		id: deleteButton

		x: 0
		y: 380
		width: 163
		height: 75

		source: {
			if (currAddr != "") {
				if (pressed) {
					return "images/setting_bt_delete_prsd.png"
				} else {
					return "images/setting_bt_delete_nor.png"
				}
			} else {
				return "images/setting_bt_delete_dis.png"
			}
		}

		onClicked: {
			if (currAddr != "") {
				BluetoothManager.deletePairedDevice(currAddr);
			}
		}
	}

	ImageButton {
		id: backButton

		x: 164
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
			bluetoothPaired.backPressed()
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

			model: BluetoothManager.pairedList

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
						currAddr = addr

						if (hfpStat == 0) {
							hfp.color = "lightgray"
						} if (hfpStat == 1) {
							hfp.color = "blue"
						} else if (hfpStat == 2) {
							hfp.color = "green"
						} else {
							hfp.color = "lightgray"
						}

						if (a2dpStat == 0) {
							a2dp.color = "lightgray"
						} if (a2dpStat == 1) {
							a2dp.color = "blue"
						} else if (a2dpStat == 2) {
							a2dp.color = "green"
						} else {
							a2dp.color = "lightgray"
						}

						if (avrcpStat == 0) {
							avrcp.color = "lightgray"
						} if (avrcpStat == 1) {
							avrcp.color = "blue"
						} else if (avrcpStat == 2) {
							avrcp.color = "green"
						} else {
							avrcp.color = "lightgray"
						}

						if (pbapStat == 0) {
							pbap.color = "lightgray"
						} if (pbapStat == 1) {
							pbap.color = "blue"
						} else if (pbapStat == 2) {
							pbap.color = "green"
						} else {
							pbap.color = "lightgray"
						}

						if (mapStat == 0) {
							map.color = "lightgray"
						} if (mapStat == 1) {
							map.color = "blue"
						} else if (mapStat == 2) {
							map.color = "green"
						} else {
							map.color = "lightgray"
						}

						if (hiddStat == 0) {
							hidd.color = "lightgray"
						} if (hiddStat == 1) {
							hidd.color = "blue"
						} else if (hiddStat == 2) {
							hidd.color = "green"
						} else {
							hidd.color = "lightgray"
						}

						if (panStat == 0) {
							pan.color = "lightgray"
						} if (panStat == 1) {
							pan.color = "blue"
						} else if (panStat == 2) {
							pan.color = "green"
						} else {
							pan.color = "lightgray"
						}
					}
				}
			}
		}
	}

	Connections {
		target: BluetoothManager

		onConnectedDeviceChanged: {
			console.log("Connected: " + connAddr)
			connectedAddr = connAddr
		}
	}
}
