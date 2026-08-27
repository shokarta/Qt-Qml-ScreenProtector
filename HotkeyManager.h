#pragma once

#include <QObject>
#include <QAbstractNativeEventFilter>
#include <QCoreApplication>
#include <QDebug>

#include <unordered_map>
#include <functional>

#include <windows.h>


class HotkeyManager : public QObject, public QAbstractNativeEventFilter
{
	Q_OBJECT


	public:
		explicit HotkeyManager(QObject *parent = nullptr) : QObject(parent)
		{
			qApp->installNativeEventFilter(this);
		}

		~HotkeyManager() override {
			unregisterAllHotkeys();
			qApp->removeNativeEventFilter(this);
		}

		bool registerHotkey(UINT modifiers, UINT vk, std::function<void()> callback) {
			const int id = m_nextId++;

			if (!RegisterHotKey(nullptr, id, modifiers, vk)) {
qWarning() << "RegisterHotKey failed:" << GetLastError();
				return false;
			}

			m_callbacks[id] = std::move(callback);

			return id;
		}

		void unregisterHotkey(int id) {
			UnregisterHotKey(nullptr, id);
			m_callbacks.erase(id);
		}

		void unregisterAllHotkeys() {
			for (const auto &[id, cb] : m_callbacks) { UnregisterHotKey(nullptr, id); }
			m_callbacks.clear();
		}

		bool nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result) override {
			Q_UNUSED(eventType)
			Q_UNUSED(result)

			MSG *msg = static_cast<MSG*>(message);

			if (msg->message != WM_HOTKEY) { return false; }

			const int id = static_cast<int>(msg->wParam);

			auto it = m_callbacks.find(id);
				if (it == m_callbacks.end()) { return false; }
				it->second();

			return true;
		}


	private:
		int m_nextId = 1;
		std::unordered_map<int, std::function<void()>> m_callbacks;
};