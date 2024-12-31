#include "WebSocketClient.h"
#include <QCoreApplication>
#include <QDebug>

WebSocketClient::WebSocketClient(const QUrl &url, QObject *parent)
    : QObject(parent), m_url(url)
{
    connect(&m_webSocket, &QWebSocket::connected, this, &WebSocketClient::onConnected);
    connect(&m_webSocket, &QWebSocket::disconnected, this, &WebSocketClient::onDisconnected);
    connect(&m_webSocket, &QWebSocket::textMessageReceived, this, &WebSocketClient::onTextMessageReceived);
}

WebSocketClient::~WebSocketClient()
{
    m_webSocket.close();
}

void WebSocketClient::connectToServer()
{
    m_webSocket.open(m_url);
}

void WebSocketClient::sendMessage(const QByteArray &message)
{
    if (m_webSocket.state() == QAbstractSocket::ConnectedState) {
        m_webSocket.sendBinaryMessage(message);
    } else {
        qWarning() << "WebSocket is not connected!";
    }
}

void WebSocketClient::onConnected()
{
    qDebug() << "Connected to WebSocket server:" << m_url;
    // Example message send
    //sendMessage("data");
}

void WebSocketClient::onDisconnected()
{
    qDebug() << "Disconnected from WebSocket server.";
}

void WebSocketClient::onTextMessageReceived(const QString &message)
{
    qDebug() << "Message received from server:" << message;
    emit messageReceived(message);
}
