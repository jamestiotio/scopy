#ifndef DEVICEBROWSER_H
#define DEVICEBROWSER_H

#include <QHBoxLayout>
#include <QWidget>
#include <QButtonGroup>
#include <QMap>


namespace Ui {
class DeviceBrowser;
}

namespace adiscope {
class DeviceBrowser : public QWidget
{
	Q_OBJECT

public:
	explicit DeviceBrowser(QWidget *parent = nullptr);
	~DeviceBrowser();
	QAbstractButton *getDeviceWidgetFor(QString id);
	void addDevice(QString id, QString name, QString description, QWidget *icon, int position = -1);
	void removeDevice(QString id);	

Q_SIGNALS:
	void requestDevice(QString id, int direction);

public Q_SLOTS:	

	void nextDevice();
	void prevDevice();	

private Q_SLOTS:
	void updateSelectedDeviceIdx(QString);
	void forwardRequestDeviceWithDirection();

private:
	void initBtns();
	QAbstractButton* buildDeviceIcon(QString name, QString description, QWidget *icon, QWidget *parent = nullptr);
	Ui::DeviceBrowser *ui;
	QButtonGroup *bg;
	QHBoxLayout *layout;
	QList<QAbstractButton*> list;
	int currentIdx;

	int getIndexOfId(QString k);
	QString getIdOfIndex(int idx);
	const char* devBrowserId = "DeviceBrowserId";

};
}
#endif // DEVICEBROWSER_H