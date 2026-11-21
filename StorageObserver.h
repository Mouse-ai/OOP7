#ifndef OOP7_STORAGEOBSERVER_H
#define OOP7_STORAGEOBSERVER_H

class StorageObserver
{
public:
    virtual ~StorageObserver() = default;
    virtual void onStorageChanged() = 0;
};

#endif