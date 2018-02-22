//
// Created by Igor Maschikevich on 11/06/2017.
//
//=====================================================================================================
#include "OtherThreads.h"
#include "VDecoder.h"
//using boost::asio::ip::udp;
using namespace std;
//using namespace std::chrono;
using namespace cv;
//using namespace glm;
//using namespace Dronestreamer;
//==============================================================
OtherThreads::OtherThreads(string udpHostE,string udpPortE):
    udpHost(udpHostE),
    udpPort(udpPortE),
    stopFlag(false),
    countReceived(0),
    countDecoded(0),
    countShown(0),
    countGL(0),
    receiveThread([this]() -> void{
                  cout <<"OtherStreams "<<udpHost << endl;
                  cout <<"OtherStreams "<<udpPort << endl;
                  }),
    decodeThread([this]()->void{
                 VDecoder vdec(ST_CAMERA_WIDTH, ST_CAMERA_HEIGHT); // Decoder

                 //                 Mat frameOutYuv(ST_CAMERA_HEIGHT * 3 / 2, ST_CAMERA_WIDTH, CV_8UC1); // Pre-allocate the YUV frame for speed

                 while (!stopFlag) {
                 int ind1, ind2;

                 //                 const auto &buf1Slot = buf1.startReading(ind1);

                 //                 if (-1 == ind1) {
                 //                 this_thread::sleep_for(chrono::milliseconds(1)); // No data available for now, we wait
                 //                 } else {
                 //                 size_t packetSize = buf1Slot.second;
                 //                 uint8_t *pos = (uint8_t *) buf1Slot.first.data();

                 //                 SupData sup;
                 //                  Read supplemental data if requested
                 //                 if (ST_USE_SUP_DATA) {
                 //                 size_t sz = sup.readFrom(pos);
                 //                 pos += sz;
                 //                 packetSize -= sz;
                 //                 }

                 //                 if (ST_USE_JPEG) {
                 // Use JPEG
                 //                 Mat rawMat = Mat(1, packetSize, CV_8UC1, (unsigned char *) pos);
                 //                 auto &buf2Slot = buf2.startWriting(ind2);
                 //                 Mat &frameOut = buf2Slot.second;
                 //                 buf2Slot.first = sup; // Supplemental data
                 //                 frameOut = imdecode(rawMat, IMREAD_COLOR); // Decode jpeg data
                 //                 buf2.endWriting(ind2);
                 //                 } else {
                 //                  Use codecs
                 //                 vdec.parse(pos, packetSize,
                 //                            [&buf2, &frameOutYuv, &stopFlag, &sup]
                 //                            (void *d0, void *d1, void *d2) -> void {
                 //    int ind2;
                 // Create a Yuv frame
                 //    const size_t size0 = ST_CAMERA_WIDTH * ST_CAMERA_HEIGHT / 4;
                 //    memcpy(frameOutYuv.data, d0, size0 * 4);
                 //    memcpy(frameOutYuv.data + size0 * 4, d1, size0);
                 //    memcpy(frameOutYuv.data + size0 * 5, d2, size0);
                 //    auto &buf2Slot = buf2.startWriting(ind2);
                 //    Mat &frameOut = buf2Slot.second;
                 //    buf2Slot.first = sup; // Supplemental data
                 //    cvtColor(frameOutYuv, frameOut, CV_YUV2BGR_I420); // Convert YUV420p to BGR
                 //    buf2.endWriting(ind2);
                 //});
                 //                 }
                 //                 buf1.endReading(ind1);

                 //                 ++countDecoded;
                 //                 }
                 }
                 })
{
}
OtherThreads::~OtherThreads(){
    stopFlag = true;
    // Join all threads
    receiveThread.join();
    decodeThread.join();
}
