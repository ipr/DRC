#ifndef DABSTRACTSIGN_H
#define DABSTRACTSIGN_H

#include <QObject>
#include <QByteArray>

class dAbstractSign : public QObject
{
public:
	dAbstractSign(QObject *parent = 0)
		: QObject(parent)
	{
	}
	virtual ~dAbstractSign(void)
	{}

	// returns type of signing for message
	// so receiver can apply correct checking
	// note: 5 char max for type
	virtual QString getType() = 0;
	
	// for sending, sign message to payload:
	//
	// szMsg = message to sign
	// outSigned = message payload with signing ("as-is" for dummy-sign)
	//
	virtual void getSigned(const QString &szMsg, QByteArray &outSigned) = 0;

	// for receiving, get plaintext from signed message:
	//
	// szType = signing type (from message), such as "PGP"
	// payload = signed payload
	//
	// returns: plaintext
	virtual QString getUnsigned(const QString &szType, const QByteArray &payload) = 0;
};

// dummy signing: just plaintext
//
/*
class dDummySign : public dAbstractSign
{
public:
	dDummySign(QObject *parent = 0)
		: dAbstractSign(parent)
	{}
	virtual ~dDummySign(void)
	{}
	
	virtual QString getType()
	{
		// note: 5 char max for type
		// or: return "DUMMY"
		return "";
	}

	virtual void getSigned(const QString &szMsg, QByteArray &outSigned)
	{
		outSigned = szMsg;
	}
	
	virtual QString getUnsigned(const QString &szType, const QByteArray &payload)
	{
		if (getType() != szType)
		{
			// unsupported signing-type?
		}
		
		// 
		return payload;
	}
};
*/

#endif // DABSTRACTSIGN_H
