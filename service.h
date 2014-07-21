#ifndef SERVICE_H
#define SERVICE_H
#include <QString>

class Service
{
public:
    Service();
    Service(QString serviceName);
    virtual ~Service();

    const QString getServiceName() const;
    const QString getServiceLogoResource() const;

protected:
    QString serviceName;
    QString serviceLogoResource;
};

#endif // SERVICE_H
