/****************************************************************************************
 *   FileName    : main.cpp
 *   Description : 
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

#include <cstdio>
#include <sys/signal.h>
#include <unistd.h>
#include <QQuickItem>
#include <QQuickWindow>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "Setting.h"
#include "AudioManager.h"
#include "ColorAdjustmentManager.h"
#ifdef ENABLE_BT
#include "BluetoothManager.h"
#endif
#ifdef ENABLE_FWUPDATE
#include "FWUpdateManager.h"
#endif
#include "SettingIFManager.h"

static void NewExceptionHandler();
static void SignalHandler(int sig);
static void PrintQtMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg);

int g_debug = 0;
int g_appID = -1;
bool g_rear = false;

Setting *_setting;

int main(int argc, char *argv[])
{
	int ret = 0;

	qInstallMessageHandler(PrintQtMessage);
	QGuiApplication app(argc, argv);
	QQmlApplicationEngine engine;

	signal(SIGTERM, SignalHandler);
	signal(SIGCHLD, SIG_IGN);

	QString argument;

	for (int i = 1; i < argc; i++)
	{
		argument = argv[i];
		argument = argument.toUpper();
		if (argument == "--DEBUG")
		{
			g_debug = 1;
		}
		else if (argument.left(5) == "--ID=")
		{
			g_appID = argument.mid(5).toInt();
		}
		else if (argument == "--REAR")
		{
			g_rear = true;
		}
	}

	std::set_new_handler(NewExceptionHandler);

	Setting *_setting = new Setting();
	AudioManager *_audioManager = new AudioManager();
	ColorAdjustmentManager *_caManager = new ColorAdjustmentManager();
#ifdef ENABLE_BT
	BluetoothManager *_bluetoothManager = new BluetoothManager();
#endif
#ifdef ENABLE_FWUPDATE
	FWUpdate *_fwUpdateManager = new FWUpdate();
#endif

	engine.rootContext()->setContextProperty("Setting", _setting);
	engine.rootContext()->setContextProperty("AudioManager", _audioManager);
	engine.rootContext()->setContextProperty("ColorAdjustmentManager", _caManager);
#ifdef ENABLE_BT
	engine.rootContext()->setContextProperty("BluetoothManager", _bluetoothManager);
#endif
#ifdef ENABLE_FWUPDATE
	engine.rootContext()->setContextProperty("FWUpdateManager", _fwUpdateManager);
#endif
	engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

	if (_setting != NULL)
	{
		ret = SETTING_IF_MANAGER->Initialize();
		_setting->Initialize(ret);
		_audioManager->Initialize();
		_setting->AddMenu(AUDIO);
		_caManager->Initialize();
		_setting->AddMenu(COLORADJUSTMENT);
#ifdef ENABLE_BT
		_bluetoothManager->Initialize();
		_setting->AddMenu(BLUETOOTH);
#endif
#ifdef ENABLE_FWUPDATE
		_fwUpdateManager->Initialize();
		_setting->AddMenu(FWUPDATE);
#endif
		_setting->CompleteMenu();

		fprintf(stderr, "SETTING PROCESS STARTED.\n");
		ret = app.exec();

		if (_setting != NULL)
		{
			delete _setting;
			_setting = NULL;
		}
		SETTING_IF_MANAGER->Release();
	}
	else
	{
		fprintf(stderr, "%s: create Setting process failed\n", __FUNCTION__);
	}

	return ret;
}

static void NewExceptionHandler()
{
	fprintf(stderr, "[SETTING] FATAL ERROR : MEMORY ALLOCATION FAILED\n");
	throw std::bad_alloc();
}

static void SignalHandler(int sig)
{
	fprintf(stderr, "[SETTING] %s: received signal(%d)\n", __FUNCTION__, sig);
	QGuiApplication::exit(sig);
}

static void PrintQtMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
	QByteArray localMsg = msg.toLocal8Bit();
	switch ((int)type)
	{
		case QtDebugMsg:
			fprintf(stderr, "QtDebug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
			break;
		case QtWarningMsg:
			fprintf(stderr, "QtWarning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
			break;
		case QtCriticalMsg:
			fprintf(stderr, "QtCritical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
			break;
		case QtFatalMsg:
			fprintf(stderr, "QtFatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
			abort();
	}
}
