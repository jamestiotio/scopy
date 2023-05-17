#ifndef GRTOPBLOCK_H
#define GRTOPBLOCK_H

#include <QObject>
#include <gnuradio/top_block.h>
#include "grsignalpath.h"
#include "scopy-gr-util_export.h"

namespace scopy {
class SCOPY_GR_UTIL_EXPORT GRTopBlock : public QObject
{
	Q_OBJECT
public:
	GRTopBlock(QString name, QObject *parent = nullptr);
	~GRTopBlock();
	GRSignalPath* addSignalPath(QString name);
	gr::top_block_sptr build();

	gr::top_block_sptr getGrBlock();

private:
	gr::top_block_sptr top;
	QString name;

	QList<GRSignalPath*> m_signalPaths;
};
}
#endif // GRTOPBLOCK_H
