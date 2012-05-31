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

int main(int argc, char *argv[])
{ 
//commmand line options
 ///usage
  cimg_usage(std::string("stepper program of the Laboratory of Mechanics in Lille (LML) is intended to make translation displacement using a stepping motor, \
it uses different GNU libraries (see --info option)\n\n \
usage: ./stepper -h -I\n \
       ./stepper -nx 10 -sx 1 -vx 1000 --device-type uControlXYZ\n \
version: "+std::string(VERSION)+"\n compilation date: " \
            ).c_str());//cimg_usage
  ///information and help
  const bool show_h   =cimg_option("-h",    false,NULL);//-h hidden option
        bool show_help=cimg_option("--help",show_h,"help (or -h option)");show_help=show_h|show_help;//same --help or -h option
  bool show_info=cimg_option("-I",false,NULL);//-I hidden option
  if( cimg_option("--info",show_info,"show compilation options (or -I option)") ) {show_info=true;cimg_library::cimg::info();}//same --info or -I option
  ///device
  const std::string DeviceType=cimg_option("--device-type","uControlXYZ","type of stepper device");
  const std::string DevicePath=cimg_option("--device-path","/dev/ttyUSB0","path of stepper device");
  const std::string SerialType=cimg_option("--serial-type","serial_system","type of serial device for stepper (i.e. serial_termios or serial_system)");
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
  const bool scan=cimg_option("--scan",false,"set volume scanning mode, else default mode is moving mode (i.e. 3D linear movement).");
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
  ///stop if help requested
  if(show_help) {/*print_help(std::cerr);*/return 0;}
//stepper device object
  Cstepper stepper;
// OPEN 
  if(!stepper.open(DevicePath,SerialType)) return 1;
//MOVE
  if(!scan)
  {//3D move
  std::cerr<<"information: move mode\ndisplacement along "<<
  #ifdef cimg_use_vt100
  cimg_library::cimg::t_green<<
  #endif
  "(X,Y,Z)=("<<number_move*step(0)<<","<<number_move*step(1)<<","<<number_move*step(2)<<") steps"<<
  #ifdef cimg_use_vt100
  cimg_library::cimg::t_normal<<
  #endif
  " at (vX,vY,vZ)=("<<velocity(0)<<","<<velocity(1)<<","<<velocity(2)<<") step(s) per second speed.\n"<<std::flush;
  for(int i=0;i<number_move;++i)
  {
    std::cerr << "actual displacement along "<<
    #ifdef cimg_use_vt100
    cimg_library::cimg::t_green<<
    #endif
    "(X,Y,Z)=("<<i*step(0)<<","<<i*step(1)<<","<<i*step(2)<<") steps"<<
    #ifdef cimg_use_vt100
    cimg_library::cimg::t_normal<<
    #endif
    " over entire displacement of ("<<number_move*step(0)<<","<<number_move*step(1)<<","<<number_move*step(2)<<") steps.\n"<<std::flush;
    if(!stepper.move(step,velocity)) return 1;
    cimg_library::cimg::wait(wait_time);
  }//step loop
  }//move
//SCAN
  else
  {
std::cerr<<"error: "<<
#ifdef cimg_use_vt100
cimg_library::cimg::t_red<<
#endif
"scan mode not implemented, yet !\n"<<
#ifdef cimg_use_vt100
cimg_library::cimg::t_normal<<
#endif
std::flush;
  }//volume scan
//CLOSE
  stepper.close();
  return 0;
}//main

