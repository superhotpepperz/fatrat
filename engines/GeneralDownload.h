#ifndef _GENERALDOWNLOAD_H
#define _GENERALDOWNLOAD_H
#include "Transfer.h"
#include <QUrl>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QHttp>
#include <QPair>
#include <QTime>
#include <QReadWriteLock>
#include <QDialog>
#include "LimitedSocket.h"
#include "ui_HttpOptsWidget.h"
#include "ui_HttpUrlOptsDlg.h"
#include "WidgetHostChild.h"
#include "fatrat.h"

class GeneralDownload : public Transfer
{
Q_OBJECT
public:
	GeneralDownload(bool local = false);
	virtual ~GeneralDownload();
	
	virtual void changeActive(bool);
	virtual void speeds(int& down, int& up) const;
	virtual qulonglong total() const { return m_nTotal; }
	virtual qulonglong done() const;
	virtual QString name() const;
	virtual QString myClass() const { return "GeneralDownload"; }
	virtual QString message() const { return m_strMessage; }
	
	virtual void init(QString uri,QString dest);
	virtual void setObject(QString target);
	void init2(QString uri,QString dest);
	virtual void load(const QDomNode& map);
	virtual void save(QDomDocument& doc, QDomNode& map) const;
	virtual QString object() const { return m_dir.path(); }
	
	virtual void setSpeedLimits(int down,int up);
	// options of a particular download
	virtual WidgetHostChild* createOptionsWidget(QWidget* w);
	virtual void fillContextMenu(QMenu& menu);
	
	// global download options
	static WidgetHostChild* createSettingsWidget(QWidget* w,QIcon&);
	static QDialog* createMultipleOptionsWidget(QWidget* parent, QList<Transfer*>& transfers);
	
	static int acceptable(QString uri, bool);
	static Transfer* createInstance() { return new GeneralDownload; }
	
	QString filePath() const { return m_dir.filePath(name()); }
	void setTargetName(QString strName) { m_strFile=strName; }
	
private slots:
	void requestFinished(bool error);
	void responseSizeReceived(qint64 totalsize);
	void redirected(QString newurl);
	void changeMessage(QString msg) { m_strMessage = msg; }
	
	void switchMirror();
	void computeHash();
private:
	void startHttp(QUrl url, QUrl referrer = QUrl());
	void startFtp(QUrl url);
	void startSftp(QUrl url);
	void generateName();
protected:
	QUrl m_urlLast;
	QDir m_dir;
	qulonglong m_nTotal,m_nStart;
	
	QString m_strMessage, m_strFile;
	
	LimitedSocket* m_engine;
	
	struct UrlObject
	{
		QUrl url;
		QString strReferrer, strBindAddress;
		FtpMode ftpMode;
		QUuid proxy;
	};
	QList<UrlObject> m_urls;
	int m_nUrl;
	
	friend class HttpOptsWidget;
	friend class HttpUrlOptsDlg;
};

class HttpOptsWidget : public QObject, public WidgetHostChild, Ui_HttpOptsWidget
{
Q_OBJECT
public:
	HttpOptsWidget(QWidget* me,GeneralDownload* myobj);
	virtual void load();
	virtual void accepted();
	virtual bool accept();
public slots:
	void addUrl();
	void editUrl();
	void deleteUrl();
private:
	GeneralDownload* m_download;
	QList<GeneralDownload::UrlObject> m_urls;
};

class HttpUrlOptsDlg : public QDialog, Ui_HttpUrlOptsDlg
{
Q_OBJECT
public:
	HttpUrlOptsDlg(QWidget* parent, QList<Transfer*>* multi = 0);
	void init();
	int exec();
	void runMultiUpdate();
	virtual void accept();
	
	QString m_strURL, m_strReferrer, m_strUser, m_strPassword, m_strBindAddress;
	FtpMode m_ftpMode;
	QUuid m_proxy;
	QList<Transfer*>* m_multi;
};

#endif
