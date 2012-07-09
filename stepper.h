#ifndef STEPPER
#define STEPPER

#include <vector>
#include <sstream>
#include "../rs232/serial_factory.h"

//! \todo . stepper base that do no movement have no comm or reader ! but have virtual move and move_jitter AND factory !
class Cstepper
{
public:
  //! class name for debug only
#if cimg_debug>1
  std::string class_name;
#endif
  //! class (or library) version for information only
  std::string class_version;
  std::vector<std::string> axis_name;
  //! constructor
  /**
   *
  **/
  Cstepper()
  {
    ///set class name (debug only)
#if cimg_debug>1
    class_name="Cstepper";
#endif
    ///set library version (information only)
    class_version=STEPPER_VERSION;
    ///set axis names
    axis_name.push_back("X");
    axis_name.push_back("Y");
    axis_name.push_back("Z");
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
    std::cerr<<"warning: this is fake stepper opening.\n"<<std::flush;
    return true;
  }//open

  //! convert value to string
  /**
   * format value to string
  **/
  template<typename T> std::string valueToString(const T& value)
  {
    std::ostringstream stream;
    stream << value;
    return stream.str();
  }//valueToString

  //! set both direction AND velocity command
  /** 
   *
   * @param[in] step=number of step to do
   * @param[in] velocity=velocity to go
   * @param[in] axis=translation along axis (as string; e.g. "X")
   *
   * @return message (i.e. string to send to serial port)
   */
  virtual const std::string set_directionNvelocity_command(const int step,const int velocity,const std::string axis) {return "fake";}
  //! set step command
  /** 
   *
   * @param[in] step=number of step to do
   * @param[in] axis=translation along axis (as string; e.g. "X")
   *
   * @return message (i.e. string to send to serial port)
   */
  virtual const std::string set_step_command(int step,const std::string axis) {return "fake";}

  //! move once using 1D step and 1D velocity along specific axis
  /** 
   *
   * @param[in] axis_index=axis index (using axis[axis_index])
   * @param[in] step=steps along specified axis
   * @param[in] velocity=velocity for axis
   *
   * @return true on success, false otherwise
   */
  virtual bool move(const int axis_index,const int step,const int velocity)
  {
    std::cerr<<"warning: this is fake stepper movement "<<
      "on "<<axis_name[axis_index]<<" axis "
      "by "<<step<<"step(s) "<<
      "at "<<velocity<<" step/s.\n"<<std::flush;
    return true;
  }//move

  //! move once using 3D step (and 3D velocity)
  /** 
   *
   * @param[in] step=steps in the 3 directions (XYZ)
   * @param[in] velocity=velocities in the 3 directions (XYZ)
   *
   * @return true on success, false otherwise
   */
  virtual bool move(const cimg_library::CImg<int> &step,const cimg_library::CImg<int> &velocity)
  {
    std::cerr<<"warning: this is fake stepper movement "<<
      "on ("<<axis_name[0]<<","<<axis_name[1]<<","<<axis_name[2]<<") axes "<<
      "by ("<<step(0)<<","<<step(1)<<","<<step(2)<<") step(s) "<<
      "at ("<<velocity(0)<<","<<velocity(1)<<","<<velocity(2)<<") step/s.\n"<<std::flush;
    return true;
  }//move

  //! Close stepper
  /** 
   *
   * @return 
   */
  virtual bool close()
  {
    std::cerr<<"warning: this is fake stepper closing.\n"<<std::flush;
    return true;
  }//close

};//Cstepper class

class Cstepper_uControlXYZ: public Cstepper
{
protected:
  //! communication with stepper
  Cserial* pComStepper;
public:
  //! constructor
  /**
   *
  **/
  Cstepper_uControlXYZ()
  {
    ///set class name (debug only)
#if cimg_debug>1
    class_name="Cstepper_uControlXYZ";
#endif
    ///set library version (information only)
    class_version=STEPPER_VERSION;
    ///set axis names
    axis_name.push_back("X");
    axis_name.push_back("Y");
    axis_name.push_back("Z");
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

  //! convert value to string
  /**
   * format value to string
  **/
  template<typename T> std::string valueToString(const T& value)
  {
    std::ostringstream stream;
    stream << value;
    return stream.str();
  }//valueToString

  //! set both direction AND velocity command
  /** 
   *
   * @param[in] step=number of step to do
   * @param[in] velocity=velocity to go
   * @param[in] axis=translation along axis (as string; e.g. "X")
   *
   * @return message (i.e. string to send to serial port)
   */
  const std::string set_directionNvelocity_command(const int step,const int velocity,const std::string axis)
  {
    const std::string direction=(step<0)?std::string("-"):std::string("+");
    //axis
    const std::string suffix=";";
    const std::string prefix="R";
    const std::string formated_velocity=valueToString(velocity);
    const std::string command=direction+axis+suffix+prefix+axis+formated_velocity+suffix;
    return command;//e.g. "+X;RX1000"
  }

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
    //axis
    const std::string formated_step=valueToString(std::abs(step));
    const std::string suffix=";";
    const std::string command=prefix+axis+formated_step+suffix;
    return command;//e.g. "NX123;"
  }

  //! move once using 1D step and 1D velocity along specific axis
  /** 
   *
   * @param[in] axis_index=axis index (using axis[axis_index])
   * @param[in] step=steps along specified axis
   * @param[in] velocity=velocity for axis
   *
   * @return true on success, false otherwise
   */
  bool move(const int axis_index,const int step,const int velocity)
  {
    //null displacement
    if(step==0) return true;
    //other displacement
    std::string command;
    ///Set direction and velocity (e.g. "+X;RX1000;")
    command=set_directionNvelocity_command(step,velocity,axis_name[axis_index]);
std::cerr<<"Set direction and velocity: write=|"<<command<<"|"<<std::flush;
    if(!pComStepper->writes(command)) return false;
//    pComStepper->reads(command);
//std::cerr<<"read=|"<<command<<"|\n"<<std::flush;
    ///Set displacement
    command=set_step_command(step,axis_name[axis_index]);//e.g. "NX123;"
std::cerr<<"Set displacement: write=|"<<command<<"|"<<std::flush;
    if(!pComStepper->writes(command)) return false;
//    pComStepper->reads(command);
//std::cerr<<"read=|"<<command<<"|\n"<<std::flush;
    ///Execute displacement
    command="M"+axis_name[axis_index]+";";//e.g. "MX;";
std::cerr<<"Make displacement: write=|"<<command<<"|"<<std::flush;
    if(!pComStepper->writes(command)) return false;
//    pComStepper->reads(command);
//std::cerr<<"read=|"<<command<<"|\n"<<std::flush;
    return true;
  }//move

  //! move once using 3D step (and 3D velocity)
  /** 
   *
   * @param[in] step=steps in the 3 directions (XYZ)
   * @param[in] velocity=velocities in the 3 directions (XYZ)
   *
   * @return true on success, false otherwise
   */
  bool move(const cimg_library::CImg<int> &step,const cimg_library::CImg<int> &velocity)
  {
    cimg_forX(step,i)
    {
      if(!move(i,step(i),velocity(i)))
      {
        std::cerr<<"error: while moving "<<axis_name[i]<<" axis (i.e. index="<<i<<").\n"<<std::flush;
        return false;
      }
      //wait for the movement ends
//! \todo use blocking RS232 functions
      if(step(i)!=0) cimg_library::cimg::wait(1000);
    }//axis loop
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

};//Cstepper_uControlXYZ class

class Cstepper_uControlXYZ_reader: public Cstepper_uControlXYZ
{
protected:
  //! communication with position reader
  Cserial* pComReader;
public:
  //! constructor
  /**
   *
  **/
  Cstepper_uControlXYZ_reader()
  {
    Cstepper_uControlXYZ::Cstepper_uControlXYZ();
    ///set class name (debug only)
#if cimg_debug>1
    class_name="Cstepper_uControlXYZ_reader";
#endif
  }//constructor

  //! Open stepper device
  /** 
   *
   * @param[in] stepper_port_path: path of serial port device connected to stepper
   * @param[in] stepper_port_type: type of serial port for stepper (see \c Cserial_factory:: )
   * @param[in]  reader_port_path: path of serial port device connected to position reader
   * @param[in]  reader_port_type: type of serial port for reader  (see \c Cserial_factory:: )
   *
   * @return 
   */
  bool open(const std::string& stepper_port_path,const std::string& stepper_port_type,
            const std::string&  reader_port_path,const std::string&  reader_port_type)
  {
    //choose serial
    Cserial_factory serial_factory;
    pComStepper=serial_factory.create(stepper_port_type);
    pComReader= serial_factory.create( reader_port_type);
    //initialise
    if(!pComStepper->opens(stepper_port_path)) return false;
    if(!pComReader->opens(  reader_port_path)) return false;
    return true;
  }//open

  //! move once using 1D step and 1D velocity along specific axis
  /** 
   *
   * @param[in] axis_index=axis index (using axis[axis_index])
   * @param[in] step=steps along specified axis
   * @param[in] velocity=velocity for axis
   *
   * @return true on success, false otherwise
   */
  bool move(const int axis_index,const int position,const int velocity)
  {
    //null displacement
    if(position==0) return true;
    //other displacement
//! \todo [high] force position
/*
read position
check return true;
loop to force absolute position
*/
    return true;
  }//move

  //! move once using 3D step (and 3D velocity)
  /** 
   *
   * @param[in] step=steps in the 3 directions (XYZ)
   * @param[in] velocity=velocities in the 3 directions (XYZ)
   *
   * @return true on success, false otherwise
   */
  bool move(const cimg_library::CImg<int> &position,const cimg_library::CImg<int> &velocity)
  {
    cimg_forX(position,i)
    {
      if(!move(i,position(i),velocity(i)))
      {
        std::cerr<<"error: while moving "<<axis_name[i]<<" axis (i.e. index="<<i<<").\n"<<std::flush;
        return false;
      }
    }//axis loop
    return true;
  }//move

  //! reset position reader
  /** 
   *
   * @return 
   */
  bool reset()
  {
    return pComReader->writes("reset");
  }//reset

  //! get position from reader
  /** 
   *
   * @return 
   */
  bool position(int &x,int &y,int &z)
  {
    std::string ask="XYZ";
    std::string value;
    const int number_of_try=3;
    const int try_wait_time=20;
    //get value (as string from RS232)
    if(!pComReader->gets(ask,value,number_of_try,try_wait_time)) return false;
    //get XYZ values (from string)
    if( sscanf(value.c_str(),"v: X=%d Y=%d Z=%d",&x,&y,&z)!=3 ) {x=y=z=1235467890;return false;}
std::cerr<<
   #ifdef cimg_use_vt100
    cimg_library::cimg::t_green<<
   #endif
   "(x,y,z)=("<<x<<","<<y<<","<<z<<").\n"<<
   #ifdef cimg_use_vt100
    cimg_library::cimg::t_normal<<
   #endif
   std::flush;
    return true;
  }//position

//use CImg
#ifdef cimg_version
  //! get position from reader
  /** 
   *
   * @return 
   */
  bool position(cimg_library::CImg<int> &xyz)
  {
    int x,y,z;
    if(!position(x,y,z)) return false;
    xyz.assign(3);
    xyz(0)=x;xyz(1)=y;xyz(2)=z;
    return true;
  }//position
#endif //use CImg

  //! Close stepper
  /** 
   *
   * @return 
   */
  bool close()
  {
    return (pComStepper->closes()&&pComReader->closes());
  }//close

};//Cstepper_uControlXYZ_reader class


#endif //STEPPER

