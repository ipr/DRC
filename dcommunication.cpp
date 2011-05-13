#include "dcommunication.h"

#include <QAbstractSocket>
//#include <QTcpSocket>
//#include <QTcpServer>
//#include <QSslSocket>

#include "dsockets.h"
#include "dabstracthash.h"

dCommunication::dCommunication(QObject *parent) :
    QObject(parent),
	m_Connections(),
	m_Listeners(),
	m_MsgHashes()
{
}

dCommunication::~dCommunication()
{
	auto itl = m_Listeners.begin();
	while (itl != m_Listeners.end())
	{
		QTcpServer *pServer = itl.value();
		pServer->close();
		delete pServer;
		++itl;
	}
	m_Listeners.clear();
	
	auto itp = m_Connections.begin();
	while (itp != m_Connections.end())
	{
		QAbstractSocket *pSocket = (*itp);
		pSocket->close();
		delete pSocket;
		++itp;
	}
	m_Connections.clear();
}

void dCommunication::initServer(QList<long> lstPortList)
{
	foreach (long lPort, lstPortList)
	{
		// only listener for port if doesn't exist yet
		QTcpServer *pExisting = m_Listeners.value(lPort, nullptr);
		if (pExisting == nullptr)
		{
			// any address, any port? list?
			dServerSocket *pServer = new dServerSocket(this);
			pServer->listen(QHostAddress::Any, lPort);
			
			// connect signal to accept connections..
			connect(pServer, SIGNAL(newPeerConnection(QTcpServer*)), this, SLOT(onAcceptPeer(QTcpServer*)));
			
			m_Listeners.insert(lPort, pServer);
		}
	}
}

// connection from server socket
void dCommunication::onAcceptPeer(QTcpServer *pServer)
{
	dPeerSocket *pPeer = new dPeerSocket(pServer->nextPendingConnection(), this);
	connect(pPeer, SIGNAL(readyReadPeer(QAbstractSocket*)), this, SLOT(dataReady(QAbstractSocket*)));

	// send current messages?
	// send other nodes?
	m_Connections.push_back(pPeer);
}

void dCommunication::addPeer(QString szPeer, long lPort)
{
	// create normal connection
	dPeerSocket *pPeer = new dPeerSocket(this);
	pPeer->connectToHost(szPeer, lPort);
	
	// TODO: can we get object pointer to the signal somehow?
	// need to inherit socket and add other signal?
	connect(pPeer, SIGNAL(connectedPeer(QAbstractSocket*)), this, SLOT(onConnectPeer(QAbstractSocket*)));
	connect(pPeer, SIGNAL(readyReadPeer(QAbstractSocket*)), this, SLOT(dataReady(QAbstractSocket*)));
	
	m_Connections.push_back(pPeer);
}

/*
void dCommunication::addSecurePeer(QString szPeer, long lPort)
{
	// create ssl connection
}
*/

void dCommunication::dataReady(QAbstractSocket *pSocket)
{
	// data ready on socket for receiving:
	// - get data
	// - send to other peers

	dSha1Hash sha1hash;
	
	// TODO: message size in protocol so we know to concatenate packets until ready..
	// 
	// receive
	QByteArray data = pSocket->readAll();
	QByteArray hash = sha1hash.getHashed(data);
	
	// TODO: check hash if this was transmitted already
	// to reduce infinite retransmissions..
	if (m_MsgHashes.contains(hash))
	{
		// already sent
		return;
	}
	// otherwise add hash
	m_MsgHashes.insert(hash);
	
	// relay to all other peers
	auto it = m_Connections.begin();
	while (it != m_Connections.end())
	{
		QAbstractSocket *pOther = (*it);
		if (pOther != pSocket)
		{
			pOther->write(data);
		}
		++it;
	}

	// if not completed, add to buffer?
	//m_MessageBuffer.append(data);

	// -> pass to user also, separate hash of peer and payload
	//
	int iIndex = data.indexOf(";");
	if (iIndex == -1)
	{
		emit message(QByteArray(), data.data());
	}
	else
	{
		emit message(data.left(iIndex), data.right(data.length() - iIndex));
	}
}

void dCommunication::sendMessage(QByteArray idThis, QByteArray &payload)
{
	// create message,
	// combine hash of this ID and message data
	
	QByteArray data;
	data.append(idThis);
	data.append(";");
	data.append(payload);
	
	// send to all peers
	auto it = m_Connections.begin();
	while (it != m_Connections.end())
	{
		QAbstractSocket *pSocket = (*it);
		pSocket->write(data);
		++it;
	}
}

