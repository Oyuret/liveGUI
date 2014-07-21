#include "../include/service.h"

Service::Service()
{
}

Service::Service(QString serviceName)
{
    this->serviceName = serviceName;
}

Service::~Service()
{

}

const QString Service::getServiceName() const
{
    return serviceName;
}

const QString Service::getServiceLogoResource() const
{
    return serviceLogoResource;
}
