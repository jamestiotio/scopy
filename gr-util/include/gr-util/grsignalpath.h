#ifndef GRSIGNALPATH_H
#define GRSIGNALPATH_H
#include <QObject>
#include "grproxyblock.h"
#include "scopy-gr-util_export.h"

namespace scopy {
class GRTopBlock;
class GRProxyBlock;

class SCOPY_GR_UTIL_EXPORT GRSignalPath : public GRProxyBlock {
	Q_OBJECT
public:
	GRSignalPath(QString name, QObject *parent = nullptr);

	GRProxyBlock* endPoint();
	void append(GRProxyBlock* p);
	void build_blks();
	void destroy_blks();
	virtual void connect_blk(GRTopBlock *top, GRProxyBlock *src);

	virtual void disconnect_blk();
protected:
	QList<GRProxyBlock*> list;
	QString name;
};
}
#endif // GRSIGNALPATH_H
