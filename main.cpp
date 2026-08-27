#include <QApplication>
#include <QSystemTrayIcon>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickWindow>
#include <QScreen>

#include "TrayManager.h"
#include "HotkeyManager.h"
#include "OverlayController.h"


QScreen* findLaptopScreen()
{
	UINT32 pathCount = 0;
	UINT32 modeCount = 0;

	if (GetDisplayConfigBufferSizes(QDC_ONLY_ACTIVE_PATHS, &pathCount, &modeCount) != ERROR_SUCCESS) { return nullptr; }

	std::vector<DISPLAYCONFIG_PATH_INFO> paths(pathCount);
	std::vector<DISPLAYCONFIG_MODE_INFO> modes(modeCount);

	if (QueryDisplayConfig(QDC_ONLY_ACTIVE_PATHS, &pathCount, paths.data(), &modeCount, modes.data(), nullptr) != ERROR_SUCCESS) { return nullptr; }

	for (const auto &path : paths) {
		DISPLAYCONFIG_TARGET_DEVICE_NAME target{};
			target.header.type = DISPLAYCONFIG_DEVICE_INFO_GET_TARGET_NAME;
			target.header.size = sizeof(target);
			target.header.adapterId = path.targetInfo.adapterId;
			target.header.id = path.targetInfo.id;

		if (DisplayConfigGetDeviceInfo(&target.header) != ERROR_SUCCESS) { continue; }

		if (target.outputTechnology != DISPLAYCONFIG_OUTPUT_TECHNOLOGY_INTERNAL) { continue; }

		DISPLAYCONFIG_SOURCE_DEVICE_NAME source{};
			source.header.type = DISPLAYCONFIG_DEVICE_INFO_GET_SOURCE_NAME;
			source.header.size = sizeof(source);
			source.header.adapterId = path.sourceInfo.adapterId;
			source.header.id = path.sourceInfo.id;

		if (DisplayConfigGetDeviceInfo(&source.header) != ERROR_SUCCESS) { continue; }

		QString gdiName = QString::fromWCharArray(source.viewGdiDeviceName);

		for (QScreen *screen : QGuiApplication::screens()) {
			if (screen->name().compare(gdiName, Qt::CaseInsensitive) == 0) { return screen; }
		}
	}

	return nullptr;
}


int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
		QApplication::setQuitOnLastWindowClosed(false);


	QQmlApplicationEngine engine;
		engine.loadFromModule(QString(APP_URI), "Main");


	auto *window = qobject_cast<QQuickWindow *>(engine.rootObjects().constFirst());
	QScreen *screen = findLaptopScreen();
	if (window && screen) {
		window->setScreen(screen);
		window->setGeometry(screen->geometry());
	}
	else {
qWarning() << "Laptop screen not found !!!";
	}


	TrayManager trayManager;
		trayManager.show();

	OverlayController overlayManager;
		overlayManager.setWindow(window);
		overlayManager.setScreen(screen);

	HotkeyManager hotkeyManager;
		bool visible = false;
		hotkeyManager.registerHotkey(MOD_ALT,				'Q',		[&overlayManager]() { overlayManager.toggle(); });		// ALT + Q
		hotkeyManager.registerHotkey(MOD_CONTROL,			'Q',		[&overlayManager]() { overlayManager.toggle(); });		// CTRL + Q
		hotkeyManager.registerHotkey(MOD_CONTROL | MOD_ALT,	'Q',		[&overlayManager]() { overlayManager.toggle(); });		// CTRL + ALT + Q
		hotkeyManager.registerHotkey(MOD_ALT,				VK_OEM_3,	[&overlayManager]() { overlayManager.toggle(); });		// ALT + ~
		hotkeyManager.registerHotkey(MOD_CONTROL,			VK_OEM_3,	[&overlayManager]() { overlayManager.toggle(); });		// CTRL + ~
		hotkeyManager.registerHotkey(MOD_CONTROL | MOD_ALT,	VK_OEM_3,	[&overlayManager]() { overlayManager.toggle(); });		// CTRL + ALT + ~


	engine.rootContext()->setContextProperty("overlayController", &overlayManager);


	return app.exec();
}