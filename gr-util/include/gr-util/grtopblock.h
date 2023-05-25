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
	QList<GRSignalPath*> signalPaths() { return m_signalPaths;}

	void connect(gr::basic_block_sptr src, int srcPort, gr::basic_block_sptr dst, int dstPort);

	gr::top_block_sptr getGrBlock();

Q_SIGNALS:
	void builtSignalPaths();
	void teardownSignalPaths();

public Q_SLOTS:
	void build();
	void teardown();
	void rebuild();
	void start();
	void stop();
	void run();

private:
	gr::top_block_sptr top;
	QString m_name;
	bool running;
	bool built;
	QList<GRSignalPath*> m_signalPaths;
};


}
#endif // GRTOPBLOCK_H
