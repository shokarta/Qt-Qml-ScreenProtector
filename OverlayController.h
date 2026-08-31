#pragma once

#include <QObject>
#include <QQuickWindow>
#include <QScreen>

#include <functional>
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

		Q_INVOKABLE void toggle() {

			if (!m_window) { return; }

			if (!m_visible) {

				QScreen *screen = nullptr;

				if (m_screenProvider) { screen = m_screenProvider(); }

				if (!screen) {
qWarning() << "Laptop screen does not exist, refusing to show overlay";
					return;
				}

				m_window->setScreen(screen);
				m_window->setGeometry(screen->geometry());
				m_window->show();

				HWND hwnd = reinterpret_cast<HWND>(m_window->winId());
				SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW | SWP_NOACTIVATE);

				m_visible = true;
			}
			else {
				m_window->hide();
				m_visible = false;
			}

			emit visibleChanged();
		}

		void hide() {
			if (!m_visible) { return; }
			if (m_window) { m_window->hide(); }

			m_visible = false;

			emit visibleChanged();
		}

		void setWindow(QQuickWindow *window) { m_window = window; }

		void setScreenProvider(std::function<QScreen*()> provider) { m_screenProvider = std::move(provider); }


	signals:
		void visibleChanged();


	private:
		bool m_visible = false;
		QQuickWindow *m_window = nullptr;
		std::function<QScreen*()> m_screenProvider;
};