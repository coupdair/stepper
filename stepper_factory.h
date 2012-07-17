#ifndef STEPPER_FACTORY
#define STEPPER_FACTORY

#include "stepper.h"

class Cstepper_factory
{
public:
  //! class name for debug only
#if cimg_debug>1
  std::string class_name;
#endif

  //! constructor
  /**
   *
  **/
  Cstepper_factory()
  {
#if cimg_debug>1
    class_name="Cstepper_factory";
#endif
  }//constructor

  //! create a stepper of a specific \c type
  /** 
   *
   * @param[in] type: type of port (e.g. type==1 <=> \c Cstepper_uControlXYZ type)
   *
   * @return 
   */
  Cstepper* create(int type)
  {
#if cimg_debug>1
std::cerr<<class_name<<"::"<<__func__<<"("<<type<<")\n"<<std::flush;
#endif
   switch(type)
    {
      case 0: return new Cstepper;  break;
//! \todo Cstepper_reader with random do,stall,jump
//      case 1: return new Cstepper_reader;  break;
      case 2: return new Cstepper_uControlXYZ;  break;
      case 3: return new Cstepper_uControlXYZ_reader; break;
      case 4: return new Cstepper_AVRmatrix; break;
      default:
      {
        std::cerr<<class_name<<"::"<<__func__<<": error: stepper type="<<type<<" is unknown\n"<<std::flush;
        return NULL; break;
      }
    }
  }//create
  //! create a stepper of a specific \c type
  /** 
   *
   * @param[in] type: type of port (e.g. type=="uControlXYZ" <=> \c Cstepper_uControlXYZ type)
   *
   * @return 
   */
  Cstepper* create(std::string type_name)
  {
#if cimg_debug>1
std::cerr<<class_name<<"::"<<__func__<<"("<<type_name<<")\n"<<std::flush;
#endif
    if(type_name=="fake") return create(0);
    else if(type_name=="fake_reader") return create(1);
    else if(type_name=="uControlXYZ") return create(2);
    else if(type_name=="uControlXYZnReader") return create(3);
    else if(type_name=="matrixiAVRmatrix") return create(4);
    else
    {
      std::cerr<<class_name<<"::"<<__func__<<": error: stepper type="<<type_name<<" not handled.\n"<<std::flush;
      return NULL;
    }
  }//create
//  Cstepper* create(CNetCDFParameter &fp)

};//Cstepper_factory class

#endif //STEPPER_FACTORY

