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
  //! mechanical jitter for backward movement
  cimg_library::CImg<int> mechanical_jitter;
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
  virtual bool open(const std::string& stepper_port_path,const std::string& stepper_port_type,
            const std::string&  reader_port_path,const std::string&  reader_port_type, const cimg_library::CImg<int> &jitter)
  {
#if cimg_debug>1
std::cerr<<class_name<<"::"<<__func__<<" empty\n"<<std::flush;
#endif
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
  bool open(const std::string& stepper_port_path,const std::string& stepper_port_type,
            const std::string&  reader_port_path,const std::string&  reader_port_type, const cimg_library::CImg<int> &jitter)
  {
#if cimg_debug>1
std::cerr<<class_name<<"::"<<__func__<<"("<<stepper_port_path<<","<<stepper_port_type<<")\n"<<std::flush;
#endif
    //choose serial
    Cserial_factory serial_factory;
    pComStepper=serial_factory.create(stepper_port_type);
    //initialise

    return pComStepper->opens(stepper_port_path);
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
    Cstepper_uControlXYZ();//::Cstepper_uControlXYZ();
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
            const std::string&  reader_port_path,const std::string&  reader_port_type, const cimg_library::CImg<int> &jitter)
  {
#if cimg_debug>1
std::cerr<<class_name<<"::"<<__func__<<"("<<stepper_port_path<<","<<stepper_port_type
  <<","<<reader_port_path<<","<<reader_port_type
  <<", jitter=("<<jitter(0)<<","<<jitter(1)<<","<<jitter(2)<<") "
  <<")\n"<<std::flush;
#endif
    //choose serial
    Cserial_factory serial_factory;
    pComStepper=serial_factory.create(stepper_port_type);
    pComReader= serial_factory.create( reader_port_type);
    //initialise
    if(!pComStepper->opens(stepper_port_path)) return false;
    if(!pComReader->opens(  reader_port_path)) return false;
    mechanical_jitter=jitter;
    return true;
  }//open

int move_backward(int axe, int position,int index,int step_value, cimg_library::CImg<int> velocity,const int mj)//,Cstepper &stepper)
{
        cimg_library::CImg<int> step(3);step=0;
        if( (position>index) )
        {//move backward: 2 moves
//cerr<<__FILE__<<"/"<<__func__<<"(axis="<<axe<<" i.e. "<<axis_name[axe]<<",step="<<step_value<<").\n"<<std::flush;
        step(axe)=-(mj-step_value);
//cerr<<__FILE__<<"/"<<__func__<<"/move by "<<step(axe)<<".\n"<<std::flush;
        if(!Cstepper_uControlXYZ::move(step,velocity)) return 1;  // move backward
        //stepx=cimg::vector(step(0),step(1),step(2));//=step;
        step(axe)=mj;
//cerr<<__FILE__<<"/"<<__func__<<"/move by "<<step(axe)<<".\n"<<std::flush;
        if(!Cstepper_uControlXYZ::move(step,velocity)) return 1;  // move forward 
        }//move backward
  return 0;
}//move_backward
//!
/**
 *
 * \param [in] position: current_position (to check forward or backward movement)
 * \param [in] index: target_position (to check forward or backward movement)
 * \param [in] step_value: step (should be current_position-target_position)
**/
int move_forward(int axe, int position,int index,int step_value, cimg_library::CImg<int> velocity)//, cimg_library::CImg<int> mj,Cstepper &stepper)
{
      cimg_library::CImg<int> step(3);step=0;
       if( (position<index) ) 
         {//move forward 
//cerr<<__FILE__<<"/"<<__func__<<"(axis="<<axe<<" i.e. "<<axis_name[axe]<<",step="<<step_value<<").\n"<<std::flush;
         step(axe)=step_value;   
//cerr<<__FILE__<<"/"<<__func__<<"/move by "<<step(axe)<<".\n"<<std::flush;
///**** move along Y axis for next position 
        if(!Cstepper_uControlXYZ::move(step,velocity)) return 1;  // move forward 
///**** wait a while for user 
//        cimg_library::cimg::wait(wait_time); 
         }//move forward 
  return 0;
}//move_forward

  //! move once using 1D step and 1D velocity along specific axis
  /** 
   *
   * @param[in] axis_index=axis index (using axis[axis_index])
   * @param[in] step=steps along specified axis
   * @param[in] velocity=velocity for axis
   *
   * @return true on success, false otherwise
   */
  bool move(const int axis_index,const int target_position,const cimg_library::CImg<int> &velocity)
  {
    cimg_library::CImg<int>  current_position(3);
    ///null displacement
    //get current position 
    this->position(current_position);
    if(target_position==current_position(axis_index)) return true;
    ///other displacement
//! \todo [high] . force absolute position
    int mj=10;//mechanical_jitter(axis_index);
std::cerr<<__FILE__<<"/"<<__func__<<"/mechanical_jitter="<<mj<<".\n"<<std::flush;
    int no_loopesz = 0; 
    while( (target_position!=current_position(axis_index)) )
    {
      no_loopesz++; 
//! \bug: add range for security reason in physical space
//if(current_position(axis_index)>60||current_position(axis_index)<-60) {cerr<<"\nERROR: SECURITY STOP ("<<__FILE__<<"/"<<__func__<<").\n\n"<<std::flush;return false;}
//cerr<<__FILE__<<"/"<<__func__<<"/move_*: target="<<target_position<<", current="<<current_position(axis_index)<<".\n"<<std::flush;
      move_backward(axis_index,current_position(axis_index),target_position,target_position-current_position(axis_index),velocity,mj);//,stepper);
      move_forward( axis_index,current_position(axis_index),target_position,target_position-current_position(axis_index),velocity);//,mj,stepper);

      ///get current position 
      this->position(current_position);
//******************************************************** 
std::cout << "index = " << target_position <<  std::endl; 
std::cout << axis_name[axis_index] << " = " << current_position(axis_index) <<  std::endl; 
std::cout << "no_loopesz = " << no_loopesz <<  std::endl;
std::cout << "mj = " << mj <<  std::endl;  
//********************************************************

      if (no_loopesz > 4) 
      { 
        mj-=1;
        no_loopesz=0; 
      } 
    }//move loop
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
      if(!move(i,position(i),velocity))
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


class Cstepper_AVRmatrix: public Cstepper
{
private:
  int value;
public:
  //! constructor
  /**
   *
  **/
  Cstepper_AVRmatrix()
  {
    Cstepper();
    ///set class name (debug only)
#if cimg_debug>1
    class_name="Cstepper_AVRmatrixi";
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
  virtual bool open(const std::string& stepper_port_path,const std::string& stepper_port_type,
            const std::string&  reader_port_path,const std::string&  reader_port_type, const cimg_library::CImg<int> &jitter)
  {
    std::cerr<<"warning: this is matrixIxirtam/AVR.matrix stepper opening.\n"<<std::flush;
    value=0;
    mechanical_jitter=0;
    return true;
  }//open

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
    std::cerr<<"warning: this is matrixIxirtam/AVR.matrix stepper moving.\n"<<std::flush;
//! \todo [high] . set current index with matrixi (i.e. make upload)
    ///set value for matrixIxirtam/matrix device
    if(value==0) value=1; else value=value<<step(0);
std::cerr<<"step(x,y,z)=("<<step(0)<<","<<step(1)<<","<<step(2)<<"), value="<<value<<".\n"<<std::flush;
    ///setup AVR.matrixi with value
//! \todo [medium] . set following using stepper_port_path: 'cd ' and stepper_port_type: './set_value.sh'.
    std::string upload="cd /home/coudert/code.matrixi/matrixi/AVR.matrix; ./set_value.sh "+valueToString(value);
    int error=std::system(upload.c_str());
    if(error!=0)
    {
      std::cerr<<"error: Unable to setup matrixIxirtam/AVR.matrxi from "<<upload<<" (i.e. std::system error code="<<error<<").\n";
      return false;
    }
std::cerr<<"done\n\n"<<std::flush;
cimg_library::cimg::wait(2000);
     return true;
  }//move

  //! Close stepper
  /** 
   *
   * @return 
   */
  virtual bool close()
  {
    std::cerr<<"warning: this is matrixIxirtam/AVR.matrix stepper closing.\n"<<std::flush;
    return true;
  }//close

};//Cstepper_AVRmatrix class

#endif //STEPPER

