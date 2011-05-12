#ifndef DSOCKETS_H
#define DSOCKETS_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>

class dPeerSocket : public QTcpSocket
{
	Q_OBJECT

public:
	explicit dPeerSocket(QObject *parent = 0)
		: QTcpSocket(parent)
	{
		connect(this, SIGNAL(connected()), this, SLOT(onConnected()));
		connect(this, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
	}
	explicit dPeerSocket(QTcpSocket *other, QObject *parent = 0)
		: QTcpSocket(parent)
	{
		setSocketDescriptor(other->socketDescriptor(), other->state());
		
		connect(this, SIGNAL(connected()), this, SLOT(onConnected()));
		connect(this, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
	}
    virtual ~dPeerSocket()
	{}
	
signals:
    void connectedPeer(QAbstractSocket*);
	void readyReadPeer(QAbstractSocket*);
	
private slots:
	
	// re-implement for multiple connections
	void onConnected()
	{
		emit connectedPeer(this);
	}
	void onReadyRead()
	{
		emit readyReadPeer(this);
	}
};


class dServerSocket : public QTcpServer
{
	Q_OBJECT
	
public:
	dServerSocket(QObject *parent = 0)
		: QTcpServer(parent)
	{
		connect(this, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
	}
    virtual ~dServerSocket()
	{}
	
signals:
    void newPeerConnection(QTcpServer*);
	
private slots:
	
	// re-implement for multiple connections
	void onNewConnection()
	{
		emit newPeerConnection(this);
	}
};


#endif // DSOCKETS_H
