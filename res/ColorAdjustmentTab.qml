/****************************************************************************************
 *   FileName    : ColorAdjustmentTab.qml
 *   Description : ColorAdjustmentTab.qml
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
	Text {
		x: 124
		y: 58
		width: 162
		height: 19

		text: "Temperature"
		color: "white"
		font.pixelSize: 21
		horizontalAlignment: Text.AlignHCenter
		verticalAlignment: Text.AlignVCenter
	}

	Row {
		x: 79
		y: 91
		width: 267
		height: 60

		spacing: 10

		ImageButton {
			width: 60
			height: 60

			source: pressed ? "images/setting_control_02_p.png" : "images/setting_control_02_n.png"

			onClicked: {
				console.log("Temperature Down")
				ColorAdjustmentManager.downTemperature()
			}
		}

		Image {
			id: temperatureValue

			y: 12

			width: 127
			height: 36

			source: "images/setting_volume_01_img.png"
		}

		ImageButton {
			width: 60
			height: 60

			source: pressed ? "images/setting_control_01_p.png" : "images/setting_control_01_n.png"

			onClicked: {
				console.log("Temperature up")
				ColorAdjustmentManager.upTemperature()
			}
		}
	}

	Text {
		id: hueText
		x: 124
		y: 305
		width: 162
		height: 19

		text: "Hue"
		color: "white"
		font.pixelSize: 21
		horizontalAlignment: Text.AlignHCenter
		verticalAlignment: Text.AlignVCenter
	}

	Row {
		x: 79
		y: 348
		width: 267
		height: 60

		spacing: 10

		ImageButton {
			id: hueDown
			width: 60
			height: 60

			source: pressed ? "images/setting_control_02_p.png" : "images/setting_control_02_n.png"

			onClicked: {
				console.log("Hue Down")
				ColorAdjustmentManager.downHue()
			}
		}

		Image {
			id: hueValue

			y: 12

			width: 127
			height: 36

			source: "images/setting_volume_05_img.png"
		}

		ImageButton {
			id: hueUp
			width: 60
			height: 60

			source: pressed ? "images/setting_control_01_p.png" : "images/setting_control_01_n.png"

			onClicked: {
				console.log("Hue up")
				ColorAdjustmentManager.upHue()
			}
		}
	}

	Text {
		id: saturationText
		x: 488
		y: 305
		width: 162
		height: 19

		text: "Saturation"
		color: "white"
		font.pixelSize: 21
		horizontalAlignment: Text.AlignHCenter
		verticalAlignment: Text.AlignVCenter
	}

	Row {
		x: 429
		y: 348
		width: 267
		height: 60

		spacing: 10

		ImageButton {
			id: saturationDown
			width: 60
			height: 60

			source: pressed ? "images/setting_control_02_p.png" : "images/setting_control_02_n.png"

			onClicked: {
				console.log("Saturation Down")
				ColorAdjustmentManager.downSaturation()
			}
		}

		Image {
			id: saturationValue

			y: 12

			width: 127
			height: 36

			source: "images/setting_volume_00_img.png"
		}

		ImageButton {
			id: saturationUp
			width: 60
			height: 60

			source: pressed ? "images/setting_control_01_p.png" : "images/setting_control_01_n.png"

			onClicked: {
				console.log("Hue up")
				ColorAdjustmentManager.upSaturation()
			}
		}
	}

	ImageButton {
		id: disp0

		x: 62
		y: 195
		width: 90
		height: 35

		source: "images/setting_ca_disp0_on.png"

		onClicked: {
			ColorAdjustmentManager.clickTempLUTDISP0()
		}
	}

	ImageButton {
		id: disp1

		x: 180
		y: 195
		width: 90
		height: 35

		source: "images/setting_ca_disp1_off.png"

		onClicked: {
			ColorAdjustmentManager.clickTempLUTDISP1()
		}
	}

	ImageButton {
		id: disp2

		x: 298
		y: 195
		width: 90
		height: 35

		source: "images/setting_ca_disp2_off.png"

		onClicked: {
			ColorAdjustmentManager.clickTempLUTDISP2()
		}
	}

	ImageButton {
		id: tempVideo

		x: 416
		y: 195
		width: 90
		height: 35

		source: "images/setting_ca_video_off.png"

		onClicked: {
			ColorAdjustmentManager.clickTempLUTVIDEO()
		}
	}

	ImageButton {
		id: tempOnOff

		x: 562
		y: 195
		width: 90
		height: 35

		source: "images/setting_ca_off_off.png"

		onClicked: {
			ColorAdjustmentManager.clickTempLUTOnOff()
		}
	}

	ImageButton {
		id: hsvDisp0

		x: 62
		y: 455
		width: 90
		height: 35

		source: "images/setting_ca_disp0_on.png"

		onClicked: {
			ColorAdjustmentManager.clickHSV3DLUTDISP0()
		}
	}

	ImageButton {
		id: hsvDisp1

		x: 180
		y: 455
		width: 90
		height: 35

		source: "images/setting_ca_disp1_off.png"

		onClicked: {
			ColorAdjustmentManager.clickHSV3DLUTDISP1()
		}
	}

	ImageButton {
		id: hsvOnOff

		x: 562
		y: 455
		width: 90
		height: 35

		source: "images/setting_ca_off_off.png"

		onClicked: {
			ColorAdjustmentManager.clickHSV3DLUTOnOff()
		}
	}

	Connections {
		target: ColorAdjustmentManager

		onHueSaturationDisabled: {
			hueText.visible = false
			hueDown.visible = false
			hueValue.visible = false
			hueUp.visible = false
			saturationText.visible = false
			saturationDown.visible = false
			saturationValue.visible = false
			saturationUp.visible = false
			hsvDisp0.visible = false
			hsvDisp1.visible = false
			hsvOnOff.visible = false
		}

		onTemperatureChanged: {
			var tmp = "10"

			if (value != 10) {
				tmp = "0" + value
			}

			console.log("Temperature: " + tmp)
			temperatureValue.source = "images/setting_volume_" + tmp + "_img.png"
		}

		onHueChanged: {
			var tmp = "10"

			if (value != 10) {
				tmp = "0" + value
			}

			console.log("Hue: " + tmp)
			hueValue.source = "images/setting_volume_" + tmp + "_img.png"
		}

		onSaturationChanged: {
			var tmp = "10"

			if (value != 10) {
				tmp = "0" + value
			}

			console.log("Saturation: " + tmp)
			saturationValue.source = "images/setting_volume_" + tmp + "_img.png"
		}

		onTempDisp0Changed: {
			console.log("temp disp 0")
			disp0.source = "images/setting_ca_disp0_on.png"
			disp1.source = "images/setting_ca_disp1_off.png"
			disp2.source = "images/setting_ca_disp2_off.png"
			tempVideo.source = "images/setting_ca_video_off.png"
		}

		onTempDisp1Changed: {
			console.log("temp disp 1")
			disp0.source = "images/setting_ca_disp0_off.png"
			disp1.source = "images/setting_ca_disp1_on.png"
			disp2.source = "images/setting_ca_disp2_off.png"
			tempVideo.source = "images/setting_ca_video_off.png"
		}

		onTempDisp2Changed: {
			console.log("temp disp 2")
			disp0.source = "images/setting_ca_disp0_off.png"
			disp1.source = "images/setting_ca_disp1_off.png"
			disp2.source = "images/setting_ca_disp2_on.png"
			tempVideo.source = "images/setting_ca_video_off.png"
		}

		onTempVideoChanged: {
			console.log("temp video")
			disp0.source = "images/setting_ca_disp0_off.png"
			disp1.source = "images/setting_ca_disp1_off.png"
			disp2.source = "images/setting_ca_disp2_off.png"
			tempVideo.source = "images/setting_ca_video_on.png"
		}

		onTempOnChanged: {
			console.log("temp on")
			tempOnOff.source = "images/setting_ca_on_on.png"
		}

		onTempOffChanged: {
			console.log("temp off")
			tempOnOff.source = "images/setting_ca_off_on.png"
		}

		onHsvDisp0Changed: {
			console.log("hsv disp 0")
			hsvDisp0.source = "images/setting_ca_disp0_on.png"
			hsvDisp1.source = "images/setting_ca_disp1_off.png"
		}

		onHsvDisp1Changed: {
			console.log("hsv disp 1")
			hsvDisp0.source = "images/setting_ca_disp0_off.png"
			hsvDisp1.source = "images/setting_ca_disp1_on.png"
		}

		onHsvOnChanged: {
			console.log("hsv on")
			hsvOnOff.source = "images/setting_ca_on_on.png"
		}

		onHsvOffChanged: {
			console.log("hsv off")
			hsvOnOff.source = "images/setting_ca_off_on.png"
		}
	}
}
