#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "dcommunication.h"

#include <QDateTime>
#include <QAbstractItemModel>
#include <QStandardItemModel>
#include <QModelIndex>

#include "dabstracthash.h"
//#include "dabstractsign.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
	m_idThis(),
	m_pComm(nullptr),
	m_pModel(nullptr),
	m_pHashing(nullptr)
	//m_pSigning(nullptr)
{
    ui->setupUi(this);
	m_pModel = new QStandardItemModel(this);
	
	// use sha1 by default
	m_pHashing = new dSha1Hash(this);
	
	// no signing, just "as-is"
	//m_pSigning = new dDummySign(this);

	m_pComm = new dCommunication(this);
	connect(m_pComm, SIGNAL(message(QByteArray, QByteArray)), this, SLOT(onMessage(QByteArray, QByteArray)));

	// temp
	on_actionInit_triggered();
}

MainWindow::~MainWindow()
{
	delete m_pComm;
    delete ui;
}

void MainWindow::on_actionInit_triggered()
{
	// TODO: get user nickname or other to add to hash?
	// TODO: other info to create ID of this?

	QByteArray id;
	
	QDateTime now = QDateTime::currentDateTimeUtc();
	id.append(now.toString());
	
	// TODO: get user nickname or other to add to hash?
	//id.append(szNickname);

	
	// only hashed id will be used in comms
	m_idThis = m_pHashing->getHashed(id);
}


// TODO: list of server-ports
void MainWindow::on_actionAddServer_triggered()
{
	// TODO: get list of ports from user?
    //m_pComm->initServer();
	QList<long> lstPorts;
	lstPorts << 1234;
	m_pComm->initServer(lstPorts);
}

// TODO: list of peers
void MainWindow::on_actionAddPeer_triggered()
{
	// TODO: get address/name + port list from user
    //m_pComm->addPeer();
	
	m_pComm->addPeer("127.0.0.1", 1234);
}

void MainWindow::onMessage(QByteArray idPeer, QByteArray payload)
{
	// show message
	QString szMsg;
	/*
	QString szHash = m_pHashing->getHashed(szMsg);
	
	// check by message hash and sender hash if same message is already shown

	// parse timestamp from message
	int iStamp = szMsg.indexOf(";");
	QString szStamp = szMsg.left(iStamp);
	szMsg = szMsg.right(szMsg.length() - iStamp);
	*/

	/*
	int iIndex = payload.indexOf(";");
	if (iIndex > 0 && iIndex <= 5
		&& m_pSigning != nullptr) // 5 char signing type at most
	{
		// handle message
		// "PGP"
		//if (msgSign == m_pSigning->getType())
		//szMsg = m_pSigning->getUnsigned(payload.left(iIndex), payload.right(payload.length() - iIndex));
	}
	else
	{
		// otherwise just show plaintext
		szMsg = payload;
	}
	*/

	// otherwise just show plaintext
	szMsg = payload;
	
	QString szToShow;
	HexEncode(idPeer.constData(), idPeer.length(), szToShow);
	szToShow.append(":").append(szMsg);
	
	ui->msgList->appendPlainText(szToShow);
	
	//m_pModel->insertRow(m_pModel->rowCount());
		//	(QModelIndex(
	
	//m_pModel->data(0, id);
	//m_pModel->data(1, szstamp);
	//m_pModel->data(2, szmsg);
	//this->model
	//ui->msgView
	
}

void MainWindow::on_actionSend_triggered()
{
	// add hash of this peer ID
	// send

	QByteArray payload;
	QString szMsg = ui->msgEdit->toPlainText();
	//QString szStamp = QDateTime::currentDateTimeUtc().toString();

	/*	
	// is PGP-signing enabled?
	if (m_pSigning != nullptr)
	{
		payload.append(m_pSigning->getType());
		payload.append(";");
		payload.append(szStamp);
		payload.append(";");
		
		m_pSigning->getSigned(szMsg, payload);
		payload.append(";");
	}
	else
	{
		// otherwise just message
		payload.append(";");
		payload.append(szMsg);
		payload.append(";");
	}
	*/
	
	payload.append(szMsg);
		
	// no other identification, just hash
	m_pComm->sendMessage(m_idThis, payload);

	// show sent
	QString szToShow;
	HexEncode(m_idThis.data(), m_idThis.length(), szToShow);
	szToShow.append(":").append(szMsg);
	
	ui->msgList->appendPlainText(szToShow);
}


