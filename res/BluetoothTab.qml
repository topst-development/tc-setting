/****************************************************************************************
 *   FileName    : BluetoothTab.qml
 *   Description : BluetoothTab.qml
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
	id: btMain

	property bool init: false
	property bool on: false
	property var type: ""

	onVisibleChanged: {
		if (this.visible) {
			BluetoothManager.prepareBluetooth()
		}
	}

	Item {
		visible: btMain.type == ""

		Text {
			x: 58
			y: 102
			width: 120
			height: 19

			text: "Local Device"
			horizontalAlignment: Text.AlignLeft
			color: "white"
			font.pixelSize: 21
		}

		Text {
			id: localDevice

			x: 59
			y: 134
			width: 330
			height: 45

			text: "None"
			horizontalAlignment: Text.AlignLeft
			color: "white"
			font.pixelSize: 27
		}

		Text {
			x: 427
			y: 102
			width: 79
			height: 19

			text: "Address"
			horizontalAlignment: Text.AlignLeft
			color: "white"
			font.pixelSize: 21
		}

		Text {
			id: localAddress

			x: 428
			y: 134
			width: 330
			height: 45

			text: "None"
			horizontalAlignment: Text.AlignLeft
			color: "white"
			font.pixelSize: 27
		}

		Text {
			x: 58
			y: 245
			width: 144
			height: 19

			text: "Remote Device"
			horizontalAlignment: Text.AlignLeft
			color: "white"
			font.pixelSize: 21
		}

		Text {
			id: remoteDevice

			x: 59
			y: 277
			width: 330
			height: 45

			text: "None"
			horizontalAlignment: Text.AlignLeft
			color: "white"
			font.pixelSize: 27
		}

		Text {
			x: 427
			y: 245
			width: 79
			height: 19

			text: "Remote Address"
			horizontalAlignment: Text.AlignLeft
			color: "white"
			font.pixelSize: 21
		}

		Text {
			id: remoteAddress

			x: 428
			y: 277
			width: 330
			height: 45

			text: "None"
			horizontalAlignment: Text.AlignLeft
			color: "white"
			font.pixelSize: 27
		}

		ImageButton {
			x: 40
			y: 429
			width: 204
			height: 94

			source: {
				if (pressed) {
					return "images/setting_bt_on_prsd.png"
				} else {
					return "images/setting_bt_on_nor.png"
				}
			}

			onClicked: {
				if (btMain.visible) {
					BluetoothManager.enableBluetooth()
				} else {
					btMain.type = "setting"
				}
			}
		}

		ImageButton {
			x: 284
			y: 429
			width: 204
			height: 94

			source: {
				if (pressed) {
					return "images/setting_bt_paired_prsd.png"
				} else if (btMain.on) {
					return "images/setting_bt_paired_nor.png"
				} else {
					return "images/setting_bt_paired_dis.png"
				}
			}

			onClicked: {
				if (btMain.on) {
					btMain.type = "paired"
				}
			}
		}

		ImageButton {
			x: 528
			y: 429
			width: 204
			height: 94

			source: {
				if (pressed) {
					return "images/setting_bt_available_prsd.png"
				} else if (btMain.on) {
					return "images/setting_bt_available_nor.png"
				} else {
					return "images/setting_bt_available_dis.png"
				}
			}

			onClicked: {
				if (btMain.on) {
					btMain.type = "search"
				}
			}
		}
	}

	BluetoothPaired {
		anchors.fill: parent

		visible: btMain.type == "paired"

		onBackPressed: {
			btMain.type = ""
		}
	}

	BluetoothSearch {
		anchors.fill: parent

		visible: btMain.type == "search"

		onBackPressed: {
			btMain.type = ""
		}
	}

	Connections {
		target: BluetoothManager

		onBluetoothInitChanged: {
			btMain.init = on
		}

		onBluetoothOnChanged: {
			btMain.on = on
		}

		onLocalInfoChanged: {
			localDevice.text = localName
			localAddress.text = localAddr
		}

		onRemoteInfoChanged: {
			remoteDevice.text = remoteName
			remoteAddress.text = remoteAddr
		}
	}
}
