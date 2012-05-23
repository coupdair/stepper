#ifndef STEPPER
#define STEPPER

#include "../rs232/serialCOM.h"

class Cstepper
{
  //! communication with stepper
  serialCOM comStepper;
public:

  //! Open stepper device
  /** 
   *
   * @param[in] port_path
   *
   * @return 
   */
  bool open(const std::string& port_path)
  {
    //open serial
    
    //initialise

    return comStepper.opens(port_path);
  }//open
  
  //! move once using 3D step (and 3D velocity)
  /** 
   *
   * @param[in] message= string to send to serial port  
   *
   * @return 
   */
  bool move(const cimg_library::CImg<int> &step,const cimg_library::CImg<int> &velocity)
  {
    std::string command;
    ///Set direction and velocity (e.g. "+X;RX1000")
    command=(step(0)<0)?std::string("-"):std::string("+");
    command+=std::string("X;");//e.g. +X;
    command+="RX1000;";//e.g. "RX1000"
std::cerr<<"Set direction and velocity: write=|"<<command<<"|"<<std::flush;
//    comStepper.writes(command);
//    comStepper.reads(command);
std::cerr<<"read=|"<<command<<"|\n"<<std::flush;
    ///Set displacement
    command="NX1;";
std::cerr<<"Set displacement: write=|"<<command<<"|"<<std::flush;
//    comStepper.writes(command);
//    comStepper.reads(command);
std::cerr<<"read=|"<<command<<"|\n"<<std::flush;
    ///Execute displacement
    command="MX;";
std::cerr<<"Set displacement: write=|"<<command<<"|"<<std::flush;
//    comStepper.writes(command);
//    comStepper.reads(command);
std::cerr<<"read=|"<<command<<"|\n"<<std::flush;
    return true;
  }//move

  //! Close stepper
  /** 
   *
   * @return 
   */
  bool close()
  {
    return comStepper.closes();
  }//close

};//Cstepper class

#endif //STEPPER
