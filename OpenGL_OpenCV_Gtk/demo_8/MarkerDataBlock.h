//
// Created by  Oleksiy Grechnyev on 10/5/2017.
//

#pragma once

#include <cstring>
#include <iostream>

namespace Dronestreamer {

    struct Pos2i{
        int x, y;
    };

    struct MarkerDataBlock {
        static constexpr size_t MAX_MARKERS = 20;
        static constexpr size_t NUM_CORNERS = 4;
        /// Number of markers
        int nMarkers;
        /// Markers data
        Pos2i data[MAX_MARKERS][NUM_CORNERS];

        /// Write (serialize) MDB into position pos and return the number of bytes
        size_t writeTo(void *pos) const{
            int *iPos = (int *)pos;
            constexpr size_t sz = sizeof(data) + 2*sizeof(int);
            *(iPos++) = sz; // Size of the serialized data
            *(iPos++) = nMarkers;
            memcpy(iPos, data, sizeof(data));
            return sz;
        }

        /// Read (de-serialize) MDB from position pos
        size_t readFrom(void * pos){
            int *iPos = (int *)pos;
            int sz = *(iPos++);
            if (sz != sizeof(data) + 2*sizeof(int)) {
                std::cerr << "CORRUPTED DATA in MarkerDataBlock !\n";
                nMarkers = 0;
            } else {
                nMarkers = *(iPos++);
                memcpy(data, iPos, sizeof(data));
            }
            return sz;
        }

    };
}


