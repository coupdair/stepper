#ifndef STEPPER
#define STEPPER

#include <vector>
#include "../rs232/serial_factory.h"

class Cstepper
{
public:
  //! class name for debug only
#if cimg_debug>1
  std::string class_name;
#endif
  //! class (or library) version for information only
  std::string class_version;
private:
  //! communication with stepper
  Cserial* pComStepper;
public:
  std::vector<std::string> axis;
  //! constructor
  /**
   *
  **/
  Cstepper()
  {
    ///set class name (debug only)
#if cimg_debug>1
    class_name="Cstepper_uControlXYZ";
#endif
    ///set library version (information only)
    class_version=VERSION;
    ///set axis names
    axis.push_back("X");
    axis.push_back("Y");
    axis.push_back("Z");
  }//constructor

  //! Open stepper device
  /** 
   *
   * @param[in] port_path: path of serial port device
   * @param[in] port_type: type of serial port (see \c Cserial_factory:: )
   *
   * @return 
   */
  bool open(const std::string& port_path,std::string port_type="serial_system")
  {
    //choose serial
    Cserial_factory serial_factory;
    pComStepper=serial_factory.create(port_type);
    //initialise

    return pComStepper->opens(port_path);
  }//open

  //! set step command
  /** 
   *
   * @param[in] step=number of step to do
   * @param[in] axis=translation along axis (as string; e.g. "X")
   *
   * @return message (i.e. string to send to serial port)
   */
  const std::string set_step_command(int step,const std::string axis)
  {
    const std::string prefix="N";
    const std::string formated_step="123";//! \todo int to string
    const std::string suffix=";";
    const std::string command=prefix+axis+formated_step+suffix;
    return command;//e.g. "NX123;"
  }

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
    command=(step(0)<0)?std::string("-"):std::string("+");//direction
    command+=std::string("X;");//axis (e.g. "+X;")
    command+="RX1000;";//velocity (e.g. "RX1000")
std::cerr<<"Set direction and velocity: write=|"<<command<<"|"<<std::flush;
    pComStepper->writes(command);
//    pComStepper->reads(command);
//std::cerr<<"read=|"<<command<<"|\n"<<std::flush;
    ///Set displacement
    command=set_step_command(step[0],axis[0]);//e.g. "NX123;"
std::cerr<<"Set displacement: write=|"<<command<<"|"<<std::flush;
    pComStepper->writes(command);
//    pComStepper->reads(command);
//std::cerr<<"read=|"<<command<<"|\n"<<std::flush;
    ///Execute displacement
    command="MX;";
std::cerr<<"Set displacement: write=|"<<command<<"|"<<std::flush;
    pComStepper->writes(command);
//    pComStepper->reads(command);
//std::cerr<<"read=|"<<command<<"|\n"<<std::flush;
    return true;
  }//move

  //! Close stepper
  /** 
   *
   * @return 
   */
  bool close()
  {
    return pComStepper->closes();
  }//close

};//Cstepper class

#endif //STEPPER

