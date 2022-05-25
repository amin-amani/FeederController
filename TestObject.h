#ifndef TESTOBJECT_H
#define TESTOBJECT_H

#include <QObject>
#include <QDebug>
#include <QThread>
#include "ManualModbus.h"

class TestObject : public QObject
{
    Q_OBJECT
public:
    explicit TestObject(QObject *parent = nullptr);
public slots:

    void Start();

signals:
    void testSignal(ManualModbus::AlarmColor color ,int val);

};

#endif // TESTOBJECT_H
