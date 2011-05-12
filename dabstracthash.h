#ifndef DABSTRACTHASH_H
#define DABSTRACTHASH_H

#include <QObject>
#include <QCryptographicHash>


class dAbstractHash : public QObject
{
public:
	dAbstractHash(QObject *parent = 0)
		: QObject(parent)
	{
	}
	virtual ~dAbstractHash(void)
	{}
	
	// get hash of given string
	//
	virtual QByteArray getHashed(const QByteArray &szData) = 0;
};

// dummy hashing: just plaintext
//
class dDummyHash : public dAbstractHash
{
public:
	dDummyHash(QObject *parent = 0)
		: dAbstractHash(parent)
	{}
	virtual ~dDummyHash(void)
	{}
	
	virtual QByteArray getHashed(const QByteArray &szData)
	{
		return szData;
	}
};

class dSha1Hash : public dAbstractHash, protected QCryptographicHash
{
public:
	dSha1Hash(QObject *parent = 0)
		: dAbstractHash(parent)
		, QCryptographicHash(QCryptographicHash::Sha1)
	{}
	virtual ~dSha1Hash(void)
	{}
	
	virtual QByteArray getHashed(const QByteArray &szData)
	{
		addData(szData);
		return result();
	}
};

#endif // DABSTRACTHASH_H

