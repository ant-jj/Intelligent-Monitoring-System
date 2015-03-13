#include "getnetframeworker.h"

getNetFrameWorker::getNetFrameWorker()
{

}

getNetFrameWorker::~getNetFrameWorker()
{

}

void getNetFrameWorker::doWork()
{
    qDebug()<<"Starting worker process in Thread "<<thread()->currentThreadId();
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
               cout<<"length : "<< length<<endl;
               if(length == 16)
               {

                   std::vector<uchar> body(atoi((string(header.begin(),header.end())).c_str()));
                   std::size_t lengthbody = boost::asio::read(socket, boost::asio::buffer(body), boost::asio::transfer_all(), error);
                   pngimage = imdecode(Mat(body),CV_LOAD_IMAGE_COLOR);
                   Mat tempimg;
                   pngimage.copyTo (tempimg);

                   /*
                    * hard process
                    *
                    */

//                   QEventLoop loop;
//                   QTimer::singleShot(100, &loop, SLOT(quit()));
//                   loop.exec();

                   emit imageChanged (tempimg);

               }else{

                   /*
                   cout<<"here go length==0"<<endl;
                   cout<<"nothing to read..."<<endl;
*/
                   break;

               }


           }
//           cout<<"ui socket close"<<endl;
           socket.close();
           io_service.stop ();
       }catch(std::exception& e){
//           std::cerr << e.what() << std::endl;
       }


       mutex.lock();
       _working = false;
       mutex.unlock();
       qDebug()<<"Worker GetImageFromNetWork process finished in Thread "<<thread()->currentThreadId();
       emit finished();


}

