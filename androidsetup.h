#ifndef ANDROIDSETUP_H
#define ANDROIDSETUP_H

#include <QObject>
#include <QDir>

class AndroidSetup
{
public:
    explicit AndroidSetup();

    QString getExternalStorageDir();
    QString getAppDataDir();

signals:

public slots:
};

#endif // ANDROIDSETUP_H
