#include "networkworker.h"

/*
 * 注意：
 * boost库只能放在调用他的地方，放在其他地方导致winsock.h头文件重复include错误。             ***NOTE HERE***
 */
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/thread/thread.hpp>
using boost::asio::ip::tcp;
netWorkWorker::netWorkWorker()
{

}

netWorkWorker::~netWorkWorker()
{

}

void netWorkWorker::doWork()
{
    Mat  pngimage;
        try{
            boost::asio::io_service io_service;
            tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 3200));
            tcp::socket socket(io_service);
            acceptor.accept(socket);
            boost::system::error_code error;
            while(true)
            {
                mutex.lock();
                bool abort = _abort;
                mutex.unlock();
                if (abort) {
                    qDebug()<<"Aborting worker process in Thread "<<thread()->currentThreadId();
                    break;
                }
                //do the main work here
                boost::array< char, 16 > header;
                std::size_t length = boost::asio::read(socket, boost::asio::buffer(header), boost::asio::transfer_all(), error);
//                cout<<"length : "<< length<<endl;
                if(length == 16)
                {

                    std::vector<uchar> body(atoi((string(header.begin(),header.end())).c_str()));
                    std::size_t lengthbody = boost::asio::read(socket, boost::asio::buffer(body), boost::asio::transfer_all(), error);
                    pngimage = imdecode(Mat(body),CV_LOAD_IMAGE_COLOR);
                    Mat tempimg;
                    pngimage.copyTo (tempimg);

                    emit imageChanged (tempimg);

                }else{

//                    cout<<"here go length==0"<<endl;
//                    cout<<"nothing to read..."<<endl;

                    break;

                }


            }
//            cout<<"ui socket close"<<endl;
            socket.close();
            io_service.stop ();
        }catch(std::exception& e){
            std::cerr << e.what() << std::endl;
        }


        mutex.lock();
        _working = false;
        mutex.unlock();
        qDebug()<<"Worker （BOOST LIB） getImageFromNetWork process finished in Thread "<<thread()->currentThreadId();
        emit finished();


}

