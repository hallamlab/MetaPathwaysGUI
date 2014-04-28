#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

#include <QObject>



/* This singleton class is used for managing the memory used by the data
 *structures in the program. This memory tracking is primarity useful for the
 * caching mechanism for the large tables */

class MemoryManager : public QObject
{
    Q_OBJECT
public:

public:
    static MemoryManager *getMemoryManager();
    void setMaximumMemoryLimit(unsigned long max);
    unsigned long  getMaximumMemoryLimit();
    void addToCurrentMemoryUsed(unsigned long meminc);
    void setCurrentMemoryUsed(unsigned long currmemory);
    unsigned long  getCurrentMemoryUsed();

private:
    static MemoryManager *memorymanager;
    explicit MemoryManager(QObject *parent = 0);
    static unsigned long maximum_memory_limit;
    static unsigned long current_memory_used;

signals:
    
public slots:
    
};

#endif // MEMORYMANAGER_H
