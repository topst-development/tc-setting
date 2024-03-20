/****************************************************************************************
 *   FileName    : AudioTab.qml
 *   Description : AudioTab.qml
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
	Item {
		id: media

		Text {
			x: 183
			y: 58

			color: "white"
			text: "Media"

			font.pixelSize: 18
		}

		Row {
			x: 79
			y: 91
			width: 267
			height: 60

			spacing: 10

			ImageButton {
				id: mediaMinus

				width: 60
				height: 60

				source: pressed ? "images/setting_control_02_p.png" : "images/setting_control_02_n.png"

				onClicked: {
					console.log("Media Down")

					AudioManager.decreaseVolume(100)
				}
			}

			Image {
				id: mediaVolume

				y: 12

				width: 127
				height: 36

				source: "images/setting_volume_00_img.png"
			}

			ImageButton {
				id: mediaUp

				width: 60
				height: 60

				source: pressed ? "images/setting_control_01_p.png" : "images/setting_control_01_n.png"

				onClicked: {
					console.log("Media up")

					AudioManager.increaseVolume(100)
				}
			}
		}
	}

	Item {
		id: navi

		Text {
			x: 533
			y: 58

			color: "white"
			text: "Navi"

			font.pixelSize: 18
		}

		Row {
			x: 429
			y: 91
			width: 267
			height: 60

			spacing: 10

			ImageButton {
				id: naviMinus

				width: 60
				height: 60

				source: pressed ? "images/setting_control_02_p.png" : "images/setting_control_02_n.png"

				onClicked: {
					console.log("Navi down")

					AudioManager.decreaseVolume(101)
				}
			}

			Image {
				id: naviVolume

				y: 12

				width: 127
				height: 36

				source: "images/setting_volume_00_img.png"
			}

			ImageButton {
				id: naviUp

				width: 60
				height: 60

				source: pressed ? "images/setting_control_01_p.png" : "images/setting_control_01_n.png"

				onClicked: {
					console.log("Navi up")

					AudioManager.increaseVolume(101)
				}
			}
		}
	}

	Item {
		id: text

		Text {
			x: 183
			y: 225

			color: "white"
			text: "Text"

			font.pixelSize: 18
		}

		Row {
			x: 79
			y: 258
			width: 267
			height: 60

			spacing: 10

			ImageButton {
				id: textMinus

				width: 60
				height: 60

				source: pressed ? "images/setting_control_02_p.png" : "images/setting_control_02_n.png"

				onClicked: {
					console.log("Text down")

					AudioManager.decreaseVolume(102)
				}
			}

			Image {
				id: textVolume

				y: 12

				width: 127
				height: 36

				source: "images/setting_volume_00_img.png"
			}

			ImageButton {
				id: textPlus

				width: 60
				height: 60

				source: pressed ? "images/setting_control_01_p.png" : "images/setting_control_01_n.png"

				onClicked: {
					console.log("Text up")

					AudioManager.increaseVolume(102)
				}
			}
		}
	}

	Item {
		id: call

		Text {
			x: 533
			y: 225

			color: "white"
			text: "Call"

			font.pixelSize: 18
		}

		Row {
			x: 429
			y: 258
			width: 267
			height: 60

			spacing: 10

			ImageButton {
				id: callMinus

				width: 60
				height: 60

				source: pressed ? "images/setting_control_02_p.png" : "images/setting_control_02_n.png"

				onClicked: {
					console.log("Call down")

					AudioManager.decreaseVolume(103)
				}
			}

			Image {
				id: callVolume

				y: 12

				width: 127
				height: 36

				source: "images/setting_volume_00_img.png"
			}

			ImageButton {
				id: callPlus

				width: 60
				height: 60

				source: pressed ? "images/setting_control_01_p.png" : "images/setting_control_01_n.png"

				onClicked: {
					console.log("Call up")

					AudioManager.increaseVolume(103)
				}
			}
		}
	}

	Item {
		id: alert

		Text {
			x: 183
			y: 391

			color: "white"
			text: "Alert"

			font.pixelSize: 18
		}

		Row {
			x: 79
			y: 424
			width: 267
			height: 60

			spacing: 10

			ImageButton {
				id: alertMinus

				width: 60
				height: 60

				source: pressed ? "images/setting_control_02_p.png" : "images/setting_control_02_n.png"

				onClicked: {
					console.log("Alert down")

					AudioManager.decreaseVolume(104)
				}
			}

			Image {
				id: alertVolume

				y: 12

				width: 127
				height: 36

				source: "images/setting_volume_00_img.png"
			}

			ImageButton {
				id: alertPlus

				width: 60
				height: 60

				source: pressed ? "images/setting_control_01_p.png" : "images/setting_control_01_n.png"

				onClicked: {
					console.log("Alert up")

					AudioManager.increaseVolume(104)
				}
			}
		}
	}

	Connections {
		target: AudioManager

		onChangedVolume: {
			var tmp = "10"

			if (volume != 10) {
				tmp = "0" + volume
			}

			console.log(sourceID + ": " + tmp)

			if (sourceID == 100) {
				mediaVolume.source = "images/setting_volume_" + tmp + "_img.png"
			} else if (sourceID == 101) {
				naviVolume.source = "images/setting_volume_" + tmp + "_img.png"
			} else if (sourceID == 102) {
				textVolume.source = "images/setting_volume_" + tmp + "_img.png"
			} else if (sourceID == 103) {
				callVolume.source = "images/setting_volume_" + tmp + "_img.png"
			} else if (sourceID == 104) {
				alertVolume.source = "images/setting_volume_" + tmp + "_img.png"
			}
		}
	}
}
