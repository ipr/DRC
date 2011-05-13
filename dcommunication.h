#ifndef DCOMMUNICATION_H
#define DCOMMUNICATION_H

#include <QObject>
#include <QList>
#include <QMap>
#include <QSet>
#include <QByteArray>

class QAbstractSocket;
class QTcpServer;

class dCommunication : public QObject
{
    Q_OBJECT
	
public:
    explicit dCommunication(QObject *parent = 0);
    virtual ~dCommunication();

signals:
	void message(QByteArray id, QByteArray payload);

public slots:
	void initServer(QList<long> lstPortList);
	void onAcceptPeer(QTcpServer *pServer);
	
	void addPeer(QString szPeer, long lPort);
	/*
	void addSecurePeer(QString szPeer, long lPort);
	*/
	
	void dataReady(QAbstractSocket *pSocket);
	
	void sendMessage(QByteArray idThis, QByteArray &payload);
	
private:
	QList<QAbstractSocket*> m_Connections;
	
	// TODO: can one instance listen more ports?
	//QTcpServer *m_pListener;
	//QList<QTcpServer*> m_Listeners;
	// TODO: map by port
	QMap<long, QTcpServer*> m_Listeners;
	
	QSet<QByteArray> m_MsgHashes;
	
	// receiving buffer until complete?
	QByteArray m_MessageBuffer;

};

#endif // DCOMMUNICATION_H
