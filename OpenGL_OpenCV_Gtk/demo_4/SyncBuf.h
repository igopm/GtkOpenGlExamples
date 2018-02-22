// 2017 by Oleksiy Grechnyev

#pragma once

#include <mutex>
#include <stdexcept>

/// The read-1, write-1 synchronized buffer with skipping frames, intended for T = cv::Mat
template <typename T, int CAPACITY = 3>
class SyncBuf{
public: //=== methods
    
    /// Start writing
    T & startWriting(int & index) {
        controlMutex.lock();
        for (int i=0; i<CAPACITY; ++i){
            if (control[i] == EMPTY) {
                control[i] = WRITING;
                controlMutex.unlock();
                index = i;
                return data[i];
            }
        }
        controlMutex.unlock();

        throw std::runtime_error("SyncBuf::startWriting : cannot find empty slot !");
    }
    
    /// End writing
    void endWriting(int index){
	if (index<0 || index>CAPACITY) 
		return;

        controlMutex.lock();
        // First we disable any previous unread data
        for (int i=0; i<CAPACITY; ++i)
            if (control[i] == CONTAINS_DATA) {
                control[i] = EMPTY;
            }
        control[index] = CONTAINS_DATA;
        controlMutex.unlock();
    }
    
    
    /// Start reading, gives index = -1 if no data !
    const T & startReading(int & index) {
        controlMutex.lock();
        for (int i=0; i<CAPACITY; ++i){
            if (control[i] == CONTAINS_DATA) {
                control[i] = READING;
                controlMutex.unlock();
                index = i;
                return data[i];
            }
        }
        // Data not found, it's OK for read op, no exceptions here :)
        controlMutex.unlock();
        index = -1;
        return data[0]; 
    }
    
    /// End reading
    void endReading(int index){
	if (index<0 || index>CAPACITY) 
		return;
        controlMutex.lock();
        control[index] = EMPTY;
        controlMutex.unlock();
    }

public: //=== Data
    
    /// Data
    T data[CAPACITY];

private: //=== Data
    /// Mutex guarding control
    std::mutex controlMutex; 
    
    /// Control block: 0 = empty, 1 = reading, 2 = writing, 3 = contains data
    int control[CAPACITY] = {EMPTY, EMPTY, EMPTY};

    enum STATE {
        EMPTY, READING, WRITING, CONTAINS_DATA
    };
};
