#include "contextprovider.h"
#include <QLoggingCategory>
#include <QDebug>
#include <QApplication>

Q_LOGGING_CATEGORY(CAT_CTXMGR, "ContextProvider")

ContextRefCounter::ContextRefCounter(QString uri)
{
	this->uri = uri;
	this->refcnt++;
	this->ctx = iio_create_context_from_uri(uri.toStdString().c_str());
}

ContextRefCounter::~ContextRefCounter() {
	iio_context_destroy(this->ctx);
}


ContextProvider* ContextProvider::pinstance_{nullptr};
std::mutex ContextProvider::mutex_;

ContextProvider::ContextProvider(QObject *parent) : QObject(parent)
{
	qDebug(CAT_CTXMGR)<<"ctor";
}

ContextProvider::~ContextProvider()
{
	qDebug(CAT_CTXMGR)<<"dtor";
}

ContextProvider *ContextProvider::GetInstance()
{
	std::lock_guard<std::mutex> lock(mutex_);
	if (pinstance_ == nullptr)
	{
		pinstance_ = new ContextProvider(QApplication::instance()); // singleton has the app as parent
	} else {
		qDebug(CAT_CTXMGR)<<"got instance from singleton";
	}
	return pinstance_;
}

iio_context *ContextProvider::open(QString uri)
{
	std::lock_guard<std::mutex> lock(mutex_);
	iio_context *ctx;	
	if(map.contains(uri)) {
		map.value(uri)->refcnt++;
		qDebug(CAT_CTXMGR)<< uri << "opening - found - refcnt++ = " << map.value(uri)->refcnt;
	} else {
		map.insert(uri, new ContextRefCounter(uri));
		qDebug(CAT_CTXMGR)<< uri << "opening  - created in map - refcnt = "<< map.value(uri)->refcnt;
	}

	ctx = map.value(uri)->ctx;
	return ctx;
}

void ContextProvider::close(QString uri)
{
	std::lock_guard<std::mutex> lock(mutex_);
	if(map.contains(uri)) {
		map.value(uri)->refcnt--;
		qDebug(CAT_CTXMGR)<< uri << "closing - found in map - refcnt-- = " << map.value(uri)->refcnt;
		if(map[uri]->refcnt == 0) {
			map.remove(uri);
			qDebug(CAT_CTXMGR)<< uri << "removed from map";
		}
	}
}

/*
auto cm = ContextManager::GetInstance();
ContextScanner *cs = new ContextScanner(this);
cs->startScan(5000);
cm->open("ip:192.168.2.1");
cm->open("ip:192.168.2.1");
cm->open("ip:192.168.2.1");
cm->close("ip:192.168.2.1");
auto cd = ContextManager::GetInstance();
cd->close("ip:192.168.2.1");
cd->close("ip:192.168.2.1");*/

