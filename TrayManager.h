#pragma once

#include <QObject>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QApplication>
#include <QStyle>


class TrayManager : public QObject
{
	Q_OBJECT


	public:
		explicit TrayManager(QObject *parent = nullptr) : QObject(parent)
		{
			m_tray.setIcon(QIcon(QString(APP_URI) + "/resources/app.ico"));
			m_tray.setToolTip("Screen Protector");

			m_titleAction = m_menu.addAction("Screen Protector");																	m_titleAction->setEnabled(false);
			m_menu.addSeparator();
			m_exitAction = m_menu.addAction(QApplication::style()->standardIcon(QStyle::SP_TitleBarCloseButton), "Exit");			m_exitAction->setEnabled(true);

			connect(m_exitAction, &QAction::triggered, qApp, &QCoreApplication::quit);

			m_tray.setContextMenu(&m_menu);
		}

		void show() { m_tray.show(); }

		QSystemTrayIcon *trayIcon() { return &m_tray; }


	private:
		QSystemTrayIcon m_tray;
		QMenu m_menu;
		QAction *m_titleAction {};
		QAction *m_exitAction {};
};