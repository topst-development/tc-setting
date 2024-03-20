/****************************************************************************************
 *   FileName    : main.qml
 *   Description : main.qml
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
import QtQuick.Window 2.2
import "types"

Window {
	id: settingWin

	x: (Screen.width - 1024) / 2
	y: (Screen.height - 600) / 2 + 65
	width: 1024
	height: 535

	flags: Qt.FramelessWindowHint
	title: qsTr("Telechips Setting")

	visible: false
	color: "transparent"

	property var selected: "vol"

	Image {
		id: background

		anchors.fill: parent
		source: "/images/setting_bg.png"

		Flickable {
			id: tab

			x: 0
			y: 0
			width: 249

			Column {
				anchors.fill: parent

				Repeater {
					anchors.fill: parent
					model: Setting.menu

					delegate: ImageButton {
						width: parent.width
						height: 107

						onClicked: {
							console.log("Tab " + index + " Selected")
							settingWin.selected = modelData
						}

						source: {
							if (this.pressed) {
								return "/images/setting_bt_" + modelData + "_prsd.png"
							} else if (settingWin.selected == modelData) {
								return "/images/setting_bt_" + modelData + "_foc.png"
							} else {
								return "/images/setting_bt_" + modelData + "_nor.png"
							}
						}
					}
				}
			}
		}

		Item {
			anchors.left: tab.right
			anchors.top: parent.top
			anchors.bottom: parent.bottom

			AudioTab {
				id: audioTab

				anchors.fill: parent

				visible: settingWin.selected == "vol"
			}

			ColorAdjustmentTab {
				id: caTab

				anchors.fill: parent

				visible: settingWin.selected == "ca"
			}

			BluetoothTab {
				id: bluetoothTab

				anchors.fill: parent

				visible: settingWin.selected == "bluetooth"
			}

			FWUpdateTab {
				id: fwUpdateTab

				anchors.fill: parent

				visible: settingWin.selected == "fwup"
			}
		}
	}

	Connections {
		target: Setting

		onShowHideChanged: {
			console.log("Visible is changed: " + show);
			settingWin.visible = show
		}

		onHomePressed: {
			if (settingWin.visible) {
				Setting.goHome()
			}
		}

		onBackPressed: {
			if (settingWin.visible) {
				Setting.goBack()
			}
		}

		onSettingPressed: {
			if (!settingWin.visible) {
				Setting.goSetting()
			}
		}
	}
}
