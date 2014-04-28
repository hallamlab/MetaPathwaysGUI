#include "memorymanager.h"

MemoryManager *MemoryManager::memorymanager=0;
unsigned long MemoryManager::maximum_memory_limit= 1000000000;
unsigned long MemoryManager::current_memory_used = 0;

MemoryManager::MemoryManager(QObject *parent) :
    QObject(parent)
{
}

MemoryManager *MemoryManager::getMemoryManager() {
    if( MemoryManager::memorymanager==0)
        MemoryManager::memorymanager = new MemoryManager;

    return MemoryManager::memorymanager;
}


void MemoryManager::setMaximumMemoryLimit(unsigned long max) {

    this->maximum_memory_limit = max;
}

unsigned long  MemoryManager::getMaximumMemoryLimit() {
    return this->maximum_memory_limit;
}

void MemoryManager::addToCurrentMemoryUsed(unsigned long meminc) {
    this->current_memory_used += meminc;
}

void MemoryManager::setCurrentMemoryUsed(unsigned long currmemory) {
    this->current_memory_used = currmemory;

}

unsigned long  MemoryManager::getCurrentMemoryUsed() {
    return this->current_memory_used;
}
