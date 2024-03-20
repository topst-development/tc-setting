/****************************************************************************************
 *   FileName    : FWUpdateTab.qml
 *   Description : FWUpdateTab.qml
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
	onVisibleChanged: {
		if (this.visible) {
			FWUpdateManager.showUpdateMenu()
		}
	}

	ImageButton {
		id: allUpdate

		x: 40
		y: 325
		width: 204
		height: 94

		source: {
			if (FWUpdateManager.allUpdate) {
				return pressed ? "images/setting_fwup_all_prsd.png" : "images/setting_fwup_all_nor.png"
			} else {
				return "images/setting_fwup_all_dis.png"
			}
		}

		onClicked: {
			FWUpdateManager.updateAllFW()
		}
	}

	ImageButton {
		id: mainUpdate

		x: 40
		y: 429
		width: 204
		height: 94

		visible: FWUpdateManager.coreSpecUpdate

		source: {
			if (FWUpdateManager.mainUpdate) {
				return pressed ? "images/setting_fwup_main_prsd.png" : "images/setting_fwup_main_nor.png"
			} else {
				return "images/setting_fwup_main_dis.png"
			}
		}

		onClicked: {
			FWUpdateManager.updateMainFW()
		}
	}

	ImageButton {
		id: subUpdate

		x: 284
		y: 429
		width: 204
		height: 94
		visible: FWUpdateManager.coreSpecUpdate

		source: {
			if (FWUpdateManager.subUpdate) {
				return pressed ? "images/setting_fwup_sub_prsd.png" : "images/setting_fwup_sub_nor.png"
			} else {
				return "images/setting_fwup_sub_dis.png"
			}
		}

		onClicked: {
			FWUpdateManager.updateSubFW()
		}
	}

	ImageButton {
		id: snorUpdate

		x: 528
		y: 429
		width: 204
		height: 94
		visible: FWUpdateManager.coreSpecUpdate

		source: {
			if (FWUpdateManager.snorUpdate) {
				return pressed ? "images/setting_fwup_snor_prsd.png" : "images/setting_fwup_snor_nor.png"
			} else {
				return "images/setting_fwup_snor_dis.png"
			}
		}

		onClicked: {
			FWUpdateManager.updateSnorFW()
		}
	}

	Text {
		x: 40
		y: 58
		width: 695
		height: 38

		color: "white"
		text: FWUpdateManager.notifyLog
		horizontalAlignment: Text.AlignLeft
		verticalAlignment: Text.AlignTop
		font.pixelSize: 24
	}

	Text {
		x: 40
		y: 136
		width: 695
		height: 38

		color: "white"
		text: FWUpdateManager.resultLog
		horizontalAlignment: Text.AlignLeft
		verticalAlignment: Text.AlignTop
		font.pixelSize: 24
	}
}
