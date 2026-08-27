#pragma once

#include <QObject>


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
			m_visible = !m_visible;
			emit visibleChanged();
		}


	signals:
		void visibleChanged();


	private:
		bool m_visible = false;
};