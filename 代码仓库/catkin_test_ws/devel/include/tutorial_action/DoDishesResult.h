// Generated by gencpp from file tutorial_action/DoDishesResult.msg
// DO NOT EDIT!


#ifndef TUTORIAL_ACTION_MESSAGE_DODISHESRESULT_H
#define TUTORIAL_ACTION_MESSAGE_DODISHESRESULT_H


#include <string>
#include <vector>
#include <map>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>


namespace tutorial_action
{
template <class ContainerAllocator>
struct DoDishesResult_
{
  typedef DoDishesResult_<ContainerAllocator> Type;

  DoDishesResult_()
    : total_is_washed(0)  {
    }
  DoDishesResult_(const ContainerAllocator& _alloc)
    : total_is_washed(0)  {
  (void)_alloc;
    }



   typedef uint32_t _total_is_washed_type;
  _total_is_washed_type total_is_washed;





  typedef boost::shared_ptr< ::tutorial_action::DoDishesResult_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::tutorial_action::DoDishesResult_<ContainerAllocator> const> ConstPtr;

}; // struct DoDishesResult_

typedef ::tutorial_action::DoDishesResult_<std::allocator<void> > DoDishesResult;

typedef boost::shared_ptr< ::tutorial_action::DoDishesResult > DoDishesResultPtr;
typedef boost::shared_ptr< ::tutorial_action::DoDishesResult const> DoDishesResultConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::tutorial_action::DoDishesResult_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::tutorial_action::DoDishesResult_<ContainerAllocator> >::stream(s, "", v);
return s;
}


template<typename ContainerAllocator1, typename ContainerAllocator2>
bool operator==(const ::tutorial_action::DoDishesResult_<ContainerAllocator1> & lhs, const ::tutorial_action::DoDishesResult_<ContainerAllocator2> & rhs)
{
  return lhs.total_is_washed == rhs.total_is_washed;
}

template<typename ContainerAllocator1, typename ContainerAllocator2>
bool operator!=(const ::tutorial_action::DoDishesResult_<ContainerAllocator1> & lhs, const ::tutorial_action::DoDishesResult_<ContainerAllocator2> & rhs)
{
  return !(lhs == rhs);
}


} // namespace tutorial_action

namespace ros
{
namespace message_traits
{





template <class ContainerAllocator>
struct IsFixedSize< ::tutorial_action::DoDishesResult_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::tutorial_action::DoDishesResult_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::tutorial_action::DoDishesResult_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::tutorial_action::DoDishesResult_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::tutorial_action::DoDishesResult_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::tutorial_action::DoDishesResult_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::tutorial_action::DoDishesResult_<ContainerAllocator> >
{
  static const char* value()
  {
    return "5b3bbd7ecb15f94222601861cfb31b9a";
  }

  static const char* value(const ::tutorial_action::DoDishesResult_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0x5b3bbd7ecb15f942ULL;
  static const uint64_t static_value2 = 0x22601861cfb31b9aULL;
};

template<class ContainerAllocator>
struct DataType< ::tutorial_action::DoDishesResult_<ContainerAllocator> >
{
  static const char* value()
  {
    return "tutorial_action/DoDishesResult";
  }

  static const char* value(const ::tutorial_action::DoDishesResult_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::tutorial_action::DoDishesResult_<ContainerAllocator> >
{
  static const char* value()
  {
    return "# ====== DO NOT MODIFY! AUTOGENERATED FROM AN ACTION DEFINITION ======\n"
"# resault\n"
"uint32 total_is_washed\n"
;
  }

  static const char* value(const ::tutorial_action::DoDishesResult_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::tutorial_action::DoDishesResult_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.total_is_washed);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct DoDishesResult_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::tutorial_action::DoDishesResult_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::tutorial_action::DoDishesResult_<ContainerAllocator>& v)
  {
    s << indent << "total_is_washed: ";
    Printer<uint32_t>::stream(s, indent + "  ", v.total_is_washed);
  }
};

} // namespace message_operations
} // namespace ros

#endif // TUTORIAL_ACTION_MESSAGE_DODISHESRESULT_H
