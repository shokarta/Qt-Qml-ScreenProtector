#pragma once

#include <QObject>
#include <QQuickWindow>
#include <QScreen>

#include <windows.h>


class OverlayController : public QObject
{
    Q_OBJECT
	
    Q_PROPERTY(bool visible READ visible NOTIFY visibleChanged)


	public:
		explicit OverlayController(QObject *parent = nullptr) : QObject(parent)
		{
		}

		bool visible() const { return m_visible; }

		Q_INVOKABLE void toggle()
		{
			m_visible = !m_visible;

			emit visibleChanged();

			if (!m_window) { return; }

			if (m_visible) {
				if (m_screen) {
					m_window->setScreen(m_screen);
					m_window->setGeometry(m_screen->geometry());
				}

				m_window->show();

				HWND hwnd = reinterpret_cast<HWND>(m_window->winId());
				SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
			}
			else { m_window->hide(); }
		}

		void setWindow(QQuickWindow *window) { m_window = window; }
		void setScreen(QScreen *screen) { m_screen = screen; }


	signals:
		void visibleChanged();


	private:
		bool m_visible = false;
		QQuickWindow *m_window = nullptr;
		QScreen *m_screen = nullptr;
};