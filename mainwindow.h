#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class dCommunication;
class QAbstractItemModel;

class dAbstractHash;
//class dAbstractSign;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
	
	// hash of this peer ID
	// ("who is sending")
	QByteArray m_idThis;
	
	dCommunication *m_pComm;
	
	// TODO: view-model for message display
	QAbstractItemModel *m_pModel;
	
	dAbstractHash *m_pHashing; // id hashing
	//dAbstractSign *m_pSigning; // message signing

	inline void HexEncode(const char *pBuffer, const size_t nLen, QString &szOutput) const
	{
		if (pBuffer == NULL
			|| pBuffer == nullptr)
		{
			return;
		}
		char hextable[] = "0123456789abcdef";
	
		// check remaining capacity against to be added
		size_t nSize = szOutput.size() + nLen*2;
		size_t nCapacity = szOutput.capacity();
		if (nCapacity < nSize)
		{
			// reserve larger string-buffer
			szOutput.reserve(nSize);
		}
		
		// determine half-bytes of each byte 
		// and appropriate character representing value of it
		// for hex-encoded string
		for ( size_t y = 0; y < nLen; y++ )
		{
			unsigned char upper;
			unsigned char lower;
	
			upper = lower = pBuffer[y];
			lower = lower & 0xF;
			upper = upper >> 4;
			upper = upper & 0xF;
	
			// string grows automatically so we just push new
			// characters at the end, same way with reserve().
			szOutput += hextable[upper]; szOutput += hextable[lower];
		}
	}
	
	
private slots:
    void on_actionInit_triggered();
    void on_actionAddServer_triggered();
    void on_actionAddPeer_triggered();
	
    void on_actionSend_triggered();
	void onMessage(QByteArray idPeer, QByteArray payload);
};

#endif // MAINWINDOW_H
