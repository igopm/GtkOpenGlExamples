//
// Created by  Oleksiy Grechnyev on 6/30/2017.
//

#pragma once

constexpr int ST_CAMERA_WIDTH = 640;
constexpr int ST_CAMERA_HEIGHT = 480;
constexpr int ST_CAMERA_FPS = 90;
constexpr int ST_UDP_PORT = 19090;
constexpr bool ST_USE_JPEG = false;
constexpr bool ST_USE_SUP_DATA = true;  // Use supplemental data for H264
constexpr int ST_JPEG_QUALITY = 50;
constexpr bool ST_DRAW_MARKERS_ON_SERVER = true;

// If true, use the frame0 as the header for all frames, need that for the OpenMax (OMX) H264 on RPi
constexpr bool ST_USE_H264_HEADER = true;
constexpr const char * ST_H264_PRESET = "ultrafast"; // ultrafast supefast veryfast faster fast ...
constexpr int ST_H264_CRF = 30;   // Compression rate: 0 = lossless, 51 = worst quality, 25-30 = OK
