//stepper
/*-----------------------------------------------------------------------
  File        : stepper.cpp
  Description : stepper motor for translation displacement program of the Laboratory of Mechanics in Lille (LML)
  Authors     : Sebastien COUDERT
-----------------------------------------------------------------------*/

/*Additional documentation for the generation of the reference page (using doxygen)*/
/** \mainpage
 *
 *  index:
 *  \li \ref sectionCommandLine
 *  \li \ref sectionStepperDocumentation
 *  \li \ref sectionDoxygenSyntax
 *
 *  \section sectionCommandLine command line options
 *
 *  \verbinclude "stepper.help.output"
 *  
 *  \section sectionStepperDocumentation documentation outline
 *  This is the reference documentation of <a href="http://www.meol.cnrs.fr/">serial</a>, from the <a href="http://www.univ-lille1.fr/lml/">LML</a>.\n\n
 *  stepper software. The main function is in <a href="stepper_8cpp.html">stepper.cpp</a> source file.\n\n
 *  This documentation has been automatically generated from the sources, 
 *  using the tool <a href="http://www.doxygen.org">doxygen</a>. It should be readed as HTML, LaTex and man page.\n
 *  It contains both
 *  \li a detailed description of all classes and functions
 *  \li TODO: a user guide (cf. \ref pages.html "related pages")
 *
 *  that as been documented in the sources.
 *
 *  \par Additional needed libraries:
 *
 *  \li <a href="http://cimg.sourceforge.net">the CImg Library</a> (v1.?.?) for command line options only.
 *
 *  \section sectionDoxygenSyntax make documentation using Doxygen syntax
 *  Each function in the source code should be commented using \b doxygen \b syntax in the same file.
 *  The documentation need to be written before the function.
 *  The basic syntax is presented in this part.
 *  \verbinclude "doxygen.example1.txt"
 *
 *  Two kind of comments are needed for \b declaration and \b explanation \b parts of the function:
 *  Standart documentation should the following (\b sample of code documentation):
 *  \verbinclude "doxygen.example2.txt"
 *
 *  In both declaration and explanation part, \b writting and \b highlithing syntax can be the following:\n\n
 *  \li \c \\code to get\n
 *
 *  \li \c \\n    a new line
 *  \li \c \\li   a list (dot list)
 *
 *  \li \c \\b    bold style
 *  \li \c \\c    code style
 *  \li \c \\e    enhanced style (italic)
 *
 *  For making \b shortcut please use:\n
 *  \li \c \\see to make a shortcut to a related function or variable
 *  \li \c \\link to make a shortcut to a file or a function
 *  \note this keyword needs to be closed using \c \\end*
 *
 *  \li \c \\todo to add a thing to do in the list of <a href="todo.html">ToDo</a> for the whole program
 *
 *  In explanation part, \b paragraph style can be the following:\n
 *  \li \c \\code for an example of the function use
 *  \li \c \\note to add a few notes
 *  \li \c \\attention for SOMETHING NOT FULLY DEFINED YET
 *  \li \c \\warning to give a few warning on the function
 *  \note these keywords need to be closed using \c \\end*
 *
 *  \verbinclude "doxygen.example3.txt"
 *
 *  Many other keywords are defined, so please read the documentation of <a href="http://www.doxygen.org/commands.html">doxygen</a>.
 *
**/

//! \todo serial_put doxygen user page

//standard library
#include <iostream>
//CImg Library
#include "../CImg/CImg.h"
//stepper library
#include "stepper.h"

//! move several time by a 3D displacement (note: no reset to original position)
/**
 * move several time by a 3D displacement i.e. with in a loop makes the same 3D displacement.
 * \note displacement could be either 1D, 2D or 3D in any direction.
 * \note no reset to original position.
 *
 * \param[in] stepper: displacement stage device (should be already open)
 * \param[in] number: number of iteration in the moving loop
 * \param[in] step: 3D displacement in step (i.e. size should be 3)
 * \param[in] velocity: 3D velocity in step per second (i.e. size should be 3)
 * \param[in] wait_time: minimum delay between each loop iteration
**/
int moving(Cstepper &stepper,const int number,const cimg_library::CImg<int> &step,const cimg_library::CImg<int> &velocity,const int wait_time)
{
///input checks
  if(step.size()!=3) return 3;
  if(velocity.size()!=3) return 3;
///movement message
  std::cerr<<"full displacement along "<<
 #ifdef cimg_use_vt100
  cimg_library::cimg::t_purple<<
 #endif
  "(X,Y,Z)=("<<number*step(0)<<","<<number*step(1)<<","<<number*step(2)<<") steps"<<
 #ifdef cimg_use_vt100
  cimg_library::cimg::t_normal<<
 #endif
  " at (vX,vY,vZ)=("<<velocity(0)<<","<<velocity(1)<<","<<velocity(2)<<") step(s) per second speed.\n"<<std::flush;
///linear 3D displacement loop
  for(int i=0;i<number;++i)
  {
///* position message
    std::cerr << "moving by "<<
   #ifdef cimg_use_vt100
    cimg_library::cimg::t_purple<<
   #endif
    "(X,Y,Z)=("<<step(0)<<","<<step(1)<<","<<step(2)<<") steps"<<
   #ifdef cimg_use_vt100
    cimg_library::cimg::t_normal<<
   #endif
    " over entire displacement of ("<<number*step(0)<<","<<number*step(1)<<","<<number*step(2)<<") steps.\n"<<std::flush;
///* move
    if(!stepper.move(step,velocity)) return 1;
///* position message
    std::cerr << "actual position "<<
   #ifdef cimg_use_vt100
    cimg_library::cimg::t_green<<
   #endif
    "(X,Y,Z)=("<<i*step(0)<<","<<i*step(1)<<","<<i*step(2)<<")"<<
   #ifdef cimg_use_vt100
    cimg_library::cimg::t_normal<<
   #endif
    " steps.\n"<<std::flush;
///* wait a while for user
    cimg_library::cimg::wait(wait_time);
  }//step loop
  return 0;
}//moving

//! scan a full volume, i.e. all positions of the volume (note: reset to original position with mechanical jitter)
/**
 * scan a volume, i.e. make all displacement along the 3 axes to do each position in the volume.
 * \note reset to original position, especially in order to make 3D position scanning in each direction loop.
 * \note could be used as 1D line, column or depth scanning for any axis scan, or even 2D plane or slice scanning.
 *
 * \param[in] stepper: displacement stage device (should be already open)
 * \param[in] number: number of iteration in each of the scanning loops (i.e. number of positions for the 3 axes; size should be 3)
 * \param[in] step: 3D displacement in step (i.e. size should be 3)
 * \param[in] velocity: 3D velocity in step per second (i.e. size should be 3)
 * \param[in] wait_time: minimum delay between each loop displacement
 * \param[in] mechanical_jitter: mechanical jitter to perform a good reset for any axes
**/
int scanning(Cstepper &stepper,const cimg_library::CImg<int> &number,const cimg_library::CImg<int> &step,const cimg_library::CImg<int> &velocity,const int wait_time, const unsigned int mechanical_jitter
#if cimg_display>0
,const unsigned int zoom=100,const bool do_display=false
#endif //cimg_display
)
{
///set signed mechanical jitter
  //set mechanical jitter for all axes with same sign as corresponding displacement.
  cimg_library::CImg<int>  mj(3);
  cimg_forX(mj,a) mj(a)=((step(a)>0)?1:-1)*mechanical_jitter;

///reset to (0,0,0)
  stepper.reset();
  cimg_library::CImg<int> position(3);
  stepper.position(position);

///scanning message
  std::cerr<<"full scanning volume of "<<
 #ifdef cimg_use_vt100
  cimg_library::cimg::t_purple<<
 #endif
  "size (sX,sY,sZ)=("<<number(0)*step(0)<<","<<number(1)*step(1)<<","<<number(2)*step(2)<<") steps"<<
 #ifdef cimg_use_vt100
  cimg_library::cimg::t_normal<<
 #endif
  " by "<<
 #ifdef cimg_use_vt100
  cimg_library::cimg::t_purple<<
 #endif
  "displacement (dX,dY,dZ)=("<<step(0)<<","<<step(1)<<","<<step(2)<<") steps"<<
 #ifdef cimg_use_vt100
  cimg_library::cimg::t_normal<<
 #endif
  " at (vX,vY,vZ)=("<<velocity(0)<<","<<velocity(1)<<","<<velocity(2)<<") step(s) per second speed.\n"<<std::flush;

#if cimg_display>0
  //GUI to display scanning progress
  cimg_library::CImg<char> volume(number(0),number(1),number(2));volume.fill(2);//0=fail(red), 1=done(green), 2=to_do(blue)
  //color
//! \todo use \c volume for setting colors to \c colume (e.g. \c red color in case of position failure; need position check stepper)
  const unsigned char red[] = { 255,0,0 }, green[] = { 0,255,0 }, blue[] = { 0,0,255 };
  cimg_library::CImg<unsigned char> colume;
  //display
  cimg_library::CImgDisplay progress;
  if(do_display)
  {
//! \todo assign both \c colume and \c progress for displaying at best an image (i.e. 2D)
    colume.assign(volume.width(),volume.height(),1,3);
    progress.assign(volume.width()*zoom,volume.height()*zoom);//,volume.depth()*zoom);
    progress.set_title("scan progress");
  }
#endif //cimg_display

///* Z axis loop
  //set 1D displacement for Z axis
  cimg_library::CImg<int> stepz(3);stepz.fill(0);stepz(2)=step(2);//e.g. (0,0,10)
  //Z axis loop
  for(int k=0;k<number(2);++k)
  {

#if cimg_display>0
    if(do_display)
    {
    //current slice
      cimg_forXY(colume,x,y) colume.draw_point(x,y,blue);
//! \todo remove slice in title if number(2)==1
      progress.set_title("scan progress (slice#%d/%d)",k,number(2));
    }//do_display
#endif //cimg_display

///** Y axis loop
    //set 1D displacement for Y axis
    cimg_library::CImg<int> stepy(3);stepy.fill(0);stepy(1)=step(1);//e.g. (0,10,0)
    //Y axis loop
    for(int j=0;j<number(1);++j)
    {
///*** X axis loop
      //set 1D displacement for X axis
      cimg_library::CImg<int> stepx(3);stepx.fill(0);stepx(0)=step(0);//e.g. (10,0,0)
      //X axis loop
      for(int i=0;i<number(0);++i)
      {
///**** position message
        std::cerr << "actual displacement to "<<
       #ifdef cimg_use_vt100
        cimg_library::cimg::t_green<<
       #endif
        "(X,Y,Z)=("<<i*step(0)<<","<<j*step(1)<<","<<k*step(2)<<") "<<
       #ifdef cimg_use_vt100
        cimg_library::cimg::t_normal<<
       #endif
        "step position over entire scanning of ("<<number(0)*step(0)<<","<<number(1)*step(1)<<","<<number(2)*step(2)<<") steps.\n"<<std::flush;
#if cimg_display>0
        //set status
        stepper.position(position);
        if( (position(0)==i*step(0))
          &&(position(1)==j*step(1))
          &&(position(2)==k*step(2)) )
          volume(i,j,k)=1;
        else
          volume(i,j,k)=0;
        if(do_display)
        {
          //GUI to display scanning progress
          colume.draw_point(i,j,(volume(i,j,k)==1)?green:red);
          progress.display(colume.get_resize(zoom));
        }//do_display
#endif //cimg_display
///**** move along X axis
        //move along X axis
        if(number(0)>1)
        {//X move only if more than one line to do
          if(!stepper.move(stepx,velocity)) return 1;
        }//X move
///**** wait a while for user
        cimg_library::cimg::wait(wait_time);
      }//X axis loop
///*** reset X axis
      //go back to zero on X axis (i.e. move backward along X axis)
      if(number(0)>1)
      {//X reset (with mechanical jitter)
        // 0. reset message
        std::cerr<<
       #ifdef cimg_use_vt100
        cimg_library::cimg::t_purple<<
       #endif
        "reset X axis to (X,Y,Z)=(0,"<<j*step(1)<<","<<k*step(2)<<")"<<
       #ifdef cimg_use_vt100
        cimg_library::cimg::t_normal<<
       #endif
        ".\n"<<std::flush;
        // 1. move backward with mechanical jitter in X step // mechanical jitter = mj
        stepx(0)=-(step(0)*number(0)+mj(0));
        if(!stepper.move(stepx,velocity)) return 1;
        cimg_library::cimg::wait(wait_time);
        // 2. move forward mechanical jitter in X step
        stepx(0)=mj(0);
        if(!stepper.move(stepx,velocity)) return 1;
        cimg_library::cimg::wait(wait_time);
      }//X reset
///*** move along Y axis
      //move along Y axis
      if(number(1)>1)
      {//Y move only if more than one column to do
        if(!stepper.move(stepy,velocity)) return 1;
      }//Y move
///*** wait a while for user
      cimg_library::cimg::wait(wait_time);
    }//Y axis loop
///** reset Y axis
    //go back to zero on Y axis (i.e. move backward along Y axis)
    if(number(1)>1)
    {//Y reset (with mechanical jitter)
      // 0. reset message
      std::cerr<<
     #ifdef cimg_use_vt100
      cimg_library::cimg::t_purple<<
     #endif
      "reset Y axis to (X,Y,Z)=(0,0,"<<k*step(2)<<")"<<
     #ifdef cimg_use_vt100
      cimg_library::cimg::t_normal<<
     #endif
      ".\n"<<std::flush;
      // 1. move backward with mechanical jitter in Y step // mechanical jitter = mj
      stepy(1)=-(step(1)*number(1)+mj(1));
      if(!stepper.move(stepy,velocity)) return 1;
      cimg_library::cimg::wait(wait_time);
      // 2. move forward mechanical jitter in Y step
      stepy(1)=mj(1);
      if(!stepper.move(stepy,velocity)) return 1;
      cimg_library::cimg::wait(wait_time);
    }//Y reset
///** move along Z axis
    //move along Z axis
    if(number(2)>1)
    {//Z move only if more than one slice to do
      if(!stepper.move(stepz,velocity)) return 1;
    }//Z move
///** wait a while for user
    cimg_library::cimg::wait(wait_time);
  }//Z axis loop
///* reset Z axis
  //go back to zero on Z axis (i.e. move backward along Z axis)
  if(number(2)>1)
  {//Z reset (with mechanical jitter)
    // 0. reset message
    std::cerr<<
   #ifdef cimg_use_vt100
    cimg_library::cimg::t_purple<<
   #endif
    "reset Z axis to (X,Y,Z)=(0,0,0)"<<
   #ifdef cimg_use_vt100
    cimg_library::cimg::t_normal<<
   #endif
    ".\n"<<std::flush;
    // 1. move backward with mechanical jitter in Z step // mechanical jitter = mj
    stepz(2)=-(step(2)*number(2)+mj(2));
    if(!stepper.move(stepz,velocity)) return 1;
    cimg_library::cimg::wait(wait_time);
    // 2. move forward mechanical jitter in Z step
    stepz(2)=mj(2);
    if(!stepper.move(stepz,velocity)) return 1;
    cimg_library::cimg::wait(wait_time);
  }//Z reset

#if cimg_display>0
  //close GUI
  progress.close();
#endif //cimg_display

  //check back to origin
  stepper.position(position);
  if( (position(0)!=0)
    ||(position(1)!=0)
    ||(position(2)!=0) )
     return -1;

  return 0;
}//scanning

int main(int argc, char *argv[])
{ 
//commmand line options
 ///usage
  cimg_usage(std::string("stepper program of the Laboratory of Mechanics in Lille (LML) is intended to make translation displacement using a stepping motor, \
it uses different GNU libraries (see --info option)\n\n \
usage: ./stepper -h -I #help and compilation information\n \
       ./stepper -n 10 -sx 1 -vx 1000 --device-type uControlXYZ #3D linear moving mode\n \
       ./stepper --scan true -nx 10 -sx 1 -vx 1000 -ny 5 -sy 1 -vy 1000 #volume scanning mode\n \
version: "+std::string(STEPPER_VERSION)+"\t(other library versions: RS232."+std::string(RS232_VERSION)+")\n compilation date: " \
            ).c_str());//cimg_usage
  ///information and help
  const bool show_h   =cimg_option("-h",    false,NULL);//-h hidden option
        bool show_help=cimg_option("--help",show_h,"help (or -h option)");show_help=show_h|show_help;//same --help or -h option
  bool show_info=cimg_option("-I",false,NULL);//-I hidden option
  if( cimg_option("--info",show_info,"show compilation options (or -I option)") ) {show_info=true;cimg_library::cimg::info();}//same --info or -I option
  ///device
  const std::string StepperDeviceType=cimg_option("--device-type","uControlXYZ","type of stepper device");
  const std::string StepperDevicePath=cimg_option("--stepper-device-path","/dev/ttyUSB0","path of stepper device");
  const std::string StepperSerialType=cimg_option("--stepper-serial-type","serial_system","type of serial device for stepper (i.e. serial_termios or serial_system)");
  const std::string  ReaderDevicePath=cimg_option("--position-reader-device-path","/dev/ttyUSB1","path of position reader device");
  const std::string  ReaderSerialType=cimg_option("--position-reader-serial-type","serial_system","type of serial device for position reader (i.e. serial_termios or serial_system)");
  ///displacement
  cimg_library::CImg<int> step(3);step.fill(0);
  {
  const int step_x=cimg_option("-sx",100,"displacement step along X axis (e.g. -10 steps to go backward, i.e. displacement can be positive or negative).");
  step(0)=step_x;
  const int step_y=cimg_option("-sy",100,"displacement step along Y axis.");
  step(1)=step_y;
  const int step_z=cimg_option("-sz",100,"displacement step along Z axis.");
  step(2)=step_z;
  }//step
  ///velocity
  cimg_library::CImg<int> velocity(3);velocity.fill(0);
  {
  const unsigned int velocity_x=cimg_option("-vx",1000,"displacement velocity along X axis (note: absolute velocity, i.e. could not be negative).");
  velocity(0)=velocity_x;
  const unsigned int velocity_y=cimg_option("-vy",1000,"displacement velocity along Y axis.");
  velocity(1)=velocity_y;
  const unsigned int velocity_z=cimg_option("-vz",1000,"displacement velocity along Z axis.");
  velocity(2)=velocity_z;
  }//velocity
  ///number of steps
  const int number_move=cimg_option("-n",10,"number of displacement in move mode only, i.e. default option (e.g. --scan false option).");
  const bool do_scan=cimg_option("--scan",false,"set volume scanning mode, else default mode is moving mode (i.e. 3D linear movement).");
  const unsigned int mechanical_jitter=cimg_option("--jitter",10,"set mechanical jitter to go back to origin for scanning mode only (i.e. reset position with same mechanical direction, could not be negative).");
  cimg_library::CImg<int> number(3);number.fill(1);
  {
  const int number_x=cimg_option("-nx",10,"number of displacement along X axis (set --scan true option).");
  number(0)=number_x;
  const int number_y=cimg_option("-ny",10,"number of displacement along Y axis (set --scan true option).");
  number(1)=number_y;
  const int number_z=cimg_option("-nz", 1,"number of displacement along Z axis (set --scan true option).");
  number(2)=number_z;
  }//number
  ///wait time between steps
  const int wait_time=cimg_option("--wait-time",500,"wait time between steps in ms.");
#if cimg_display>0
  const bool do_display=cimg_option("-X",false,"activate GUI (i.e. progress display during scan mode only; set --scan true option).");
  const unsigned int zoom=cimg_option("--GUI-progress-zoom",100,"GUI progress display zoom.");
#endif
  ///stop if help requested
  if(show_help) {/*print_help(std::cerr);*/return 0;}
//stepper device object
  Cstepper stepper;
// OPEN 
  if(!stepper.open(StepperDevicePath,StepperSerialType,ReaderDevicePath,ReaderSerialType)) return 1;
  int error;
//MOVE
  if(!do_scan)
  {//3D move
    std::cerr<<"information: move mode\n";
    error=moving(stepper,number_move,step,velocity,wait_time);
  }//move
//SCAN
  else
  {
    std::cerr<<"information: scan mode\n";
    error=scanning(stepper,number,step,velocity,wait_time,mechanical_jitter,zoom,do_display);
    if(error==-1)
    {
      std::cerr<<
     #ifdef cimg_use_vt100
      cimg_library::cimg::t_green<<
     #endif
      "warning: scanning function did not return to origin !"<<
     #ifdef cimg_use_vt100
      cimg_library::cimg::t_normal<<
     #endif
      " (need manual origin reset)\n";
    }
  }//volume scan
//CLOSE
  stepper.close();
  if(error!=0) {std::cerr<<"error: "<<std::string(do_scan?"scann":"mov")<<"ing function returns error="<<error<<".\n"; return error;}
  return 0;
}//main

