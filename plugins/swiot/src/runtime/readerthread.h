/*
 * Copyright (c) 2023 Analog Devices Inc.
 *
 * This file is part of Scopy
 * (see https://www.github.com/analogdevicesinc/scopy).
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


#ifndef READERTHREAD_H
#define READERTHREAD_H

#include "src/runtime/ad74413r/chnlinfo.h"
#include <QThread>
#include <QMap>
#include <QMutex>
#include <atomic>
#include <iio.h>
#include <iioutil/commandqueue.h>

namespace scopy::swiot {
class ReaderThread : public QThread {
	Q_OBJECT
public:
	explicit ReaderThread(bool isBuffered, CommandQueue *cmdQueue, QObject *parent = nullptr);
	~ReaderThread();

	void addDioChannel(int index, struct iio_channel *channel);

	void addBufferedDevice(iio_device *device);

	void runDio();
        void singleDio();

	void runBuffered(int requiredBuffersNumber = 0);

	void createIioBuffer();

	void destroyIioBuffer();

	void enableIioChnls();

	int getEnabledChnls();

	QVector<ChnlInfo *> getEnabledBufferedChnls();

	void startCapture(int requiredBuffersNumber = 0);

	void requestStop();
public Q_SLOTS:

	void onChnlsChange(QMap<int, ChnlInfo *> chnlsInfo);
	void onSamplingFreqWritten(int samplingFreq);

Q_SIGNALS:

	void bufferRefilled(QVector<QVector<double>> bufferData, int bufferCounter);

	void channelDataChanged(int channelId, double value);

private Q_SLOTS:
	void bufferRefillCommandFinished(scopy::Command *cmd);
	void bufferCreateCommandFinished(scopy::Command *cmd);
	void bufferDestroyCommandFinished(scopy::Command *cmd);

private:
	void createBufferRefillCommand();
	void createBufferDestroyCommand();

        void run() override;

	bool isBuffered;
	QMap<int, struct iio_channel *> m_dioChannels;

	int m_samplingFreq = 4800;
	int m_enabledChnlsNo;
	std::atomic<int> bufferCounter;
	std::atomic<int> m_requiredBuffersNumber;
	std::condition_variable m_cond, m_condBufferCreated;

	CommandQueue *m_cmdQueue;
	QVector<scopy::Command*> m_dioChannelsReadCommands;
	Command *m_refillBufferCommand, *m_createBufferCommand;
	Command *m_destroyBufferCommand;

	struct iio_device *m_iioDev;
	struct iio_buffer *m_iioBuff;
	QMap<int, ChnlInfo*> m_chnlsInfo;
	QVector<ChnlInfo *> m_bufferedChnls;
	QVector<QVector<double>> m_bufferData;

	std::mutex m_mutex;
};
}

#endif // READERTHREAD_H
